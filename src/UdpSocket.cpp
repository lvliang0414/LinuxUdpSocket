#include "UdpSocket.h"
#include <string.h>


UdpSocket::UdpSocket()
{
    m_Socket = -1;
    bzero(&m_ServerAddr, sizeof(m_ServerAddr));
    bzero(&m_ClientAddr, sizeof(m_ClientAddr));
    bzero(&m_DestAddr, sizeof(m_DestAddr));
    m_timeoutms = 3000;
}

UdpSocket::~UdpSocket()
{
    if (m_Socket > 0)
    {
        close(m_Socket);
    }
}

bool UdpSocket::Init(string host, int port)
{
    m_ServerAddr.sin_family = AF_INET;

    if (host == "")
        m_ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        m_ServerAddr.sin_addr.s_addr = inet_addr(host.c_str());
        
    m_ServerAddr.sin_port = htons(port);

    m_Socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (m_Socket < 0) {
        printf("Create socket failed!\n");
        return false;
    }

    if (bind(m_Socket, (struct sockaddr*)&m_ServerAddr, sizeof(m_ServerAddr)) < 0)
    {
        printf("Bind filed!\n");
        return false;
    }
    return true;
}

void UdpSocket::SetDest(string ip, int port)
{
    if (ip == "")
        return;
    if (port <= 0)
        return;

    bzero(&m_DestAddr, sizeof(m_DestAddr));
    m_DestAddr.sin_family = AF_INET;
    m_DestAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    m_DestAddr.sin_port = htons(port);
    socklen_t socketlen = sizeof(m_DestAddr);

}

void UdpSocket::SetTimeout(int ms)
{
    m_timeoutms = ms;
}

int UdpSocket::RecvFrom(char * buf, int len)
{
    socklen_t socklen = sizeof(m_ClientAddr);

    int size = 0;
    if (m_timeoutms <= 0) {
        size = recvfrom(m_Socket, buf, len, 0, (struct sockaddr*)&m_ClientAddr, &socklen);
    }
    else {
        fd_set fds;
        struct timeval tv;
        tv.tv_sec = m_timeoutms / 1000;
        tv.tv_usec = m_timeoutms % 1000;

        int maxsock = m_Socket;

        FD_ZERO(&fds);
        FD_SET(m_Socket, &fds);

        int ret = select(maxsock + 1, &fds, NULL, NULL, &tv);
        if (ret == -1) {
            perror("select()");
        }
        else if (ret == 0) {
            //printf("select timeout\n");
        }
        else {
            if (FD_ISSET(m_Socket, &fds)) {
                size = recvfrom(m_Socket, buf, len, 0, (struct sockaddr*)&m_ClientAddr, &socklen);
            }
        }
    }

    return size;
}

int UdpSocket::SendTo(char * buf, int len, const char * ip, int port)
{
    struct sockaddr_in dest_addr;

    bzero(&dest_addr, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(ip);
    dest_addr.sin_port = htons(port);
    socklen_t socketlen = sizeof(dest_addr);

    int size = sendto(m_Socket, buf, len, 0, (struct sockaddr*)&dest_addr, socketlen);
    return size;
}

int UdpSocket::Send(char * buf, int len)
{
    int size = sendto(m_Socket, buf, len, 0, (struct sockaddr*)&m_DestAddr, m_SocketLen);
    return size;

}

int UdpSocket::SendAnswer(char * buf, int len)
{
    int size = sendto(m_Socket, buf, len, 0, (struct sockaddr*)&m_ClientAddr, m_SocketLen);
    return size;

}
