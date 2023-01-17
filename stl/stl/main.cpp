#include <iostream>

#include "functional.hpp"
#include "memory.hpp"
#include <memory>

int main()
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	{
		// 関数クロージャーを入れよう！
		casey::function<void()> a = []() {
			std::cout << "a";
		};
		casey::function<void()> b = []() {
			std::cout << "b";
		};
		std::swap(a, b);

		a();
		b();
	}

}