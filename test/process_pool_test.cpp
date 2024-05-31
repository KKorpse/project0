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

TEST(SampleTest, ProcessPoolCoutTest)
{
	ProcessPool processPool(4, SingleTask);
	processPool.Init();
	for (int i = 0; i < 10; ++i)
	{
		// Serialize the string.
		std::string str = "hello world" + std::to_string(i);
		auto total_size = str.size() + sizeof(size_t);

		char *arg = new char[str.size() + sizeof(size_t)];
		memcpy(arg, &total_size, sizeof(size_t));
		memcpy(arg + sizeof(size_t), str.c_str(), str.size());

		processPool.AddTask(arg, total_size);
		delete[] arg;
	}
	sleep(2);
	processPool.Shutdown();
	ASSERT_TRUE(true);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}