#include "socket/server_socket.h"
#include "thread_pool/thread_pool.h"
#include <memory>

class EchoServer
{
  private:
	int m_iPort = 0;
	ThreadPool m_ThreadPool;
	ServerSocket m_pServerSocket;
	std::thread m_tdListenThread;
	bool m_bIsStopped = false;

	static const int s_iDefaultThreadPoolSize = 10;

  public:
	EchoServer(int iPort, int iThreadPoolSize = s_iDefaultThreadPoolSize);
	~EchoServer();

	void Start();
	void Stop();

	void Listen();
};