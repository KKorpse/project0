#include "fd_task_queue.h"

void FdCircleTaskQueue::Init()
{
	CircleTaskQueue::Init();
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, m_iaUnixSockt) < 0)
	{
		std::cerr << "socketpair failed: " << strerror(errno) << std::endl;
		return;
	}
}

void FdCircleTaskQueue::AddFd(int fd)
{
	if (m_pSharedMemory == nullptr)
	{
		m_pSharedMemory =
		    (SharedMemory *)mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, m_iShmFd, 0);
		if (m_pSharedMemory == MAP_FAILED)
		{
			std::cerr << "mmap failed" << std::endl;
			return;
		}
	}
	assert(m_pSharedMemory != nullptr);

	struct msghdr msg = {0};
	struct cmsghdr *cmsg;
	char buf[CMSG_SPACE(sizeof(fd))];
	memset(buf, 0, sizeof(buf));

	struct iovec io = {.iov_base = (void *)"FD", .iov_len = 2};

	msg.msg_iov = &io;
	msg.msg_iovlen = 1;
	msg.msg_control = buf;
	msg.msg_controllen = sizeof(buf);

	cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(sizeof(fd));

	*((int *)CMSG_DATA(cmsg)) = fd;

	if (sendmsg(m_iaUnixSockt[0], &msg, 0) < 0)
	{
		std::cerr << "sendmsg failed: " << strerror(errno) << std::endl;
		return;
	}

	sem_post(&m_pSharedMemory->semaphore);
}

void FdCircleTaskQueue::GetFdTask(int &fd, TaskFunctionPointer &func)
{
	if (m_pSharedMemory == nullptr)
	{
		m_pSharedMemory =
		    (SharedMemory *)mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, m_iShmFd, 0);
		if (m_pSharedMemory == MAP_FAILED)
		{
			std::cerr << "mmap failed" << std::endl;
			return;
		}
	}
	assert(m_pSharedMemory != nullptr);

	sem_wait(&m_pSharedMemory->semaphore);

	struct msghdr msg = {0};

	char m_buffer[256];
	struct iovec io = {.iov_base = m_buffer, .iov_len = sizeof(m_buffer)};

	char c_buffer[256];
	msg.msg_iov = &io;
	msg.msg_iovlen = 1;
	msg.msg_control = c_buffer;
	msg.msg_controllen = sizeof(c_buffer);

	if (recvmsg(m_iaUnixSockt[1], &msg, 0) < 0)
	{
		perror("recvmsg");
		return;
	}

	struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);

	if (cmsg == NULL || cmsg->cmsg_type != SCM_RIGHTS)
	{
		std::cerr << "No passed fd" << std::endl;
		return;
	}

	fd = *((int *)CMSG_DATA(cmsg));
	func = m_fpTaskFunctionPointer;
}
