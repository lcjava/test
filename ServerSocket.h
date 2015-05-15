/* 
 * File:   ServerSocket.h
 * Author: Vicky.H
 * Email:  eclipser@163.com
 *
 * Created on 2014年2月26日, 下午8:43
 */

#ifndef CN_VICKY__SERVERSOCKET_H
#define	CN_VICKY__SERVERSOCKET_H


#include "Socket.h"

class ServerSocket {
public:

    // constructor
    ServerSocket(unsigned int port, unsigned int backlog = 5);

    // destructor
    ~ServerSocket();

public:

    // close socket
    void close();

    // accept new connection
    bool accept(Socket* socket);

    // get/set socket's linger status

    unsigned int getLinger() {
        return m_Socket->getLinger();
    }

    void setLinger(unsigned int lingertime) {
        m_Socket->setLinger(lingertime);
    }

    // get/set socket's nonblocking status

    bool isNonBlocking() const {
        return m_Socket->isNonBlocking();
    }

    void setNonBlocking(bool on = true) {
        m_Socket->setNonBlocking(on);
    }

    // get/set receive buffer size

    unsigned int getReceiveBufferSize() const {
        return m_Socket->getReceiveBufferSize();
    }

    void setReceiveBufferSize(unsigned int size) {
        m_Socket->setReceiveBufferSize(size);
    }

    // get/set send buffer size

    unsigned int getSendBufferSize() const {
        return m_Socket->getSendBufferSize();
    }

    void setSendBufferSize(unsigned int size) {
        m_Socket->setSendBufferSize(size);
    }

    SOCKET getSOCKET() {
        return m_Socket->getSOCKET();
    }

protected:

    // socket implementation object
    Socket* m_Socket;

};


#endif	/* CN_VICKY__SERVERSOCKET_H */

