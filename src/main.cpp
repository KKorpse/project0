#include "socket/ClientSocket.h"
#include "socket/ServerSocket.h"
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <unistd.h>

// simple test of echoserver
int main(int argc, char *argv[])
{
	// EchoServer echoServer(6666);
	// echoServer.Start();
	// std::cout << "EchoServer started" << std::endl;
	// sleep(30);
	// echoServer.Stop();

	ServerSocket server(6666);
	ClientSocket client;
	server.Init();

	client.ConnectTo("127.0.0.1", 6666);
	client.Send("Hello");
	auto socket = server.Accept();
	auto recivedMessage = server.ReadFrom(socket);


	return 0;
}