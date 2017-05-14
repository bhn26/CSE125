#pragma once

#include "network/NetworkData.h"
#include "network/NetworkServices.h"

#include <map>
#include <string>

#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6881"

class ServerNetwork
{
public:
    ServerNetwork(void);
    ~ServerNetwork(void);

    // send data to all clients
    void SendToAll(std::uint8_t* packets, int totalSize);

    // send data to one client
    void SendToClient(std::uint8_t* packets, int totalSize, unsigned int clientId);

    // receive incoming data
    int ReceiveData(unsigned int client_id, std::uint8_t* recvbuf);

    // accept new connections
    bool AcceptNewClient(unsigned int& id);

    // Socket to listen for new connections
    SOCKET m_listenSocket = INVALID_SOCKET;

    // Socket to give to the clients
    SOCKET m_clientSocket = INVALID_SOCKET;

    // for error checking return values
    int m_result = 0;

    // table to keep track of each client's socket
    std::map<unsigned int, SOCKET> m_sessions;

private:
    std::string m_port;
};
