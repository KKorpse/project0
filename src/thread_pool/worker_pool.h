#include "task_queue.h"
#include <functional>
#include <queue>
#include <thread>
#include <vector>

// class WorkerPool
// {
//   private:
// 	int m_iNumWorkers;

// 	WorkQueue m_WorkQueue;

// 	void WorkerThread();

//   public:
// 	ThreadPool(int iNumThreads);
// 	~ThreadPool();

// 	void Init();
// 	void Shutdown();
//   void AddWork(std::unique_ptr<Task> pWork);
// };