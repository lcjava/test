/* 
 * File:   main.cpp
 * Author: Vicky.H
 * Email:  eclipser@163.com
 */
#include <iostream>
#include <thread>
#include <atomic>
#include <list>
#include <string.h>

#include "SocketAPI.h"

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
 * 
 */
int main(void) {

    //TCPSERVER:socket()  bind()  listen() accept()  read()/write()  close()/shutdown
    //TCPCLIENT:socket()                   connect() write()/read()  close()/shutdown
    //sockaddr_in.sin_port = htons(port); 转换int类型的端口
    //inet_pton(AF_INET, "192.168.1.78", &sockaddr_in.sin_addr); 转换string类型地址

    SOCKET sock = SocketAPI::socket_ex(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        perror("creare sock error : ");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "192.168.1.98", &addr.sin_addr);

    bool flag = SocketAPI::bind_ex(sock, (const struct sockaddr *) &addr, sizeof (struct sockaddr_in));
    if (!flag) {
        perror("bind sock error : ");
        return 1;
    }

    flag = SocketAPI::listen_ex(sock, 5);
    if (!flag) {
        perror("listen sock error : ");
        return 1;
    }
    
    std::list<SOCKET> clients(10);

    for (;;) {
        SOCKET clientSock = SocketAPI::accept_ex(sock, nullptr, nullptr);
        if (clientSock != -1) {
            std::atomic<bool>* _flag = new std::atomic<bool>(true);
            // 创建读取
            std::cout << "new Client:" << clientSock << std::endl;
            std::thread __readthread(processClientRead, clientSock, _flag);
            __readthread.detach();
            // 创建写入
            std::thread __writethread(processClientWrite, clientSock, _flag);
            __writethread.detach();
            clients.push_back(clientSock);
        }
    }
    
//    for (SOCKET clientSock : clients) {
//        SocketAPI::closesocket_ex(clientSock);
//    }
    
    SocketAPI::closesocket_ex(sock);
    
    std::cout << "Done !" << std::endl;
    return 0;
}
