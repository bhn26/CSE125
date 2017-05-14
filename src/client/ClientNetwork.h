#pragma once
// Networking libraries
#include <winsock2.h>
#include <Windows.h>
#include "../network/NetworkServices.h"
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include "../network/NetworkData.h"

// size of our buffer
#define DEFAULT_BUFLEN 512
// port to connect sockets through
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT "6881"
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

class ClientNetwork
{
public:
    // ctor/dtor
    ClientNetwork(void);
    ~ClientNetwork(void);

    int ReceivePackets(std::uint8_t*);

    // for error checking function calls in Winsock library
    int m_result;

    // socket for client to connect to server
    SOCKET m_connectSocket;

private:
    std::string m_ip;
    std::string m_port;
};
