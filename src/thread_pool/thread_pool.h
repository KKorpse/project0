#include "task_queue.h"
#include <functional>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool
{
  private:
	int32_t m_iNumThreads;

	std::vector<std::thread> m_vecThreads;

	WorkQueue m_WorkQueue;

	void WorkerThread();

  public:
	ThreadPool(int32_t iNumThreads);
	~ThreadPool();

	void Init();
	void Shutdown();
  void AddWork(std::unique_ptr<Task> pWork);
};