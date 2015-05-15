#include <algorithm>
#include <string.h>

#include "SocketOutputStream.h"
#include "Packet.h"

SocketOutputStream::SocketOutputStream(Socket* sock, unsigned int BufferSize, unsigned int MaxBufferSize) {
    m_pSocket = sock;
    m_BufferLen = BufferSize;
    m_MaxBufferLen = MaxBufferSize;

    m_Head = 0;
    m_Tail = 0;

    m_Buffer = new char[ m_BufferLen ];
    memset(m_Buffer, 0, m_BufferLen);
}

SocketOutputStream::~SocketOutputStream() {
    if (m_Buffer != NULL) {
        delete[] m_Buffer;
        m_Buffer = NULL;
    }
}

unsigned int SocketOutputStream::Length() const {
    if (m_Head < m_Tail)
        return m_Tail - m_Head;

    else if (m_Head > m_Tail)
        return m_BufferLen - m_Head + m_Tail;

    return 0;
}

unsigned int SocketOutputStream::Write(const char* buf, unsigned int len) {
    //					//
    //     T  H			//    H   T			LEN=10
    // 0123456789		// 0123456789
    // abcd...efg		// ...abcd...
    //					//

    unsigned int nFree = ((m_Head <= m_Tail) ? (m_BufferLen - m_Tail + m_Head - 1) : (m_Head - m_Tail - 1));

    if (len >= nFree) {
        if (!Resize(len - nFree + 1))
            return 0;
    }

    if (m_Head <= m_Tail) {
        if (m_Head == 0) {
            nFree = m_BufferLen - m_Tail - 1;
            memcpy(&m_Buffer[m_Tail], buf, len);
        } else {
            nFree = m_BufferLen - m_Tail;
            if (len <= nFree) {
                memcpy(&m_Buffer[m_Tail], buf, len);
            } else {
                memcpy(&m_Buffer[m_Tail], buf, nFree);
                memcpy(m_Buffer, &buf[nFree], len - nFree);
            }
        }
    } else {
        memcpy(&m_Buffer[m_Tail], buf, len);
    }

    m_Tail = (m_Tail + len) % m_BufferLen;

    return len;
}

void SocketOutputStream::Initsize() {
    m_Head = 0;
    m_Tail = 0;

    if (m_Buffer != NULL) {
        delete[] m_Buffer;
        m_Buffer = NULL;
    }

    m_Buffer = new char[DEFAULTSOCKETOUTPUTBUFFERSIZE];

    m_BufferLen = DEFAULTSOCKETOUTPUTBUFFERSIZE;

    memset(m_Buffer, 0, m_BufferLen);
}

unsigned int SocketOutputStream::Flush() {
    unsigned int nFlushed = 0;
    unsigned int nSent = 0;
    unsigned int nLeft;

    if (m_BufferLen > m_MaxBufferLen) {//如果单个客户端的缓存太大，则重新设置缓存，并将此客户端断开连接
        Initsize();

        return SOCKET_ERROR - 1;
    }

    unsigned int flag = MSG_NOSIGNAL;

    try {
        if (m_Head < m_Tail) {
            nLeft = m_Tail - m_Head;

            while (nLeft > 0) {
                nSent = m_pSocket->send(&m_Buffer[m_Head], nLeft, flag);
                if (nSent == SOCKET_ERROR_WOULDBLOCK) return 0;
                if (nSent == SOCKET_ERROR) return SOCKET_ERROR - 2;
                if (nSent == 0) return 0;

                nFlushed += nSent;
                nLeft -= nSent;
                m_Head += nSent;
            }
            // Assert( nLeft == 0 );
        } else if (m_Head > m_Tail) {
            nLeft = m_BufferLen - m_Head;

            while (nLeft > 0) {
                nSent = m_pSocket->send(&m_Buffer[m_Head], nLeft, flag);
                if (nSent == SOCKET_ERROR_WOULDBLOCK) return 0;
                if (nSent == SOCKET_ERROR) return SOCKET_ERROR - 3;
                if (nSent == 0) return 0;

                nFlushed += nSent;
                nLeft -= nSent;
                m_Head += nSent;
            }

            // Assert( m_Head == m_BufferLen );

            m_Head = 0;

            nLeft = m_Tail;

            while (nLeft > 0) {
                nSent = m_pSocket->send(&m_Buffer[m_Head], nLeft, flag);
                if (nSent == SOCKET_ERROR_WOULDBLOCK) return 0;
                if (nSent == SOCKET_ERROR) return SOCKET_ERROR - 4;
                if (nSent == 0) return 0;

                nFlushed += nSent;
                nLeft -= nSent;
                m_Head += nSent;
            }

            // Assert( nLeft == 0 );
        }

        if (m_Head != m_Tail) {
            // Assert( m_Head == m_Tail );
        }

    } catch (...) {
        if (nSent > 0) {
            m_Head += nSent;
        }
    }

    m_Head = m_Tail = 0;

    return nFlushed;
}

bool SocketOutputStream::Resize(int size) {
    int orgSize = size;

    size = std::max(size, (int) (m_BufferLen >> 1));
    unsigned int newBufferLen = m_BufferLen + size;
    unsigned int len = Length();

    if (size < 0) {
        if (newBufferLen < 0 || newBufferLen < len)
            return false;
    }

    char * newBuffer = new char[ newBufferLen ];
    //	Assert( newBuffer ) ;
    if (newBuffer == NULL)
        return false;

    if (m_Head < m_Tail) {
        memcpy(newBuffer, &m_Buffer[m_Head], m_Tail - m_Head);
    } else if (m_Head > m_Tail) {
        memcpy(newBuffer, &m_Buffer[m_Head], m_BufferLen - m_Head);
        memcpy(&newBuffer[m_BufferLen - m_Head], m_Buffer, m_Tail);
    }

    delete [] m_Buffer;

    m_Buffer = newBuffer;
    m_BufferLen = newBufferLen;
    m_Head = 0;
    m_Tail = len;

    return true;
}

void SocketOutputStream::CleanUp() {
    m_Head = 0;
    m_Tail = 0;
}

