#pragma once

#include <algorithm>  // std::forward

template <class> class function;

template <class R, class... Args>
class function<R(Args...)> {

	// �֐���ێ����邽�߂̉��z�֐��e�[�u��
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

	// �֐��|�C���^��ێ�����R���X�g���N�^
	template <class Functor>
	function(Functor&& f) {

		// �t�@���N�^��ێ����邽�߂̉��z�֐��e�[�u��
		static Vtable staticVTable = {
			[](void* functionPtr, Args&&... args) -> R {
				return (*reinterpret_cast<Functor*>(functionPtr))(std::forward<Args>(args)...);
			},
			[](void* functionPtr) { delete reinterpret_cast<Functor*>(functionPtr); }
		};

		// ���z�֐��e�[�u���ɃZ�b�g
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
