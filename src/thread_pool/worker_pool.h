#include "task_queue.h"
#include <functional>
#include <queue>
#include <thread>
#include <vector>

class WorkerPool
{
  protected:
	int m_iNumWorkers;

	WorkQueue m_WorkQueue;

	void WorkerThread();

  public:
	WorkerPool(int iNumWorkers) : m_iNumWorkers(iNumWorkers) {}
	~WorkerPool(){};

	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual void AddWork(std::unique_ptr<Work> pWork) = 0;
};