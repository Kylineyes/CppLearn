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
		// 展现 mutex 的入门使用
		/* mutex 初始状态为未锁定。
          default (1）      constexpr mutex() noexcept;
		  copy[deleted] (2) mutex(const mutex&) = delete;
		  move (3)          mutex(mutex&& x) = delete;
		 */
		/* mutex::lock()
		  如果互斥对象当前没有被任何线程锁定，则调用线程锁定它(从这一点开始，直到它的成员解锁被调用，线程拥有互斥对象)。
          如果互斥锁当前被另一个线程锁定，则调用线程的执行将被阻塞，直到其他线程解锁(其他非锁定线程继续执行它们)。
          如果互斥锁当前被相同的线程所锁定，调用此函数，则会产生死锁(未定义的行为)。
		 */
		/* mutex::try_lock()
		  试图锁定互斥锁，不阻塞。
          若互斥对象当前没有被任何线程锁定，则调用线程锁定它；
          若互斥锁当前被另一个线程锁定，则该函数失败并返回false，没有阻塞；
		  若互斥锁当前被相同的线程所锁定，调用此函数，则会产生死锁。
		 */
#elif __EXAMPLE_ID__ == 1
		recursive_mutex mtx{};
		thread tr{ func1_a, ref(mtx) };
		tr.join();
		/* recursive_mutex
		   递归锁，允许同一个线程对互斥量多次上锁（即递归上锁），来获得对互斥量对象的多层所有权，
		   std::recursive_mutex 释放互斥量时需要调用与该锁层次深度相同次数的 unlock()。
		   即当一个线程，多个函数对一个锁进行多次上锁时，会立即死锁，这种情况下就需要递归锁。
		   但是这并不意味着应该用递归锁去代替非递归锁。递归锁用起来固然简单，但往往会隐藏某些代码问题。
		   比如调用函数和被调用函数以为自己拿到了锁，都在修改同一个对象，这时就很容易出现问题。
		   因此在能使用非递归锁的情况下，应该尽量使用非递归锁，因为死锁相对来说，更容易通过调试发现。
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
		   定时锁，主要多了这两个函数：
		   try_lock_for：传入时间段，在时间范围内未获得所就阻塞住线程，
		     如果在此期间其他线程释放了锁，则该线程可以获得对互斥量的锁，如果超时，则返回 false。
		   try_lock_until：同上面的一样，只是传入参数为一个未来的一个时间点。
		 */
#elif __EXAMPLE_ID__ == 3
		mutex mtx{};
		vector<thread> tr_list{};
		for (int i = 0; i < 4; ++i)
		{
			tr_list.emplace_back(thread{ func3, ref(mtx) });
		}
		for (auto& e : tr_list) e.join();
		// lock_guard<T> 只是一种通过 RAII 风格机制的封装器，
		// 当其在构造的时候，自动上锁，析构的时候自动解锁
		// 它本身没有任何方式管理互斥对象的生命周期。
#elif __EXAMPLE_ID__ == 4
		mutex mtx1{}, mtx2{};
		thread tr{ func4, ref(mtx1), ref(mtx2) };
		tr.join();
		/* unique_lock<T>
		   独占所有权锁，unique_lock对象在析构的时候一定保证互斥量为解锁状态；
		   因此它作为具有自动持续时间的对象特别有用，因为它保证在抛出异常时，互斥对象被正确地解锁。
		   不过,注意unique_lock对象并不以任何方式管理互斥对象的生命周期。
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
		   下面我们来分别介绍以上各个构造函数：
		   (1) 默认构造函数
		      unique_lock 对象不管理任何 Mutex 对象m。
		   (2) locking 初始化
		      unique_lock 对象管理 Mutex 对象 m，并调用 m.lock() 对 Mutex 对象进行上锁，
			  如果其他 unique_lock 对象已经管理了m，该线程将会被阻塞。
		   (3) try-locking 初始化
		      unique_lock 对象管理 Mutex 对象 m，并调用 m.try_lock() 对 Mutex 对象进行上锁，
			  但如果上锁不成功，不会阻塞当前线程。
		   (4) deferred 初始化
		      unique_lock 对象管理 Mutex 对象 m并不锁住m。 m 是一个没有被当前线程锁住的 Mutex 对象。
		   (5) adopting 初始化
		      unique_lock 对象管理 Mutex 对象 m， m 应该是一个已经被当前线程锁住的 Mutex 对象。
			  (当前unique_lock 对象拥有对锁(lock)的所有权)。
		   (6) locking 一段时间(duration)
		      新创建的 unique_lock 对象管理 Mutex 对象 m，
			  通过调用 m.try_lock_for(rel_time) 来锁住 Mutex 对象一段时间(rel_time)。
		   (7) locking 直到某个时间点(time point)
		      新创建的 unique_lock 对象管理 Mutex 对象m，
			  通过调用 m.try_lock_until(abs_time) 来在某个时间点(abs_time)之前锁住 Mutex 对象。
		   (8) 拷贝构造 [被禁用]
		      unique_lock 对象不能被拷贝构造。
		   (9) 移动(move)构造
		      新创建的 unique_lock 对象获得了由 x 所管理的 Mutex 对象的所有权(包括当前 Mutex 的状态)。
			  调用 move 构造之后， x 对象如同通过默认构造函数所创建的，就不再管理任何 Mutex 对象了。
		 */
		/* 其他函数
		   move unique_lock& operator= (unique_lock&& x) noexcept;
		      使用x的mutex对象替换掉当前对象的mutex，同时获取x的状态。替换掉的x将不再有mutex对象，
			  如果对象在调用之前对其托管的mutex对象拥有一个锁，那么它的解锁成员在被替换之前就被调用了。

		   copy [deleted] unique_lock& operator= (const unique_lock&) = delete;
		      unique_lock对象不能被复制。

		   std::unique_lock::swap
			  与x交换内容，包括托管的互斥对象和它们当前拥有的状态。

		   std::unique_lock::release
		      返回一个指向托管的互斥对象的指针，并释放对它的所有权;
			  调用后unique_lock不再管理mutex对象(像默认构造一样)；
			  注意，该函数不会锁定或释放返回的互斥对象。

		   std::unique_lock::owns_lock
		      返回unique_lock对象是否拥有一个锁。
			  unique_lock锁定一个mutex，在没有解锁或者释放unique_lock之前返回为真，其他情况返回为假；
			  是unique_lock::operator bool的别名。
		   std::unique_lock::mutex
              返回一个指向托管的mutex对象的指针。
			  unique_lock不释放互斥对象管理的所有权。
			  如果它拥有一个互斥锁，它仍然是负责释放它在某一时刻(像当unique_lock被销毁的时候)。
			  用处：lck.mutex()->id()

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
		// 如果多个线程需要同时调用某个函数，call_once 可以保证多个线程对该函数只调用一次。
		// call_once 是通过 once_flag
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
		// 如果不想自动上锁，想要实现延迟上锁，就要写成：
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