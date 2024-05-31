#include "socket/server_socket.h"
#include "thread_pool/thread_pool.h"
#include <memory>

enum class PoolType
{
	kThread,
	kProcess
};

class EchoServer
{
  private:
	ThreadWorkerPool m_ThreadPool;
	ServerSocket m_pServerSocket;
	std::thread m_tdListenThread;
	bool m_bIsStopped = false;
	PoolType m_PoolType = PoolType::kThread;

	static const int s_iDefaultPoolSize = 10;

  public:
	EchoServer(int iPort, int iThreadPoolSize = s_iDefaultPoolSize, PoolType poolType = PoolType::kThread);
	~EchoServer();

	void Start();
	void Stop();

	void Listen();
};