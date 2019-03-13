#include "stl_thread.h"
namespace stl_thread 
{

	int _main()
	{
#if __EXAMPLE_ID__ == 0
		thread tr(func0);
		// �򵥵ĵ���һ������������ֱ�������������
		// �����߳�ִ�����ǰ�����߳̾ͽ�������������������Ҫjoin
#elif __EXAMPLE_ID__ == 1
		thread tr(func0);
		tr.join();
		// join �������߳�ִ����ɵ�ʱ�򷵻أ�
		// �˺����ں�������ʱ���߳������в����������ͬ���ģ�
		// ����joinֱ��join�����캯�����÷��ؼ䣬�������õ��̣߳�
		// �ڵ��ô˺���֮���̶߳����ò������ӣ����԰�ȫ�����١�

#elif __EXAMPLE_ID__ == 2
		thread tr(func2);
		tr.detach();
		cout << "main is free" << endl;
		// �˶δ�����Ҫ������������ں�������ִ�У�
		// �������Ϊ������ջ�������߳�
#elif __EXAMPLE_ID__ == 3
		thread tr(func3, 20);
		tr.join();
		// �̴߳������
#elif __EXAMPLE_ID__ == 4
		thread tr(func4, 9, 9);
		tr.join();
		// �̴߳���������
#elif __EXAMPLE_ID__ == 5
		thread tr([](int x, int y) {
			cout << x * y << endl;
		}, 9, 9);
		tr.join();
		// һ�����ͨ������������ʵ��Ϊ�������󣩽����½��߳�
#elif __EXAMPLE_ID__ == 6
		LambdaClass task{};
		thread tr{ LambdaClass(), 9, 9 };
		tr.join();
		// ������ʹ�����ţ�
		// C++�����һ���������󣬰�tr������һ��������������ʽ��
		// ���������ʹ��C11��ͳһ��ʼ���﷨���⡣
#elif __EXAMPLE_ID__ == 7
		/* ���캯��
		default (1)             thread() noexcept;
		initialization(2)       template <class Fn, class... Args>
		                        explicit thread(Fn&& fn, Args&&... args);
		copy[deleted](3)        thread(const thread&) = delete;
		move(4)                 thread(thread&& x) noexcept;
		(1)Ĭ�Ϲ��캯��
           ����һ������ʾ�κ�ִ���̵߳��̶߳���
        (2)��ʼ���Ĺ��캯�� ģ�溯��
           ����һ���̶߳��󣬸ö����ʾһ���µĿɽӺ��̡߳�
		   �µ�ִ���̵߳��� fn ���������� args ��Ϊ����(ʹ����lvalue��rvalue���õ�˥�丱��)��
		   �˹�������ɿ�ʼͬ������fn�����ġ�
        (3)��������
		   ������������
        (4)�ƶ�����
		   �����̻߳�ȡ x �̣߳������������Ӱ���ƶ��̵߳�ִ�У���ֻ�ᴫ�����Ĵ������
		   ��ɺ� x �����ٱ�ʾһ���̡߳�
		 */
		/*��������
		std::thread::~thread
		  �ƻ����̶߳������������ʱ�߳��ǿɽӺϵģ��������ֹ()��
		std::thread::operator=��
		move (1)            thread& operator= (thread&& rhs) noexcept��
		copy [deleted] (2)  thread& operator= (const thread&) = delete;
		thread����������
		�������ǰ����joinable�ģ����������rhs(����еĻ�)��ʾ��ִ���̡߳�
		�����joinable���������ֹ()����ֵ��=�������ͨ����ֵ���ʽ�����ƺ��thread��������һ���̡߳�
		 */
#elif __EXAMPLE_ID__ == 8
		vector<thread> tr_list{};
		cout << "main thread is " << this_thread::get_id() << endl;
		for (int i = 3; i < 7; ++i)
		{
			tr_list.emplace_back(thread{ func8, i });
		}
		for (auto& e : tr_list) e.join();
		// ��ʾ��λ���߳�id�����߳�����
#elif __EXAMPLE_ID__ == 9
		cout << this_thread::get_id() << " is main thread." << endl;
		thread tr1{ func8, 1 };
		thread tr2{ func8, 2 };
		thread tr3{ func8, 4 };
		thread::id id{ tr1.get_id() };
		cout << id << " thread is joinable? " << boolalpha << tr1.joinable() << endl;
		/* std::thread::joinable
		   �����̶߳����Ƿ��joinable��
		   ����̶߳����ʾִ�е��̣߳����ǿ�joinable��
		   ����Щ����£�һ���̶߳����ǲ������ӵ� :
            �����Ĭ�Ϲ��졣
			������Ѿ����ƶ�(���߹�����һ���̶߳��󣬻��߷������)��
			������ĳ�Ա�������뱻���á�
		 */
		tr1.detach();
		tr2.detach();
		tr3.detach();
		cout << id << " thread is joinable? " << boolalpha << tr1.joinable() << endl;
		func8(3);
		// detach����������̶߳�����������̣߳��������Ǳ˴˶�����ִ��;
		// �������߳����κη�ʽ�϶���������ͬ����
		// ע�⣬��һ������ִ��ʱ��������Դ���ͷš�
		// �ڵ��ô˺���֮���̶߳����ò������ӣ����԰�ȫ�����١�

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