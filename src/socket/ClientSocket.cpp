#include "ClientSocket.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

ClientSocket::ClientSocket() {}

ClientSocket::~ClientSocket() {}

void ClientSocket::ConnectTo(const std::string &strServerIP, const int iServerPort)
{
	if (m_bIsConnected)
	{
		std::cerr << "Error: Already connected to server." << std::endl;
		return;
	}

	m_iServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_iServerSocket < 0)
	{
		std::cerr << "Error: Could not create socket." << std::endl;
		return;
	}

	m_strServerIP = strServerIP;
	m_iServerPort = iServerPort;

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(iServerPort);
	if (inet_pton(AF_INET, strServerIP.c_str(), &serverAddress.sin_addr) <= 0)
	{
		std::cerr << "Invalid address/ Address not supported" << std::endl;
		return;
	}

	if (connect(m_iServerSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cerr << "Error: Could not connect to server." << std::endl;
		return;
	}

	m_bIsConnected = true;
}

void ClientSocket::Disconnect()
{
	if (m_bIsConnected)
	{
		close(m_iServerSocket);
		m_bIsConnected = false;
	}
	else
	{
		std::cerr << "Warning: Try to close a socket that is not connected." << std::endl;
	}
}

void ClientSocket::Send(const std::string &strMessage)
{
	auto res = send(m_iServerSocket, strMessage.c_str(), strMessage.size(), 0);
	if (res < 0)
	{
		std::cerr << "Error: Could not send message." << std::endl;
	}
}

std::string ClientSocket::Receive()
{
	char buffer[1024] = {0};
	int valRead = read(m_iServerSocket, buffer, 1024);
	return std::string(buffer);
}