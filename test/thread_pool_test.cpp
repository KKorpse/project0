#include "thread_pool.h"
#include <gtest/gtest.h>

// a Simple test of TheadPool
TEST(SampleTest, OutputTest)
{
	ThreadPool threadPool(4);
	threadPool.Init();

	int32_t cnt = 0;
	for (int i = 0; i < 100; i++)
	{
		std::unique_ptr<Task> pWork(new Task([i, &cnt]() { cnt++; }));
		threadPool.AddWork(std::move(pWork));
	}

    // FIXME: Now the thread pool only work in async mode.
	// EXPECT_EQ(cnt, 100);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}