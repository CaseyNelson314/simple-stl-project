#pragma once

#include <algorithm>  // std::forward

template <class> class function;

template <class R, class... Args>
class function<R(Args...)> {

	// 関数を保持するための仮想関数テーブル
	struct Vtable {
		R(*call)(void* functionPtr, Args&&... args);
		void (*destroy)(void* functionPtr);
	};

	Vtable* vTable;

	void* functionPtr;

public:

	function() noexcept
		: functionPtr()
		, vTable()
	{}

	// 関数ポインタを保持するコンストラクタ
	template <class Functor>
	function(Functor&& f) {

		// ファンクタを保持するための仮想関数テーブル
		static Vtable staticVTable = {
			[](void* functionPtr, Args&&... args) -> R {
				return (*reinterpret_cast<Functor*>(functionPtr))(std::forward<Args>(args)...);
			},
			[](void* functionPtr) { delete reinterpret_cast<Functor*>(functionPtr); }
		};

		// 仮想関数テーブルにセット
		functionPtr = new Functor(f);
		vTable = &staticVTable;

	}

	~function() {

		vTable->destroy(functionPtr);

	}

	R operator()(Args&&... args) {

		return vTable->call(functionPtr, std::forward<Args>(args)...);

	}

};
