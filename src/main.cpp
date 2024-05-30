#include <gtest/gtest.h>
#include "thread_pool/ThreadPool.h"
#include <iostream>
#include <memory>


// simple test of thread pool
int main(int argc, char *argv[]) {
    ThreadPool threadPool(4);
    threadPool.Init();

    for (int i = 0; i < 100; i++) {
        std::unique_ptr<Work> pWork(new Work([i]() {
            std::cout << "Task " << i << " is running" << std::endl;
        }));
        threadPool.AddWork(std::move(pWork));
    }

    threadPool.Shutdown();
}