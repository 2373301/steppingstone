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
		//�����߳���
		for (int i = 0; i < thread_num; ++i)
		{
			m_threadgroup.push_back(std::make_shared<std::thread>(&Actor::Svc, this));
		}
		return 0;
	}

	void Stop()
	{
		std::call_once(m_flag, [this] {StopThreadGroup(); }); //��֤���߳������ֻ����һ��StopThreadGroup
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
		m_running = false; //��Ϊfalse�����ڲ��߳�����ѭ�����˳�

		for (auto thread : m_threadgroup) //�ȴ��߳̽���
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
			//ȡ����ֱ�ִ��
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

	std::vector<std::shared_ptr<std::thread>> m_threadgroup; //����������߳���
	atomic_bool m_running = false; //�Ƿ�ֹͣ�ı�־
	std::once_flag m_flag;
};
