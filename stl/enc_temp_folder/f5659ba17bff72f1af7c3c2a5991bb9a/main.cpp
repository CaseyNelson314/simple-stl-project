#include <iostream>

#include "functional.hpp"
#include <functional>

int main()
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	{
		// 関数クロージャーを入れよう！
		casey::function<void()> f = []() {
			std::cout << "a";
		};
		f();

		casey::function<void()> ff[100];

		casey::function<void()> ff0 = []() {
			std::cout << "a";
		};

		casey::function<void()> ff1;

		ff1 = std::move(ff0);

		ff0();
		

	}

}