#include "process_pool.h"
#include <assert.h>

ProcessPool::ProcessPool(int32_t num_process, TaskFunctionPointer func) : m_iNumProcess(num_process), m_TaskQueue(func)
{
}

ProcessPool::~ProcessPool() {}

void ProcessPool::Init()
{
	m_TaskQueue.Init();
	for (int i = 0; i < m_iNumProcess; ++i)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			WorkerProcess();
			return;
		}
		else
		{
			m_vecPids.emplace_back(pid);
		}
	}
}

void ProcessPool::WorkerProcess()
{
	char *args = nullptr;
	TaskFunctionPointer func = nullptr;
	while (1)
	{
		m_TaskQueue.GetTask(args, func);
		if (args != nullptr)
		{
			assert(func != nullptr);
			func(args);
			// args own the memory.
			delete args;
			args = nullptr;
		}
	}
}

void ProcessPool::AddTask(const char * args, size_t size)
{
    m_TaskQueue.AddTask(args, size);
}

void ProcessPool::Shutdown()
{
	for (auto pid : m_vecPids)
	{
		kill(pid, SIGKILL);
	}
}