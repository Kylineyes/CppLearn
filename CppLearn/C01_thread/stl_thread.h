#ifndef __STL_THREAD__
#define __STL_THREAD__

#include <thread>
#include <vector>
#include <iostream>
#include <initializer_list>
#include <chrono>
using namespace std;
namespace stl_thread 
{

#define __EXAMPLE_ID__ 9
	void func0();
	void func2();
	void func3(int count);
	void func4(int x, int y);
	void func8(int sleep_count);

	class LambdaClass {
	public:
		void operator()(int x, int y) 
		{
			cout << x * y << endl;
		}
	};

	int _main();
}

#endif // !__STL_THREAD__
