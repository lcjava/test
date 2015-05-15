/* 
 * File:   SocketOutputStream.h
 * Author: Vicky.H
 * Email:  eclipser@163.com
 *
 * Created on 2014年2月27日, 下午8:23
 */

#ifndef CN_VICKY__SOCKETOUTPUTSTREAM_H
#define	CN_VICKY__SOCKETOUTPUTSTREAM_H

#include "Socket.h"

//初始化的发送缓存长度
#define DEFAULTSOCKETOUTPUTBUFFERSIZE 8192
//最大可以允许的缓存长度，如果超过此数值，则断开连接
#define DISCONNECTSOCKETOUTPUTSIZE 100 * 1024


class Packet;

class SocketOutputStream {
public:
    SocketOutputStream(Socket* sock,
            unsigned int BufferSize = DEFAULTSOCKETOUTPUTBUFFERSIZE,
            unsigned int MaxBufferSize = DISCONNECTSOCKETOUTPUTSIZE);
    virtual ~SocketOutputStream();


public:
    unsigned int Write(const char* buf, unsigned int len);
    //bool						WritePacket( const Packet* pPacket ) ;

    unsigned int Flush();

    void Initsize();
    bool Resize(int size);

    int Capacity()const {
        return m_BufferLen;
    }

    unsigned int Length()const;

    unsigned int Size()const {
        return Length();
    }

    char* GetBuffer()const {
        return m_Buffer;
    }

    char* GetTail()const {
        return &(m_Buffer[m_Tail]);
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
protected:

    Socket* m_pSocket;

    char* m_Buffer;

    unsigned int m_BufferLen;
    unsigned int m_MaxBufferLen;

    unsigned int m_Head;
    unsigned int m_Tail;
};


#endif	/* CN_VICKY__SOCKETOUTPUTSTREAM_H */

