#include "stl_thread.h"
namespace stl_thread 
{

	int _main()
	{
#if __EXAMPLE_ID__ == 0
		thread tr(func0);
		// 简单的调用一个函数，但是直接这样做会崩溃
		// 在子线程执行完毕前，父线程就结束了任务，所以我们需要join
#elif __EXAMPLE_ID__ == 1
		thread tr(func0);
		tr.join();
		// join 函数在线程执行完成的时候返回；
		// 此函数在函数返回时与线程中所有操作的完成是同步的；
		// 调用join直到join被构造函数调用返回间，阻塞调用的线程；
		// 在调用此函数之后，线程对象变得不可连接，可以安全地销毁。

#elif __EXAMPLE_ID__ == 2
		thread tr(func2);
		tr.detach();
		cout << "main is free" << endl;
		// 此段代码需要贴到真正的入口函数里面执行，
		// 这里会因为函数出栈而析构线程
#elif __EXAMPLE_ID__ == 3
		thread tr(func3, 20);
		tr.join();
		// 线程传入参数
#elif __EXAMPLE_ID__ == 4
		thread tr(func4, 9, 9);
		tr.join();
		// 线程传入多个参数
#elif __EXAMPLE_ID__ == 5
		thread tr([](int x, int y) {
			cout << x * y << endl;
		}, 9, 9);
		tr.join();
		// 一般可以通过匿名函数（实际为函数对象）进行新建线程
#elif __EXAMPLE_ID__ == 6
		LambdaClass task{};
		thread tr{ LambdaClass(), 9, 9 };
		tr.join();
		// 这里，如果使用括号，
		// C++会出现一个解析错误，把tr解析成一个函数的声明形式，
		// 解决方法是使用C11的统一初始化语法避免。
#elif __EXAMPLE_ID__ == 7
		/* 构造函数
		default (1)             thread() noexcept;
		initialization(2)       template <class Fn, class... Args>
		                        explicit thread(Fn&& fn, Args&&... args);
		copy[deleted](3)        thread(const thread&) = delete;
		move(4)                 thread(thread&& x) noexcept;
		(1)默认构造函数
           构造一个不表示任何执行线程的线程对象。
        (2)初始化的构造函数 模版函数
           构建一个线程对象，该对象表示一个新的可接合线程。
		   新的执行线程调用 fn 函数，传递 args 作为参数(使用其lvalue或rvalue引用的衰变副本)。
		   此构建的完成开始同步调用fn副本的。
        (3)拷贝构造
		   不允许拷贝构造
        (4)移动构造
		   构造线程获取 x 线程，这个操作不会影响移动线程的执行，它只会传输它的处理程序。
		   完成后 x 将不再表示一个线程。
		 */
		/*析构函数
		std::thread::~thread
		  破坏了线程对象。如果在销毁时线程是可接合的，则调用终止()。
		std::thread::operator=：
		move (1)            thread& operator= (thread&& rhs) noexcept；
		copy [deleted] (2)  thread& operator= (const thread&) = delete;
		thread不允许拷贝；
		如果对象当前不是joinable的，它将获得由rhs(如果有的话)表示的执行线程。
		如果是joinable，则调用终止()。赋值“=”运算符通过右值表达式，复制后的thread对象不再是一个线程。
		 */
#elif __EXAMPLE_ID__ == 8
		vector<thread> tr_list{};
		cout << "main thread is " << this_thread::get_id() << endl;
		for (int i = 3; i < 7; ++i)
		{
			tr_list.emplace_back(thread{ func8, i });
		}
		for (auto& e : tr_list) e.join();
		// 演示如何获得线程id和让线程休眠
#elif __EXAMPLE_ID__ == 9
		cout << this_thread::get_id() << " is main thread." << endl;
		thread tr1{ func8, 1 };
		thread tr2{ func8, 2 };
		thread tr3{ func8, 4 };
		thread::id id{ tr1.get_id() };
		cout << id << " thread is joinable? " << boolalpha << tr1.joinable() << endl;
		/* std::thread::joinable
		   返回线程对象是否可joinable。
		   如果线程对象表示执行的线程，则是可joinable。
		   在这些情况下，一个线程对象是不可连接的 :
            如果是默认构造。
			如果它已经被移动(或者构造另一个线程对象，或者分配给它)。
			如果它的成员加入或分离被调用。
		 */
		tr1.detach();
		tr2.detach();
		tr3.detach();
		cout << id << " thread is joinable? " << boolalpha << tr1.joinable() << endl;
		func8(3);
		// detach分离出调用线程对象所代表的线程，允许它们彼此独立地执行;
		// 这两个线程在任何方式上都不阻塞或同步；
		// 注意，当一个结束执行时，它的资源被释放。
		// 在调用此函数之后，线程对象变得不可连接，可以安全地销毁。

#endif
		return 0;
	}

	void func0()
	{
		cout << "good" << endl;
	}

	void func2()
	{
		for (int i = 0; i < 100; ++i)
			cout << "good" << endl;
	}

	void func3(int count)
	{
		for (int i = 0; i < count; ++i)
			cout << "good" << endl;
	}

	void func4(int x, int y)
	{
		for (int i = 1; i <= x; ++i)
			for (int j = 1; j <= y; ++j)
				cout << i << "*" << j << "=" << i * j << endl;
	}

	void func8(int sleep_count)
	{
		this_thread::sleep_for(std::chrono::seconds(sleep_count));
		cout << this_thread::get_id() << " is slept for " << sleep_count << "s" << endl;
	}
}