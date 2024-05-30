#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class ClientSocket
{
  private:
	int m_iServerSocket = -1;
	int m_iServerPort = -1;
	bool m_bIsConnected = false;
	std::string m_strServerIP;

  public:
	ClientSocket();
	~ClientSocket();

	void ConnectTo(const std::string &strServerIP, int iServerPort);
	void Disconnect();

	void Send(const std::string &strMessage);
	std::string Receive();
};