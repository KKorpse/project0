#include "echo_server.h"
#include <memory>
#include <sstream>

const std::string kExitMessage = "exit\r\n";

EchoServer::EchoServer(int port, int pool_size, PoolType pool_type)
    : m_ThreadPool(pool_size), m_ProcessPool(pool_size, ProcessWork, ProcessPoolTaskType::kTaskTypeFd),
      m_pServerSocket(port), m_PoolType(pool_type)
{
}

EchoServer::~EchoServer() {}

void EchoServer::ProcessWork(const char *args)
{
	int client_socket = *(int *)args;
	ThreadWork(client_socket);
}

void EchoServer::ThreadWork(int client_socket)
{
	std::string message = ServerSocket::ReadFrom(client_socket);
	while (!message.empty())
	{
		ServerSocket::SendTo(message, client_socket);
		if (message == kExitMessage)
		{
			close(client_socket);
			return;
		}
		message = ServerSocket::ReadFrom(client_socket);
	}
}

void EchoServer::StartThreadPool()
{
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
			    std::unique_ptr<Work> pWork(new Work(std::bind(ThreadWork, client_socket)));
			    m_ThreadPool.AddWork(std::move(pWork));
		    }
	    });
}

void EchoServer::StartProcessPool()
{
	m_ProcessPool.Init();
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
			    m_ProcessPool.AddTask(client_socket);
		    }
	    });
}

void EchoServer::Start()
{
	m_pServerSocket.Init();

	if (m_PoolType == PoolType::kThread)
	{
		StartThreadPool();
	}
	else
	{
		StartProcessPool();
	}
}

void EchoServer::Stop()
{
	m_pServerSocket.~ServerSocket();
	if (m_PoolType == PoolType::kThread)
	{
		m_ThreadPool.Shutdown();
	}
	else
	{
		m_ProcessPool.Shutdown();
	}

	m_bIsStopped = true;
	m_tdListenThread.join();
}