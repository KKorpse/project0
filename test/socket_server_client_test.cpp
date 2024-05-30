#include "socket/client_socket.h"
#include "socket/server_socket.h"

#include <gtest/gtest.h>

TEST(SocketServerClientTest, ClientAndServerShouldComunicates)
{
	ServerSocket server(6666);
	ClientSocket client;
	server.Init();

	client.ConnectTo("127.0.0.1", 6666);
	std::string sendMessage = "Hello";
	client.Send(sendMessage);
	auto socket = server.Accept();
	auto recivedMessage = server.ReadFrom(socket);
	EXPECT_EQ(recivedMessage, sendMessage);
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}