#pragma once

#include "functional.hpp"
#include <memory>
#include <any>
class any {
	function<void()> access;
	std::shared_ptr<void*> p;
public:
	template<class Ty>
	any& operator=(const Ty& r) {
		return *this;
	}
	template<class Ty>
	any(const Ty& r)
		: p(static_cast<void*>(new(r)))
	{
		static Vtable staticVtable = {

		}
		vtable = &staticVtable;
	}


	template<class Ty>
	friend Ty cast() {
		return access();
	};


};
