#include "echo_server/echo_server.h"
#include "thread_pool/task_queue.h"
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>

void SingleTask(void *args)
{
	std::string out(static_cast<char *>(args));
	std::cout << out << std::endl;
}

void ChiledProcess(CircleTaskQueue que)
{
	char *args = nullptr;
	TaskFunctionPointer func = nullptr;
	while (1)
	{
		que.GetTask(args, func);
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

// simple test of echoserver
int main(int argc, char *argv[])
{
	CircleTaskQueue que(SingleTask);
	que.Init();
	std::vector<int> pids;
	for (int i = 0; i < 4; ++i)
	{
		auto pid = fork();
		if (pid == 0)
		{
			ChiledProcess(que);
			return 0;
		}
		else
		{
			pids.emplace_back(pid);
		}
	}
	for (int i = 0; i < 10; ++i)
	{
		std::string arg = "hello world" + std::to_string(i);
		que.AddTask(arg.c_str(), arg.size());
	}

	sleep(10);
	for (auto pid : pids)
	{
		kill(pid, SIGKILL);
	}
}