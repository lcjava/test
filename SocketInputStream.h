/* 
 * File:   SocketInputStream.h
 * Author: Vicky.H
 * Email:  eclipser@163.com
 *
 * Created on 2014年2月27日, 下午10:51
 */

#ifndef CN_VICKY__SOCKETINPUTSTREAM_H
#define	CN_VICKY__SOCKETINPUTSTREAM_H

#include "Socket.h"

//初始化的接收缓存长度
#define DEFAULTSOCKETINPUTBUFFERSIZE 64 * 1024
//最大可以允许的缓存长度，如果超过此数值，则断开连接
#define DISCONNECTSOCKETINPUTSIZE 96 * 1024

class Packet;

// 输入流

class SocketInputStream {
public:
    SocketInputStream(Socket* sock,
            unsigned int BufferSize = DEFAULTSOCKETINPUTBUFFERSIZE,
            unsigned int MaxBufferSize = DISCONNECTSOCKETINPUTSIZE);
    virtual ~SocketInputStream();


public:
    unsigned int Read(char* buf, unsigned int len);
    bool ReadPacket(Packet* p);

    bool Peek(char* buf, unsigned int len);

    bool Find(char* buf);

    bool Skip(unsigned int len);

    unsigned int Fill();
    unsigned int Fill_RAW();

    void Initsize();
    bool Resize(int size);

    unsigned int Capacity()const {
        return m_BufferLen;
    }

    unsigned int Length()const;

    unsigned int Size()const {
        return Length();
    }

    bool IsEmpty()const {
        return m_Head == m_Tail;
    }

    void CleanUp();

    unsigned int GetHead() {
        return m_Head;
    }

    unsigned int GetTail() {
        return m_Tail;
    }

    unsigned int GetBuffLen() {
        return m_BufferLen;
    }

    char* GetBuff() {
        return m_Buffer;
    }
private:
    Socket* m_pSocket;

    char* m_Buffer;

    unsigned int m_BufferLen;
    unsigned int m_MaxBufferLen;

    unsigned int m_Head;
    unsigned int m_Tail;

};


#endif	/* CN_VICKY__SOCKETINPUTSTREAM_H */

