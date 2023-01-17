#include <iostream>

#include "functional.hpp"


int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	{
		// 関数クロージャーを入れよう！
		casey::function<void()> f = []() {
			std::cout << "a";
		};
		f();

		casey::function<void()> ff;
		ff = std::move(f);
		ff();

	}

	{

	}



}