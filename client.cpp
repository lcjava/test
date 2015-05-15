/* 
 * File:   client.cpp
 * Author: Vicky.H
 * Email:  eclipser@163.com
 */
#include "Socket.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <string.h>


void processRead(Socket* sock, std::atomic<bool>* _flag) {
    char buf[256] = {0};
    while(_flag->load()) {
        unsigned int n = sock->receive(buf, 256);
        std::cout << "recv:" << buf << std::endl;
        if (strncmp(buf, "exit", 4) == 0) {
            _flag->exchange(false);
        }
        memset(buf, 0, n + 1);
    }
    std::cout << "processClientRead Over" << std::endl;
}

void processWrite(Socket* sock, std::atomic<bool>* _flag) {
    char buf[256] = {0};
    while(_flag->load()) {
        std::cin >> buf;
        sock->send(buf, strlen(buf));
        if (strncmp(buf, "exit", 4) == 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
            _flag->exchange(false);
        }
    }
}


/*
 * 
 */
int main(void) {

    //TCPSERVER:socket()  bind()  listen() accept()  read()/write()  close()/shutdown
    //TCPCLIENT:socket()                   connect() write()/read()  close()/shutdown
    //sockaddr_in.sin_port = htons(port); 转换int类型的端口
    //inet_pton(AF_INET, "192.168.1.78", &sockaddr_in.sin_addr); 转换string类型地址
    Socket client("192.168.1.98", 8080);
    client.connect();
    
    std::atomic<bool>* _flag = new std::atomic<bool>(true);
    
    std::thread read(processRead, &client, _flag);
    read.detach();
    std::thread write(processWrite, &client, _flag);
    write.join();
            
    return 0;
}
