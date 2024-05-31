#include "process_pool/process_pool.h"
#include <gtest/gtest.h>
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

TEST(SampleTest, OutputTest)
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
			return;
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

	sleep(2);
	for (auto pid : pids)
	{
		kill(pid, SIGKILL);
	}

    ASSERT_TRUE(true);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}