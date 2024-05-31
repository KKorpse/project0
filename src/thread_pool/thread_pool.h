#include "worker_pool.h"
#include <functional>
#include <queue>
#include <thread>
#include <vector>

class ThreadWorkerPool : public WorkerPool
{
  private:
	int32_t m_iNumThreads;

	std::vector<std::thread> m_vecThreads;

	void WorkerThread();

  public:
	ThreadWorkerPool(int32_t iNumThreads);
	~ThreadWorkerPool();

	void Init() override;
	void Shutdown() override;
	void AddWork(std::unique_ptr<Work> pWork) override;
};