#include "echo_server/echo_server.h"
#include "process_pool/task_queue.h"
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>

// simple test of echoserver
int main(int argc, char *argv[])
{
	EchoServer echoServer(8080, 10, PoolType::kThread);
	echoServer.Start();
	sleep(30);
}