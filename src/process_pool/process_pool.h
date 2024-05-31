#include "fd_task_queue.h"

#define kDEFAULT_NUM_PROCESS 4

enum class ProcessPoolTaskType
{
	kTaskTypeFd,
	kTaskTypeArgs
};

class ProcessPool
{
  private:
	int32_t m_iNumProcess;

	FdCircleTaskQueue m_TaskQueue;

	std::vector<pid_t> m_vecPids;

	ProcessPoolTaskType m_TaskType = ProcessPoolTaskType::kTaskTypeArgs;

	void WorkerProcess();

  public:
	ProcessPool(int32_t num_process, TaskFunctionPointer func, ProcessPoolTaskType type = ProcessPoolTaskType::kTaskTypeArgs);
	~ProcessPool();

	void Init();
	void Shutdown();
	void AddTask(const char *args, size_t size);
	void AddTask(const int fd);
};