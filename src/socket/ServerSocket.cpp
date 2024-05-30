#include "ServerSocket.h"

#define kDEFAULT_BUFFER_SIZE 1024

ServerSocket::ServerSocket(int iPort)
{
	m_iPort = iPort;
	m_bIsListening = false;
}

ServerSocket::~ServerSocket() { close(m_iSocket); }

void ServerSocket::Init()
{
	m_iSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_iSocket == -1)
	{
		std::cerr << "Error creating socket" << std::endl;
		exit(1);
	}

	m_addrAdrees.sin_family = AF_INET;
	m_addrAdrees.sin_addr.s_addr = INADDR_ANY;
	m_addrAdrees.sin_port = htons(m_iPort);

	if (bind(m_iSocket, (struct sockaddr *)&m_addrAdrees, sizeof(m_addrAdrees)) == -1)
	{
		std::cerr << "Error binding socket" << std::endl;
		exit(1);
	}

	if (listen(m_iSocket, 10) == -1)
	{
		std::cerr << "Error listening on socket" << std::endl;
		exit(1);
	}

	m_bIsListening = true;
}

int ServerSocket::Accept()
{
	// TODO: Stronger Error handlling.
	if (!m_bIsListening)
	{
		std::cerr << "Socket is not listening" << std::endl;
		return 0;
	}

	struct sockaddr_in clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);
	int clientSocket = accept(m_iSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
	if (clientSocket == -1)
	{
		std::cerr << "Error accepting connection" << std::endl;
		return 0;
	}

	return clientSocket;
}

std::string ServerSocket::ReadFrom(const int iClientSocket)
{
	char buffer[kDEFAULT_BUFFER_SIZE];
	int bytesRead = read(iClientSocket, buffer, kDEFAULT_BUFFER_SIZE);
	if (bytesRead == -1)
	{
		std::cerr << "Error reading from socket" << std::endl;
		return "";
	}

	return std::string(buffer, bytesRead);
}

void ServerSocket::SendTo(const std::string sMessage, const int iClientSocket)
{
	auto wrise = write(iClientSocket, sMessage.c_str(), sMessage.size());
	if (wrise == -1)
	{
		std::cerr << "Error writing to socket" << std::endl;
	}
}