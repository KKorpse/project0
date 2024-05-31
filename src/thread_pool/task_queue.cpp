#include "task_queue.h"
#include <assert.h>
#include <iostream>

#define DEFAULT_SHARED_MEMORY_NAME "NAME"

CircleTaskQueue::CircleTaskQueue(TaskFunctionPointer func)
{
	assert(func != nullptr);
	m_fpTaskFunctionPointer = func;
}

CircleTaskQueue::~CircleTaskQueue()
{
	if (m_pSharedMemory != nullptr)
	{
		munmap(m_pSharedMemory, sizeof(SharedMemory));
	}
	close(m_iShmFd);
	shm_unlink(DEFAULT_SHARED_MEMORY_NAME);
}

/* Init shared memory */
void CircleTaskQueue::Init()
{
	m_iShmFd = shm_open(DEFAULT_SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
	if (m_iShmFd == -1)
	{
		perror("shm_open");
		std::cerr << "shm_open failed" << std::endl;
		exit(1);
	}

	if (ftruncate(m_iShmFd, sizeof(SharedMemory)) == -1)
	{
		perror("ftruncate");
		std::cerr << "ftruncate failed" << std::endl;
		exit(1);
	}

	m_pSharedMemory = (SharedMemory *)mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, m_iShmFd, 0);
	sem_init(&m_pSharedMemory->semaphore, 1, 1);

	/*
	 * Release the shared memory for the first time.
	 * This class will send to the child process by copy, so the pointer is not work in the child process.
	 * The chiled process will call mmap() to get the shared memory.
	 */
	munmap(m_pSharedMemory, sizeof(SharedMemory));
	m_pSharedMemory = nullptr;
}

void CircleTaskQueue::AddDataToQueue(const char *data, size_t size)
{
	if (m_pSharedMemory == nullptr)
	{
		m_pSharedMemory =
		    (SharedMemory *)mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, m_iShmFd, 0);
		if (m_pSharedMemory == MAP_FAILED)
		{
			perror("mmap");
			std::cerr << "mmap failed" << std::endl;
			exit(1);
		}
	}

	assert(m_pSharedMemory != nullptr);

	sem_wait(&m_pSharedMemory->semaphore);
	if (m_pSharedMemory->tail + size + sizeof(size_t) > kDEFAULT_SHARED_MEMORY_SIZE &&
	    (m_pSharedMemory->tail + size + sizeof(size_t)) % kDEFAULT_SHARED_MEMORY_SIZE > m_pSharedMemory->head)
	{
		std::cerr << "Queue is full"
		          << "head: " << m_pSharedMemory->head << ", tail: " << m_pSharedMemory->tail << std::endl;
		sem_post(&m_pSharedMemory->semaphore);
		return;
	}

	if (m_pSharedMemory->tail + sizeof(size_t) + size < kDEFAULT_SHARED_MEMORY_SIZE)
	{
		memcpy(m_pSharedMemory->message + m_pSharedMemory->tail, &size, sizeof(size_t));
		memcpy(m_pSharedMemory->message + m_pSharedMemory->tail + sizeof(size_t), data, size);
		m_pSharedMemory->tail += sizeof(size_t) + size;
	}
	else
	{
		size_t first_size = kDEFAULT_SHARED_MEMORY_SIZE - m_pSharedMemory->tail - sizeof(size_t);
		size_t second_size = size - first_size;
		// FIXME: What if the rest of the memory is not enough for sizeof(size_t) bytes?
		memcpy(m_pSharedMemory->message + m_pSharedMemory->tail, &first_size, sizeof(size_t));
		memcpy(m_pSharedMemory->message + m_pSharedMemory->tail + sizeof(size_t), data, first_size);
		assert(m_pSharedMemory->message + second_size < m_pSharedMemory->message + m_pSharedMemory->head);
		memcpy(m_pSharedMemory->message, data + first_size, second_size);
		m_pSharedMemory->tail = second_size;
	}

	sem_post(&m_pSharedMemory->semaphore);

	std::cout << "Data added: " << std::string(data, size) << std::endl;
}

void CircleTaskQueue::GetDataFromQueue(char *&data)
{
	if (m_pSharedMemory == nullptr)
	{
		m_pSharedMemory =
		    (SharedMemory *)mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, m_iShmFd, 0);
		if (m_pSharedMemory == MAP_FAILED)
		{
			perror("mmap");
			std::cerr << "mmap failed" << std::endl;
			data = nullptr;
			return;
		}
	}

	assert(m_pSharedMemory != nullptr);

	sem_wait(&m_pSharedMemory->semaphore);
	if (m_pSharedMemory->head == m_pSharedMemory->tail)
	{
		std::cout << "Queue is empty" << std::endl;
		sem_post(&m_pSharedMemory->semaphore);
		data = nullptr;
		return;
	}

	size_t data_size;
	memcpy(&data_size, m_pSharedMemory->message + m_pSharedMemory->head, sizeof(size_t));
	if (m_pSharedMemory->head + sizeof(size_t) + data_size < kDEFAULT_SHARED_MEMORY_SIZE)
	{
		memcpy(data, m_pSharedMemory->message + m_pSharedMemory->head + sizeof(size_t), data_size);
		m_pSharedMemory->head += sizeof(size_t) + data_size;
	}
	else
	{
		size_t first_size = kDEFAULT_SHARED_MEMORY_SIZE - m_pSharedMemory->head - sizeof(size_t);
		size_t second_size = data_size - first_size;
		memcpy(data, m_pSharedMemory->message + m_pSharedMemory->head + sizeof(size_t), first_size);
		assert(m_pSharedMemory->message + second_size < m_pSharedMemory->message + m_pSharedMemory->tail);
		memcpy(data + first_size, m_pSharedMemory->message, second_size);
		m_pSharedMemory->head = second_size;
	}
	sem_post(&m_pSharedMemory->semaphore);
	std::cout << "Get queue.";
}

void CircleTaskQueue::AddTask(const char *args, const size_t size) { AddDataToQueue(args, size); }
void CircleTaskQueue::GetTask(char *&args, TaskFunctionPointer &func)
{
	GetDataFromQueue(args);
	func = m_fpTaskFunctionPointer;
}