
#include "NetworkServices.h"

int NetworkServices::Send(SOCKET curSocket, std::uint8_t* message, int messageSize)
{
    return send(curSocket, reinterpret_cast<char*>(message), messageSize, 0);
}

int NetworkServices::Receive(SOCKET curSocket, std::uint8_t* buffer, int bufSize)
{
    return recv(curSocket, reinterpret_cast<char*>(buffer), bufSize, 0);
}

int NetworkServices::SockInit()
{
#ifdef _WIN32
    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#else
    return 0;
#endif
}

int NetworkServices::SockQuit()
{
#ifdef _WIN32
    return WSACleanup();
#else
    return 0;
#endif
}

int NetworkServices::SockClose(SOCKET sock)
{
    int status = 0;

#ifdef _WIN32
    status = shutdown(sock, SD_BOTH);
    if (status == 0)
    {
        status = closesocket(sock);
    }
#else
    status = shutdown(sock, SHUT_RDWR);
    if (status == 0)
    {
        status = close(sock);
    }
#endif

    return status;
}