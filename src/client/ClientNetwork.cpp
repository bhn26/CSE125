#include "ClientNetwork.h"
#include "ConfigManager.h"

ClientNetwork::ClientNetwork(void) : m_connectSocket(INVALID_SOCKET)
{
    // create WSADATA object
    WSADATA wsaData;

    // holds address info for socket to connect to
    addrinfo *result = nullptr, *ptr = nullptr, hints;

    // Initialize Winsock
    m_result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (m_result != 0)
    {
        printf("WSAStartup failed with error: %d\n", m_result);
        exit(1);
    }

    // set address info
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP; // TCP connection!!!

    ConfigManager::Instance()->LoadConfigs("eggs.cfg");
    m_ip = ConfigManager::Instance()->GetConfigValue("m_ip");
    m_port = ConfigManager::Instance()->GetConfigValue("m_port");

    // resolve server address and m_port
    if (std::string(m_ip).size() == 0 || std::string(m_port).size() == 0)
    {
        printf("failed to read from config file, using defaults\n");
        m_result = getaddrinfo(DEFAULT_IP, DEFAULT_PORT, &hints, &result);
    }
    else
    {
        m_result = getaddrinfo(m_ip.c_str(), m_port.c_str(), &hints, &result);
    }

    if (m_result != 0)
    {
        printf("getaddrinfo failed with error: %d\n", m_result);
        WSACleanup();
        exit(1);
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        // Create a SOCKET for connecting to server
        m_connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (m_connectSocket == INVALID_SOCKET)
        {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            exit(1);
        }

        // Connect to server.
        m_result = connect(m_connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

        if (m_result == SOCKET_ERROR)
        {
            closesocket(m_connectSocket);
            m_connectSocket = INVALID_SOCKET;
            printf("The server is down... did not connect");
        }
    }

    // no longer need address info for server
    freeaddrinfo(result);

    // if connection failed
    if (m_connectSocket == INVALID_SOCKET)
    {
        printf("Unable to connect to server!\n");
        WSACleanup();
        exit(1);
    }

    // Set the mode of the socket to be nonblocking
    u_long iMode = 1;

    m_result = ioctlsocket(m_connectSocket, FIONBIO, &iMode);
    if (m_result == SOCKET_ERROR)
    {
        printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
        closesocket(m_connectSocket);
        WSACleanup();
        exit(1);
    }

    // disable nagle
    char value = 1;
    setsockopt(m_connectSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
}

ClientNetwork::~ClientNetwork(void)
{
}

int ClientNetwork::ReceivePackets(std::uint8_t* recvbuf)
{
    m_result = NetworkServices::Receive(m_connectSocket, recvbuf, g_maxPacketSize);

    if (m_result == 0)
    {
        printf("Connection closed.... %d\n", errno);
        closesocket(m_connectSocket);
        WSACleanup();
        exit(1);
    }

    return m_result;
}
