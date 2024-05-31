#include "process_pool/process_pool.h"
#include <gtest/gtest.h>
void SingleTask(void *args)
{   
    // Deserialize the string and print it.
	auto data = static_cast<char *>(args);
	size_t size = static_cast<size_t>(data[0]);
	std::string out(data + sizeof(size_t), size - sizeof(size_t));
	std::cout << out << std::endl;
}

void ChiledProcess(CircleTaskQueue que)
{
	char *args = nullptr;
	TaskFunctionPointer func = nullptr;
	while (1)
	{
		auto size = que.GetTask(args, func);
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
        // Serialize the string.
		std::string str = "hello world" + std::to_string(i);
        auto total_size = str.size() + sizeof(size_t);

		char *arg = new char[str.size() + sizeof(size_t)];
        memcpy(arg, &total_size, sizeof(size_t));
        memcpy(arg + sizeof(size_t), str.c_str(), str.size());
        
		que.AddTask(arg, total_size);
		delete[] arg;
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