#ifndef __STL_THREAD__
#define __STL_THREAD__

#include <thread>
#include <iostream>
#include <initializer_list>
#include <chrono>
#include <mutex>
#include <vector>
using namespace std;
namespace stl_mutex
{

#define __EXAMPLE_ID__ 5
	void func0(int *x, mutex& mtx);
	void func1_a(recursive_mutex & mtx);
	void func1_b(recursive_mutex & mtx);
	void func2(timed_mutex & mtx);
	void func3(mutex& mtx);
	void func4(mutex & mtx1, mutex & mtx2);
	void func5_a(once_flag& flag, void func(int), int id);
	void func5_b(int id);
	int _main();
}
#endif // !__STL_THREAD__
