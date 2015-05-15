/* 
 * File:   GameUtils.h
 * Author: Vicky.H
 * Email:  eclipser@163.com
 *
 * Created on 2014年3月8日, 下午3:43
 */

#ifndef CN_VICKY__GAMEUTILS_H
#define	CN_VICKY__GAMEUTILS_H
#include <cstddef>
#include <pthread.h>


#define __PI			3.1415f
#define __HALF_PI		__PI / 2
#define __QUARTER_PI            __PI / 4


//共享锁

class MyLock {
    pthread_mutex_t m_Mutex;
public:

    MyLock() {
        pthread_mutex_init(&m_Mutex, NULL);
    };

    ~MyLock() {
        pthread_mutex_destroy(&m_Mutex);
    };

    void Lock() {
        pthread_mutex_lock(&m_Mutex);
    };

    void Unlock() {
        pthread_mutex_unlock(&m_Mutex);
    };
};


//自动加锁解锁器

class AutoLock_T {
public:

    AutoLock_T(MyLock& rLock) {
        m_pLock = &rLock;
        m_pLock->Lock();
    }

    ~AutoLock_T() {
        m_pLock->Unlock();
    }
protected:
private:
    AutoLock_T();
    MyLock* m_pLock;
};

#endif	/* CN_VICKY__GAMEUTILS_H */

