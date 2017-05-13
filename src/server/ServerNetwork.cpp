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
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed with error: %d\n", iResult);
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
        iResult = getaddrinfo(nullptr, DEFAULT_PORT, &hints, &result);
    }
    else
    {
        iResult = getaddrinfo(nullptr, m_port.c_str(), &hints, &result);
    }

    if (iResult != 0)
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // Set the mode of the socket to be nonblocking
    u_long iMode = 1;
    iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);

    if (iResult == SOCKET_ERROR)
    {
        printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // no longer need address information
    freeaddrinfo(result);

    // start listening for new clients attempting to connect
    iResult = listen(ListenSocket, SOMAXCONN);

    if (iResult == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }
}

ServerNetwork::~ServerNetwork(void)
{
}

// accept new connections
bool ServerNetwork::acceptNewClient(unsigned int& id)
{
    // if client waiting, accept the connection and save the socket
    ClientSocket = accept(ListenSocket, nullptr, nullptr);

    if (ClientSocket != INVALID_SOCKET)
    {
        // disable nagle on the client's socket
        char value = 1;
        setsockopt(ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

        // insert new client into session id table
        m_sessions.insert(std::pair<unsigned int, SOCKET>(id, ClientSocket));

        return true;
    }

    return false;
}

// receive incoming data
int ServerNetwork::receiveData(unsigned int client_id, std::uint8_t* recvbuf)
{
    if (m_sessions.find(client_id) != m_sessions.end())
    {
        SOCKET currentSocket = m_sessions[client_id];
        iResult = NetworkServices::receiveMessage(currentSocket, recvbuf, g_maxPacketSize);

        if (iResult == 0)
        {
            printf("Connection closed\n");
            closesocket(currentSocket);
        }

        /*if (iResult == -1) {
            int error = WSAGetLastError();
            printf("ERROR: %d\n", error);
        }*/

        return iResult;
    }

    return 0;
}

// send data to all clients
void ServerNetwork::sendToAll(std::uint8_t* packets, int totalSize)
{
    SOCKET currentSocket;
    std::map<unsigned int, SOCKET>::iterator iter;
    int iSendResult;

    for (iter = m_sessions.begin(); iter != m_sessions.end(); iter++)
    {
        currentSocket = iter->second;
        iSendResult = NetworkServices::sendMessage(currentSocket, packets, totalSize);

        if (iSendResult == SOCKET_ERROR)
        {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(currentSocket);
        }
    }
}

// send to specific client
void ServerNetwork::sendToClient(std::uint8_t* packets, int totalSize, unsigned int clientId)
{
    std::map<unsigned int, SOCKET>::iterator iter;
    int iSendResult;

    iter = m_sessions.find(clientId);
    if (iter != m_sessions.end())
    {
        SOCKET cSocket = iter->second;

        iSendResult = NetworkServices::sendMessage(cSocket, packets, totalSize);

        if (iSendResult == SOCKET_ERROR)
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