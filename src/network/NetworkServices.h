#ifdef _WIN32
    #include <winsock2.h>
    #include <Windows.h>
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0501  /* Windows XP. */
    #endif
    #include <winsock2.h>
    #include <Ws2tcpip.h>
#else
    /* Assume that any non-Windows platform uses POSIX-style sockets instead. */
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
    #include <unistd.h> /* Needed for close() */
    #typedef int SOCKET;
#endif

#include <cstdint>

#define SERVER_ID -1

class NetworkServices
{
public:
    static int sendMessage(SOCKET curSocket, std::uint8_t* message, int messageSize);
    static int receiveMessage(SOCKET curSocket, std::uint8_t* buffer, int bufSize);
    static int sockInit();
    static int sockQuit();
    static int sockClose(SOCKET sock);
};

