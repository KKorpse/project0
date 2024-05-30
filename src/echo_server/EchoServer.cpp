#include "EchoServer.h"
#include <memory>

EchoServer::EchoServer(int iPort, int iThreadPoolSize)
    : m_iPort(iPort), m_ThreadPool(iThreadPoolSize), m_pServerSocket(iPort)
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
			    int iClientSocket = m_pServerSocket.Accept();
			    if (iClientSocket < 0)
			    {
				    std::cerr << " Error when accepting client connection" << std::endl;
				    break;
			    }
			    std::cout << "Accepted connection" << std::endl;
			    std::unique_ptr<Work> pWork(new Work(
			        [iClientSocket]()
			        {
				        std::string sMessage = ServerSocket::ReadFrom(iClientSocket);
				        ServerSocket::SendTo(sMessage, iClientSocket);
				        // FIXME: Should I close the socket here?
				        close(iClientSocket);
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