#include "ServerSocket.h"
#include <string.h>

ServerSocket::ServerSocket(unsigned int port, unsigned int backlog) {
    bool ret = false;
#define KEY_MAC 12
    m_Socket = new Socket;
    //	Assert( m_Socket ) ;
    if (m_Socket == NULL)
        throw 1;

    // create socket
    ret = m_Socket->create();
    if (ret == false)
        throw 1;
    ret = m_Socket->setReuseAddr();
    if (ret == false)
        throw 1;
    ret = m_Socket->bind(port);
    if (ret == false)
        throw 1;
    ret = m_Socket->listen(backlog);
    if (ret == false)
        throw 1;
}

ServerSocket::~ServerSocket() {
    if (m_Socket != NULL) {
        m_Socket->close();
        delete m_Socket;
        m_Socket = NULL;
    }
}

void ServerSocket::close() {
    if (m_Socket) {
        m_Socket->close();
    }
}

bool ServerSocket::accept(Socket* socket) {
    unsigned int addrlen = sizeof (SOCKADDR_IN);
    socket->close();

    socket->m_SocketID = m_Socket->accept((struct sockaddr *) (&(socket->m_SockAddr)), &addrlen);
    if (socket->m_SocketID == INVALID_SOCKET)
        return false;

    socket->m_Port = ntohs(socket->m_SockAddr.sin_port);
    strncpy(socket->m_Host, inet_ntoa(socket->m_SockAddr.sin_addr), IP_SIZE - 1);

    return true;
}




