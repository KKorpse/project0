#include "thread_pool.h"

ThreadWorkerPool::ThreadWorkerPool(int32_t iNumThreads) : WorkerPool(iNumThreads) { m_iNumThreads = iNumThreads; }

ThreadWorkerPool::~ThreadWorkerPool() {}

void ThreadWorkerPool::WorkerThread()
{
	while (true)
	{
		auto pWork = m_WorkQueue.GetWork();
		if (pWork)
		{
			pWork->Run();
		}
	}
}

void ThreadWorkerPool::Init()
{
	for (int32_t i = 0; i < m_iNumThreads; i++)
	{
		m_vecThreads.push_back(std::thread(&ThreadWorkerPool::WorkerThread, this));
	}
}

// FIXME: This function is currently not work.
void ThreadWorkerPool::Shutdown()
{
	for (int32_t i = 0; i < m_iNumThreads; i++)
	{
		m_vecThreads[i].join();
	}
}

void ThreadWorkerPool::AddWork(std::unique_ptr<Task> pWork) { m_WorkQueue.AddWork(std::move(pWork)); }