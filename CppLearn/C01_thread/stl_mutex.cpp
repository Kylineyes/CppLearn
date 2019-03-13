#include "stl_mutex.h"
namespace stl_mutex
{
	int _main()
	{
#if __EXAMPLE_ID__ == 0
		mutex mtx{};
		int *x = new int{ 0 };
		vector<thread> tr_list{};
		for (int i = 0; i < 4; ++i)
		{
			tr_list.emplace_back(thread{ func0, x, ref(mtx) });
		}
		for (auto& e : tr_list) e.join();
		// չ�� mutex ������ʹ��
		/* mutex ��ʼ״̬Ϊδ������
          default (1��      constexpr mutex() noexcept;
		  copy[deleted] (2) mutex(const mutex&) = delete;
		  move (3)          mutex(mutex&& x) = delete;
		 */
		/* mutex::lock()
		  ����������ǰû�б��κ��߳�������������߳�������(����һ�㿪ʼ��ֱ�����ĳ�Ա���������ã��߳�ӵ�л������)��
          �����������ǰ����һ���߳�������������̵߳�ִ�н���������ֱ�������߳̽���(�����������̼߳���ִ������)��
          �����������ǰ����ͬ���߳������������ô˺���������������(δ�������Ϊ)��
		 */
		/* mutex::try_lock()
		  ��ͼ��������������������
          ���������ǰû�б��κ��߳�������������߳���������
          ����������ǰ����һ���߳���������ú���ʧ�ܲ�����false��û��������
		  ����������ǰ����ͬ���߳������������ô˺�����������������
		 */
#elif __EXAMPLE_ID__ == 1
		recursive_mutex mtx{};
		thread tr{ func1_a, ref(mtx) };
		tr.join();
		/* recursive_mutex
		   �ݹ���������ͬһ���̶߳Ի�����������������ݹ�������������öԻ���������Ķ������Ȩ��
		   std::recursive_mutex �ͷŻ�����ʱ��Ҫ�����������������ͬ������ unlock()��
		   ����һ���̣߳����������һ�������ж������ʱ����������������������¾���Ҫ�ݹ�����
		   �����Ⲣ����ζ��Ӧ���õݹ���ȥ����ǵݹ������ݹ�����������Ȼ�򵥣�������������ĳЩ�������⡣
		   ������ú����ͱ����ú�����Ϊ�Լ��õ������������޸�ͬһ��������ʱ�ͺ����׳������⡣
		   �������ʹ�÷ǵݹ���������£�Ӧ�þ���ʹ�÷ǵݹ�������Ϊ���������˵��������ͨ�����Է��֡�
		 */
#elif __EXAMPLE_ID__ == 2
		timed_mutex mtx{};
		vector<thread> tr_list{};
		for (int i = 0; i < 4; ++i)
		{
			tr_list.emplace_back(thread{ func2, ref(mtx) });
		}
		for (auto& e : tr_list) e.join();
		/* timed_mutex
		   ��ʱ������Ҫ����������������
		   try_lock_for������ʱ��Σ���ʱ�䷶Χ��δ�����������ס�̣߳�
		     ����ڴ��ڼ������߳��ͷ�����������߳̿��Ի�öԻ����������������ʱ���򷵻� false��
		   try_lock_until��ͬ�����һ����ֻ�Ǵ������Ϊһ��δ����һ��ʱ��㡣
		 */
#elif __EXAMPLE_ID__ == 3
		mutex mtx{};
		vector<thread> tr_list{};
		for (int i = 0; i < 4; ++i)
		{
			tr_list.emplace_back(thread{ func3, ref(mtx) });
		}
		for (auto& e : tr_list) e.join();
		// lock_guard<T> ֻ��һ��ͨ�� RAII �����Ƶķ�װ����
		// �����ڹ����ʱ���Զ�������������ʱ���Զ�����
		// ������û���κη�ʽ�����������������ڡ�
#elif __EXAMPLE_ID__ == 4
		mutex mtx1{}, mtx2{};
		thread tr{ func4, ref(mtx1), ref(mtx2) };
		tr.join();
		/* unique_lock<T>
		   ��ռ����Ȩ����unique_lock������������ʱ��һ����֤������Ϊ����״̬��
		   �������Ϊ�����Զ�����ʱ��Ķ����ر����ã���Ϊ����֤���׳��쳣ʱ�����������ȷ�ؽ�����
		   ����,ע��unique_lock���󲢲����κη�ʽ�����������������ڡ�
		   default        (1) unique_lock() noexcept;
		   locking        (2) explicit unique_lock (mutex_type& m);
		   try-locking    (3) unique_lock (mutex_type& m, try_to_lock_t tag);
		   deferred       (4) unique_lock (mutex_type& m, defer_lock_t tag) noexcept;
		   adopting       (5) unique_lock (mutex_type& m, adopt_lock_t tag);
		   locking for    (6) template <class Rep, class Period>
		               unique_lock (mutex_type& m, const chrono::duration<Rep,Period>& rel_time);
		   locking until  (7) template <class Clock, class Duration>
		               unique_lock (mutex_type& m, const chrono::time_point<Clock,Duration>& abs_time);
		   copy [deleted] (8)  unique_lock (const unique_lock&) = delete;
		   move           (9)  unique_lock (unique_lock&& x);
		   �����������ֱ�������ϸ������캯����
		   (1) Ĭ�Ϲ��캯��
		      unique_lock ���󲻹����κ� Mutex ����m��
		   (2) locking ��ʼ��
		      unique_lock ������� Mutex ���� m�������� m.lock() �� Mutex �������������
			  ������� unique_lock �����Ѿ�������m�����߳̽��ᱻ������
		   (3) try-locking ��ʼ��
		      unique_lock ������� Mutex ���� m�������� m.try_lock() �� Mutex �������������
			  ������������ɹ�������������ǰ�̡߳�
		   (4) deferred ��ʼ��
		      unique_lock ������� Mutex ���� m������סm�� m ��һ��û�б���ǰ�߳���ס�� Mutex ����
		   (5) adopting ��ʼ��
		      unique_lock ������� Mutex ���� m�� m Ӧ����һ���Ѿ�����ǰ�߳���ס�� Mutex ����
			  (��ǰunique_lock ����ӵ�ж���(lock)������Ȩ)��
		   (6) locking һ��ʱ��(duration)
		      �´����� unique_lock ������� Mutex ���� m��
			  ͨ������ m.try_lock_for(rel_time) ����ס Mutex ����һ��ʱ��(rel_time)��
		   (7) locking ֱ��ĳ��ʱ���(time point)
		      �´����� unique_lock ������� Mutex ����m��
			  ͨ������ m.try_lock_until(abs_time) ����ĳ��ʱ���(abs_time)֮ǰ��ס Mutex ����
		   (8) �������� [������]
		      unique_lock �����ܱ��������졣
		   (9) �ƶ�(move)����
		      �´����� unique_lock ���������� x ������� Mutex ���������Ȩ(������ǰ Mutex ��״̬)��
			  ���� move ����֮�� x ������ͬͨ��Ĭ�Ϲ��캯���������ģ��Ͳ��ٹ����κ� Mutex �����ˡ�
		 */
		/* ��������
		   move unique_lock& operator= (unique_lock&& x) noexcept;
		      ʹ��x��mutex�����滻����ǰ�����mutex��ͬʱ��ȡx��״̬���滻����x��������mutex����
			  ��������ڵ���֮ǰ�����йܵ�mutex����ӵ��һ��������ô���Ľ�����Ա�ڱ��滻֮ǰ�ͱ������ˡ�

		   copy [deleted] unique_lock& operator= (const unique_lock&) = delete;
		      unique_lock�����ܱ����ơ�

		   std::unique_lock::swap
			  ��x�������ݣ������йܵĻ����������ǵ�ǰӵ�е�״̬��

		   std::unique_lock::release
		      ����һ��ָ���йܵĻ�������ָ�룬���ͷŶ���������Ȩ;
			  ���ú�unique_lock���ٹ���mutex����(��Ĭ�Ϲ���һ��)��
			  ע�⣬�ú��������������ͷŷ��صĻ������

		   std::unique_lock::owns_lock
		      ����unique_lock�����Ƿ�ӵ��һ������
			  unique_lock����һ��mutex����û�н��������ͷ�unique_lock֮ǰ����Ϊ�棬�����������Ϊ�٣�
			  ��unique_lock::operator bool�ı�����
		   std::unique_lock::mutex
              ����һ��ָ���йܵ�mutex�����ָ�롣
			  unique_lock���ͷŻ��������������Ȩ��
			  �����ӵ��һ��������������Ȼ�Ǹ����ͷ�����ĳһʱ��(��unique_lock�����ٵ�ʱ��)��
			  �ô���lck.mutex()->id()

		 */
#elif __EXAMPLE_ID__ == 5
		once_flag flag;
		vector<thread> tr_list{};
		for (int i = 0; i < 5; ++i)
		{
			cout << "start id " << i << " thread." << endl;
			tr_list.emplace_back(thread{ func5_a, ref(flag), func5_b, i });
		}
		for (auto& e : tr_list) e.join();
		// �������߳���Ҫͬʱ����ĳ��������call_once ���Ա�֤����̶߳Ըú���ֻ����һ�Ρ�
		// call_once ��ͨ�� once_flag
#endif
		return 0;
	}
	void func0(int *x, mutex& mtx)
	{
		for (int i = 0; i < 10; ++i)
		{
			mtx.lock();
			*x += 1;
			cout << this_thread::get_id() << " " << *x << endl;
			mtx.unlock();
		}
	}
	void func1_a(recursive_mutex& mtx)
	{
		mtx.lock();
		func1_b(ref(mtx));
		mtx.unlock();
	}
	void func1_b(recursive_mutex& mtx)
	{
		mtx.lock();
		mtx.unlock();
	}
	void func2(timed_mutex& mtx)
	{
		while (!mtx.try_lock_for(chrono::milliseconds(80)))
			cout << this_thread::get_id() << " wait\n";
		cout << this_thread::get_id() << " get lock!\n";
		this_thread::sleep_for(chrono::milliseconds(120));
		mtx.unlock();
	}
	void func3(mutex& mtx)
	{
		lock_guard<mutex> lock{ mtx };
		// ��������Զ���������Ҫʵ���ӳ���������Ҫд�ɣ�
		// lock_guard<mutex> lock{ mtx , std::adopt_lock };
		// mtx.lock();
		cout << this_thread::get_id() << " get lock!\n";
		for (int i = 0; i < 20; ++i)
		{
			cout << "test" << endl;
		}
		cout << this_thread::get_id() << " lose lock!\n";
	}
	void func4(mutex& mtx1, mutex& mtx2)
	{
		unique_lock<mutex> ulock1{ mtx1, std::try_to_lock };
		unique_lock<mutex> ulock2{ mtx2, std::defer_lock };
		cout << "done" << endl;
	}
	void func5_a(once_flag& flag, void func(int), int id)
	{
		for (int i = 0; i < 1000; ++i)
			this_thread::sleep_for(chrono::milliseconds(2));
		call_once(flag, func, id);
	}
	void func5_b(int id)
	{
		cout << id << " call this function.\n";
	}
}