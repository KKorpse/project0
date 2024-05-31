#include "task_queue.h"
#include <assert.h>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <vector>

class FdCircleTaskQueue : public CircleTaskQueue
{
  private:
	/*
	 * [0] for parent process, [1] for chiled process.
	 */
	int m_iaUnixSockt[2]{-1, -1};

  public:
	FdCircleTaskQueue(TaskFunctionPointer func) : CircleTaskQueue(func) {}
	~FdCircleTaskQueue() = default;

	/*
	 * Init a pair of Unix socket and number of chiled process.
	 */
	void Init();

	/*
	 * Only called by parent process.
	 */
	void AddFd(int fd);

	int GetFd(TaskFunctionPointer &func);
};