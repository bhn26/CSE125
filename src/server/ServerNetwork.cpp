#include "ServerNetwork.h"
#include "ConfigManager.h"

ServerNetwork::ServerNetwork(void)
{
    // create WSADATA object
    WSADATA wsaData;

    // address info for the server to listen to
    addrinfo* result = nullptr;
    addrinfo hints;

    // Initialize Winsock
    m_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (m_result != 0)
    {
        printf("WSAStartup failed with error: %d\n", m_result);
        exit(1);
    }

    // set address information
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP; // TCP connection!!!
    hints.ai_flags = AI_PASSIVE;

    ConfigManager::Instance()->LoadConfigs("../eggs.cfg");
    m_port = ConfigManager::Instance()->GetConfigValue("m_port");

    // resolve server address and m_port
    if (m_port.empty())
    {
        m_result = getaddrinfo(nullptr, DEFAULT_PORT, &hints, &result);
    }
    else
    {
        m_result = getaddrinfo(nullptr, m_port.c_str(), &hints, &result);
    }

    if (m_result != 0)
    {
        printf("getaddrinfo failed with error: %d\n", m_result);
        WSACleanup();
        exit(1);
    }

    // Create a SOCKET for connecting to server
    m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (m_listenSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // Set the mode of the socket to be nonblocking
    u_long iMode = 1;
    m_result = ioctlsocket(m_listenSocket, FIONBIO, &iMode);

    if (m_result == SOCKET_ERROR)
    {
        printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
        closesocket(m_listenSocket);
        WSACleanup();
        exit(1);
    }

    // Setup the TCP listening socket
    m_result = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (m_result == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(m_listenSocket);
        WSACleanup();
        exit(1);
    }

    // no longer need address information
    freeaddrinfo(result);

    // start listening for new clients attempting to connect
    m_result = listen(m_listenSocket, SOMAXCONN);

    if (m_result == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(m_listenSocket);
        WSACleanup();
        exit(1);
    }
}

ServerNetwork::~ServerNetwork(void)
{
}

// accept new connections
bool ServerNetwork::AcceptNewClient(unsigned int& id)
{
    // if client waiting, accept the connection and save the socket
    m_clientSocket = accept(m_listenSocket, nullptr, nullptr);

    if (m_clientSocket != INVALID_SOCKET)
    {
        // disable nagle on the client's socket
        char value = 1;
        setsockopt(m_clientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

        // insert new client into session id table
        m_sessions.insert(std::pair<unsigned int, SOCKET>(id, m_clientSocket));

        return true;
    }

    return false;
}

// receive incoming data
int ServerNetwork::ReceiveData(unsigned int client_id, std::uint8_t* recvbuf)
{
    if (m_sessions.find(client_id) != m_sessions.end())
    {
        SOCKET currentSocket = m_sessions[client_id];
        m_result = NetworkServices::Receive(currentSocket, recvbuf, g_maxPacketSize);

        if (m_result == 0)
        {
            printf("Connection closed\n");
            closesocket(currentSocket);
        }

        /*if (m_result == -1) {
            int error = WSAGetLastError();
            printf("ERROR: %d\n", error);
        }*/

        return m_result;
    }

    return 0;
}

// send data to all clients
void ServerNetwork::SendToAll(std::uint8_t* packets, int totalSize)
{
    SOCKET currentSocket;
    std::map<unsigned int, SOCKET>::iterator iter;
    int iSenm_result;

    for (iter = m_sessions.begin(); iter != m_sessions.end(); iter++)
    {
        currentSocket = iter->second;
        iSenm_result = NetworkServices::Send(currentSocket, packets, totalSize);

        if (iSenm_result == SOCKET_ERROR)
        {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(currentSocket);
        }
    }
}

// send to specific client
void ServerNetwork::SendToClient(std::uint8_t* packets, int totalSize, unsigned int clientId)
{
    std::map<unsigned int, SOCKET>::iterator iter;
    int iSenm_result;

    iter = m_sessions.find(clientId);
    if (iter != m_sessions.end())
    {
        SOCKET cSocket = iter->second;

        iSenm_result = NetworkServices::Send(cSocket, packets, totalSize);

        if (iSenm_result == SOCKET_ERROR)
        {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(cSocket);
        }
    }
    else
    {
        printf("no client with id: %d\n", clientId);
    }
}