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
		std::unique_lock<std::mutex > lock(reqs_mutex_);
		m_running = true;
		//创建线程组
		for (int i = 0; i < thread_num; ++i)
		{
			m_threadgroup.push_back(std::make_shared<std::thread>(&Actor::Svc, this));
		}
		return 0;
	}

	void Stop()
	{
		std::call_once(m_flag, [this] {StopThreadGroup(); }); //保证多线程情况下只调用一次StopThreadGroup
	}

	virtual int DoOnce(Task& task) 
	{
		return 0;
	};

	virtual int DoOthers() 
	{
		return -1;
	};

private:
	void StopThreadGroup()
	{	
		std::unique_lock<std::mutex > lock(reqs_mutex_);
		m_running = false; //置为false，让内部线程跳出循环并退出

		for (auto thread : m_threadgroup) //等待线程结束
		{
			if (thread)
				thread->join();
		}
		m_threadgroup.clear();
	}

	void Svc()
	{	
		while (m_running)
		{
			//取任务分别执行
			DataRequestInfoVec_t local_reqs;
			{
				std::unique_lock<std::mutex > lock(reqs_mutex_);
				local_reqs.swap(reqs_);
				reqs_.clear();
			}

			for (auto& task : local_reqs)
			{
				if (!m_running)
					return;
				DoOnce(*task);
			}

			int res = DoOthers();

			if (local_reqs.empty() && -1 == res)
				std::this_thread::sleep_for(std::chrono::milliseconds(1));

		}

	}

private:
	typedef vector<Task *> DataRequestInfoVec_t;
	DataRequestInfoVec_t reqs_;
	std::mutex reqs_mutex_;

	std::vector<std::shared_ptr<std::thread>> m_threadgroup; //处理任务的线程组
	atomic_bool m_running = false; //是否停止的标志
	std::once_flag m_flag;
};
