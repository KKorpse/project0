#include "task_queue.h"

#define kDEFAULT_NUM_PROCESS 4

class ProcessPool
{
  private:
	int32_t m_iNumProcess;

	CircleTaskQueue m_TaskQueue;

	std::vector<pid_t> m_vecPids;

	void WorkerProcess();

  public:
	ProcessPool(int32_t num_process, TaskFunctionPointer func);
	~ProcessPool();

	void Init();
	void Shutdown();
	void AddTask(const char *args, size_t size);
};