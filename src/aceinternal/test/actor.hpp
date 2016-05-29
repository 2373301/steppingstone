#pragma once
#include <thread>
#include <mutex>
#include <atomic>

template<typename Task>
class Actor
{
public:
	Actor() {}
	virtual ~Actor() {}
	int Init(int thread_num)
	{	
		std::unique_lock<std::mutex > lock(switch_mutex_);
		running_ = true;
		//创建线程组
		for (int i = 0; i < thread_num; ++i)
		{	
			ThreadLocalQueue *q = new ThreadLocalQueue;
			thread_que_.push_back(q);
			threads_group_.push_back(std::make_shared<std::thread>(&Actor::Svc, this, i));
		}
		return 0;
	}

	void Stop()
	{
		std::call_once(flag_, [this] {StopThreadGroup(); }); //保证多线程情况下只调用一次StopThreadGroup
	}

	void AddTask(uint64_t id, const Task& task)
	{
		uint64_t index = id  % threads_group_.size();
		ThreadLocalQueue *q = thread_que_[index];
		std::unique_lock<std::mutex > lock(q->m);
		q->vec.push_back(task);
	}

	virtual int DoOnce(Task& task) 
	{
		return 0;
	};

	virtual int DoOthers() 
	{
		return -1;
	};

	struct ThreadLocalQueue
	{
		std::mutex m;
		vector<Task *> vec;
	};

private:
	void StopThreadGroup()
	{	
		std::unique_lock<std::mutex > lock(switch_mutex_);
		running_ = false; //置为false，让内部线程跳出循环并退出

		for (auto thread : threads_group_) //等待线程结束
		{
			if (thread)
				thread->join();
		}
		threads_group_.clear();
	}

	void Svc(int i)
	{	
		while (running_)
		{
			//取任务分别执行
			vector<Task *> local_reqs;
			{
				std::unique_lock<std::mutex > lock(thread_que_[i]->m);
				local_reqs.swap(thread_que_[i]->vec);
				thread_que_[i]->vec.clear();
			}

			for (auto& task : local_reqs)
			{
				if (!running_)
					return;
				DoOnce(*task);
			}

			int res = DoOthers();

			if (local_reqs.empty() && -1 == res)
				std::this_thread::sleep_for(std::chrono::milliseconds(1));

		}

	}

private:
	std::mutex switch_mutex_;

	std::vector<ThreadLocalQueue *> thread_que_;
	std::vector<std::shared_ptr<std::thread>> threads_group_; //处理任务的线程组

	atomic_bool running_ = false; //是否停止的标志
	std::once_flag flag_;
};
