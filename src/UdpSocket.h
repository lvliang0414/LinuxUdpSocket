#ifndef _LINUX_UDP_SOCKET_UDP_SOCKET_H_
#define _LINUX_UDP_SOCKET_UDP_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>

using namespace std;

class UdpSocket
{
public:
    UdpSocket();
    ~UdpSocket();

    bool Init(string host="", int port=0);
    void SetDest(string ip, int port);
    int RecvFrom(char * buf, int len);
    int SendTo(char * buf, int len, const char * addr, int port);
    int Send(char * buf, int len);
    int SendAnswer(char * buf, int len);

private:
    int m_Socket;
    socklen_t m_SocketLen;
    struct sockaddr_in  m_ServerAddr;
    struct sockaddr_in  m_ClientAddr;
    struct sockaddr_in  m_DestAddr;
};

#endif
