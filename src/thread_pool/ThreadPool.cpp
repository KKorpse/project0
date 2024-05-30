#include "ThreadPool.h"

ThreadPool::ThreadPool(int32_t iNumThreads) { m_iNumThreads = iNumThreads; }

ThreadPool::~ThreadPool() {}

void ThreadPool::WorkerThread()
{
	while (true)
	{
		auto pWork = m_WorkQueue.GetWork();
		if (pWork == nullptr)
		{
			break;
		}

		pWork->Run();
	}
}

void ThreadPool::Init()
{
	for (int32_t i = 0; i < m_iNumThreads; i++)
	{
		m_vecMaxThreads.push_back(std::thread(&ThreadPool::WorkerThread, this));
	}
}

// FIXME: This function is currently not work.
void ThreadPool::Shutdown()
{
	for (int32_t i = 0; i < m_iNumThreads; i++)
	{
		m_vecMaxThreads[i].join();
	}
}

void ThreadPool::AddWork(std::unique_ptr<Work> pWork)
{
    m_WorkQueue.AddWork(std::move(pWork));
}