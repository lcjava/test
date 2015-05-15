/* 
 * File:   main1.cpp
 * Author: Vicky.H
 * Email:  eclipser@163.com
 */
#include "Socket.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <string.h>

void processClientRead(SOCKET clientsock, std::atomic<bool>* _flag) {
    char buf[256] = {0};
    while (_flag->load()) {
        int n = SocketAPI::recv_ex(clientsock, buf, 256, 0);
        std::cout << "recv:" << buf << std::endl;
        if (strncmp(buf, "exit", 4) == 0) {
            _flag->exchange(false);
        }
        memset(buf, 0, n + 1);
    }
    std::cout << "processClientRead Over" << std::endl;
}

void processClientWrite(SOCKET clientsock, std::atomic<bool>* _flag) {
    char buf[256] = {0};
    while (_flag->load()) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        sprintf(buf, "now: %d\n", time(0));
        SocketAPI::send_ex(clientsock, buf, strlen(buf) + 1, 0);
    }
    sprintf(buf, "exit\n", time(0));
    SocketAPI::send_ex(clientsock, buf, strlen(buf) + 1, 0);

    std::this_thread::sleep_for(std::chrono::microseconds(100));

    SocketAPI::closesocket_ex(clientsock);

    std::cout << "processClientWrite Over" << std::endl;
}

/*
 * g++ -std=c++11 -pthread   -c -g -MMD -MP -MF "build/Debug/GNU-Linux-x86/Socket.o.d" -o build/Debug/GNU-Linux-x86/Socket.o Socket.cpp
 * g++ -std=c++11 -pthread   -c -g -MMD -MP -MF "build/Debug/GNU-Linux-x86/SocketAPI.o.d" -o build/Debug/GNU-Linux-x86/SocketAPI.o SocketAPI.cpp
 * g++ -std=c++11 -pthread   -c -g -MMD -MP -MF "build/Debug/GNU-Linux-x86/server.o.d" -o build/Debug/GNU-Linux-x86/server.o server.cpp
 * g++ -std=c++11 -pthread    -o dist/Debug/GNU-Linux-x86/socket build/Debug/GNU-Linux-x86/Socket.o build/Debug/GNU-Linux-x86/SocketAPI.o build/Debug/GNU-Linux-x86/server.o 
 */
int main(void) {

    //TCPSERVER:socket()  bind()  listen() accept()  read()/write()  close()/shutdown
    //TCPCLIENT:socket()                   connect() write()/read()  close()/shutdown
    //sockaddr_in.sin_port = htons(port); 转换int类型的端口
    //inet_pton(AF_INET, "192.168.1.78", &sockaddr_in.sin_addr); 转换string类型地址
    Socket s("192.168.1.98", 8080);
    s.bind();
    s.listen(5);
    for (;;) {
        SOCKADDR addr;
        unsigned int len;
        SOCKET clientSock = s.accept(&addr, &len);
        if (clientSock != INVALID_SOCKET) {
            std::atomic<bool>* _flag = new std::atomic<bool>(true);
            // 创建读取
            std::cout << "new Client:" << clientSock << std::endl;
            std::thread __readthread(processClientRead, clientSock, _flag);
            __readthread.detach();
            // 创建写入
            std::thread __writethread(processClientWrite, clientSock, _flag);
            __writethread.detach();
        }
    }

    s.close();
    return 0;
}
