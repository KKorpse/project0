#include "process_pool/process_pool.h"
#include <gtest/gtest.h>

TEST(SampleTest, ProcessPoolCoutTest)
{
	ProcessPool processPool(4,
	                        [](void *args)
	                        {
		                        std::string out(static_cast<char *>(args));
		                        std::cout << out << std::endl;
	                        });
	processPool.Init();
	for (int i = 0; i < 10; ++i)
	{
		std::string arg = "hello world" + std::to_string(i);
		processPool.AddTask(arg.c_str(), arg.size());
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