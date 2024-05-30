#include "WorkQueue.h"
#include <mutex>

WorkQueue::WorkQueue() {}

WorkQueue::~WorkQueue() {}

void WorkQueue::AddWork(std::unique_ptr<Work> pWork)
{
	std::unique_lock<std::mutex> lock(m_mtxQueue);
	m_queWorks.emplace(std::move(pWork));
}

std::unique_ptr<Work> WorkQueue::GetWork()
{
	std::unique_lock<std::mutex> lock(m_mtxQueue);
	if (m_queWorks.empty())
	{
		return nullptr;
	}

	auto pWork = std::move(m_queWorks.front());
	m_queWorks.pop();
	return pWork;
}