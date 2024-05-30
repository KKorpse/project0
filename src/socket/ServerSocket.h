#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class ServerSocket
{
  private:
	int m_iSocket = -1;
	int m_iPort = -1;
	struct sockaddr_in m_addrAdrees;
	bool m_bIsListening = false;

  public:
	ServerSocket(int iPort);
	~ServerSocket();

	/*
	 * Bind and Listen the port.
	 */
	void Init();

	int Accept();

	static std::string ReadFrom(const int iClientSocket);

	static void SendTo(const std::string sMessage, const int iClientSocket);
};