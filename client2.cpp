/* 
 * File:   client2.cpp
 * Author: Vicky.H
 * Email:  eclipser@163.com
 */
#include "Socket.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <string.h>

/*
 * 
 */
int main(void) {
    Socket client("192.168.1.98", 8080);
    std::atomic<bool> flag(true);
    if (client.connect()) {

        // READ
        std::thread t1(
                [&client, &flag]() {
                    std::cout << "READ THREAD IS RUNING" << std::endl;
                    char buf[256] = {0};
                    while(flag.load()) {
                        int n = client.receive(buf, 256);
                        std::cout << "recv:" << buf << std::flush;
                        memset(buf, 0, n);
                    }
                    std::cout << "READ THREAD IS OVER" << std::endl;
                }
        );
        t1.detach();
        
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        // WRITE
        std::thread t2(
                [&client, &flag](){
                    std::cout << "WRITE THREAD IS RUNING" << std::endl;
                    char buf[256] = {0};
                    while(flag.load()) {
                        std::cin >> buf;
                        int n = client.send(buf, strlen(buf));
                        if (strncmp("exit", buf, 4) == 0) {
                            flag.exchange(false);
                        }
                        memset(buf, 0, n);
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    client.close();
                    std::cout << "READ THREAD IS OVER" << std::endl;
                }
        );
        t2.join();
    }
    return 0;
}
