#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
typedef std::function<void(char *vArgs)> TaskFunctionPointer;

/*
 * Should init the CircleTaskQueue before fork() the child process.
 */
class CircleTaskQueue
{
  private:
	static const size_t kDEFAULT_TASK_SIZE = 16;
	static const size_t kDEFAULT_SHARED_MEMORY_SIZE = 4096;

	struct SharedMemory
	{
		sem_t semaphore;
		int head;
		int tail;
		/*
		 * Data encoding format: {size, data}.
		 */
		char message[kDEFAULT_SHARED_MEMORY_SIZE];
	};

	int m_iShmFd = -1;

	SharedMemory *m_pSharedMemory = nullptr;
	TaskFunctionPointer m_fpTaskFunctionPointer = nullptr;

  private:
	void AddDataToQueue(const char *data, size_t size);
	void GetDataFromQueue(char *&data);

  public:
	CircleTaskQueue(TaskFunctionPointer);
	~CircleTaskQueue();

	/* Called by main process*/
	void Init();

	void AddTask(const char *args, const size_t size);
	void GetTask(char *&args, TaskFunctionPointer &func);
};