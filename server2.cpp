/* 
 * File:   server2.cpp
 * Author: Vicky.H
 * Email:  eclipser@163.com
 */
#include "ServerSocket.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <string.h>
#include <algorithm>

/*
 * 
 */
int main(void) {

    ServerSocket server(8080);
    if (server.isNonBlocking()) {
        server.setNonBlocking(true);
        std::cout << "set nonblocking !" << std::endl;
    }
    std::vector<Socket*> clients;
    std::atomic<bool> flag1(true);

    std::thread t1(
            [&server, &clients, &flag1]() {
                std::cout << "Thread1 is running" << std::endl;
                while (flag1.load()) {
                    Socket* _client = new Socket();
                    if (server.accept(_client)) {
                        clients.push_back(_client);
                        std::cout << "New Client" << _client->m_Host << ":" << _client->m_Port << std::endl;
                    }
                }
                std::cout << "Thread1 Over" << std::endl;
            }
    );
    t1.detach();

    std::thread t2(
            [&clients, &flag1]() {
                std::cout << "Thread2 is running" << std::endl;
                while (flag1.load()) {
                    ;
                    clients.end();
                    for (Socket* _client : clients) {
                        if (_client) {
                            char buf[256];
                            int recv = _client->receive(buf, 256);
                            std::cout << _client->m_Host << ":" << _client->m_Port << " " << time(0) << " :\n" << buf << "recv:" << recv << std::endl;
//                            if (strncmp("exit", "buf", 4) == 0) {
//                            }
                        }
                    }
                }
                std::cout << "Thread2 Over" << std::endl;
            }
    );
    t2.detach();

    // 系统公告
    std::thread t3(
            [&clients, &flag1]() {
                std::cout << "Thread3 is running" << std::endl;
                while (flag1.load()) {
                    std::this_thread::sleep_for(std::chrono::seconds(20));
                            char buf[256] = {0};
                    int n = sprintf(buf, "NOW:%d  Online:[%d]\n", time(0), clients.size());
                    for (Socket* _client : clients) {
                        if (_client) {
                            _client->send(buf, n);
                        }
                    }
                }
                std::cout << "Thread3 Over" << std::endl;
            }
    );

    std::this_thread::sleep_for(std::chrono::seconds(1));
    char buf[64] = {0};
    for (;;) {
        std::cout << "<<" << std::flush;
        std::cin >> buf;
        if (strncmp("exit", buf, 4) == 0) {
            flag1.exchange(false);

            server.close();
            std::cout << "Over" << std::endl;
            break;
        }
        memset(buf, 0, 64);
    }

    return 0;
}
