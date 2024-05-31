#include "echo_server.h"
#include <memory>

EchoServer::EchoServer(int port, int pool_size, PoolType pool_type)
    : m_ThreadPool(pool_size), m_pServerSocket(port), m_PoolType(pool_type)
{
}

EchoServer::~EchoServer() {}

void EchoServer::Start()
{
	m_pServerSocket.Init();
	m_ThreadPool.Init();

	// start a new thread to listen for incoming connections
	m_tdListenThread = std::thread(
	    [this]()
	    {
		    while (true)
		    {
			    if (m_bIsStopped)
			    {
				    break;
			    }

			    // TODO: Better way?
			    int client_socket = m_pServerSocket.Accept();
			    if (client_socket < 0)
			    {
				    std::cerr << " Error when accepting client connection" << std::endl;
				    break;
			    }
			    std::cout << "Accepted connection" << std::endl;
			    std::unique_ptr<Work> pWork(new Work(
			        [client_socket]()
			        {
				        std::string sMessage = ServerSocket::ReadFrom(client_socket);
				        ServerSocket::SendTo(sMessage, client_socket);
				        close(client_socket);
			        }));
			    m_ThreadPool.AddWork(std::move(pWork));
		    }
	    });
}

void EchoServer::Stop()
{
	m_pServerSocket.~ServerSocket();
	m_ThreadPool.Shutdown();
	m_bIsStopped = true;
	m_tdListenThread.join();
}