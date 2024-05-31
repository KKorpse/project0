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
  protected:
	static const size_t kDEFAULT_TASK_SIZE = 16;
	static const size_t kDEFAULT_SHARED_MEMORY_SIZE = 4096;

	struct SharedMemory
	{
		sem_t semaphore;
		int head = 0;
		int tail = 0;
		/*
		 * Data encoding format: {size, data}.
		 */
		char message[kDEFAULT_SHARED_MEMORY_SIZE];
	};

	int m_iShmFd = -1;

	SharedMemory *m_pSharedMemory = nullptr;
	TaskFunctionPointer m_fpTaskFunctionPointer = nullptr;

  protected:
	void AddDataToQueue(const char *data, size_t size);
	size_t GetDataFromQueue(char *&data);

  public:
	CircleTaskQueue(TaskFunctionPointer);
	~CircleTaskQueue();

	/*
	 * @brief Called by main process
	 */
	void Init();

	void AddTask(const char *args, const size_t size);

	/*
	 *  @return the size of args. 0 if failed.
	 */
	size_t GetTask(char *&args, TaskFunctionPointer &func);
};