#pragma once

#include <algorithm>  // std::forward

template <class> class function;

template <class R, class... Args>
class function<R(Args...)> {

	// 関数を保持するための仮想関数テーブル
	struct Vtable {
		R(*call)(void* function_ptr, Args&&... args);
		void (*destroy)(void* function_ptr);
	};

	Vtable* vTable;

	void* functionPtr;

public:

	function() noexcept
		: functionPtr(nullptr)
		, vTable(nullptr)
	{}

	// 関数ポインタを保持するコンストラクタ
	template <class Functor>
	function(Functor&& f) {

		// ファンクタを保持するための仮想関数テーブル
		static Vtable staticVTable = {
			[](void* function_ptr, Args&&... args) -> R {
				return (*reinterpret_cast<Functor*>(function_ptr))(std::forward<Args>(args)...);
			},
			[](void* function_ptr) { delete reinterpret_cast<Functor*>(function_ptr); }
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
