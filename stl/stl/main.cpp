#include <iostream>

#include "functional.hpp"

int main() {

	{
		// 最小構成ラムダ式
		casey::function<void()> f = []() {
			std::cout << "a";
		};
		f();

		std::cout << std::endl;
	}

	{
		// キャプチャ有りラムダ式

		int i = 1234;
		casey::function<void()> f = [&]() {
			std::cout << i;
		};
		f();

		std::cout << std::endl;
	}

	{
		// 引数ありラムダ式
		casey::function<void(int)> f = [](int value) {
			std::cout << value;
		};
		f(100);

		std::cout << std::endl;
	}


	{
		// 戻り値ありラムダ式
		casey::function<int()> f = [&]() {
			return 200;
		};
		std::cout << f();

		std::cout << std::endl;
	}

	{
		// casey::bad_function_call 例外
		casey::function<void()> f;
		try {
			f();
		}
		catch (const casey::exception& e) {
			std::cout << e.what();
		}
	}
}