#pragma once

#include <algorithm>  // std::forward

template <class> class function;

template <class R, class... Args>
class function<R(Args...)> {

	// �֐���ێ����邽�߂̉��z�֐��e�[�u��
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

	// �֐��|�C���^��ێ�����R���X�g���N�^
	template <class Functor>
	function(Functor&& f) {

		// �t�@���N�^��ێ����邽�߂̉��z�֐��e�[�u��
		static Vtable staticVTable = {
			[](void* function_ptr, Args&&... args) -> R {
				return (*reinterpret_cast<Functor*>(function_ptr))(std::forward<Args>(args)...);
			},
			[](void* function_ptr) { delete reinterpret_cast<Functor*>(function_ptr); }
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

	template <class T>
	function(const function<T>&) = delete;

	template <class T>
	function(function<T>&&) = delete;

};
