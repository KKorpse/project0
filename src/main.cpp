#include "echo_server/echo_server.h"
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <unistd.h>

// simple test of echoserver
int main(int argc, char *argv[])
{
	EchoServer echoServer(6666);
	echoServer.Start();
	std::cout << "EchoServer started" << std::endl;
	sleep(30);
	echoServer.Stop();

	return 0;
}