#include "echo_server/echo_server.h"
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>

// simple test of echoserver
int main(int argc, char *argv[])
{	
	// TODO: Use argument to determine the pool type
	EchoServer echoServer(9999, 10, PoolType::kThread);
	echoServer.Start();
	sleep(30);
}