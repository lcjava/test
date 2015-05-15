#include <algorithm>
#include <string.h>

#include "SocketInputStream.h"
#include "Packet.h"

SocketInputStream::SocketInputStream(Socket* sock, unsigned int BufferSize, unsigned int MaxBufferSize) {
    m_pSocket = sock;
    m_BufferLen = BufferSize;
    m_MaxBufferLen = MaxBufferSize;


    m_Head = 0;
    m_Tail = 0;

    m_Buffer = new char[m_BufferLen];

    memset(m_Buffer, 0, m_BufferLen);
}

SocketInputStream::~SocketInputStream() {
    if (m_Buffer != NULL) {
        delete[] m_Buffer;
        m_Buffer = NULL;
    }
}

unsigned int SocketInputStream::Length() const {
    if (m_Head < m_Tail)
        return m_Tail - m_Head;

    else if (m_Head > m_Tail)
        return m_BufferLen - m_Head + m_Tail;

    return 0;
}

unsigned int SocketInputStream::Read(char* buf, unsigned int len) {
    if (len == 0)
        return 0;

    if (len > Length())
        return 0;

    if (m_Head < m_Tail) {
        memcpy(buf, &m_Buffer[m_Head], len);
    } else {
        unsigned int rightLen = m_BufferLen - m_Head;
        if (len <= rightLen) {
            memcpy(buf, &m_Buffer[m_Head], len);
        } else {
            memcpy(buf, &m_Buffer[m_Head], rightLen);
            memcpy(&buf[rightLen], m_Buffer, len - rightLen);
        }
    }

    m_Head = (m_Head + len) % m_BufferLen;

    return len;
}

bool SocketInputStream::ReadPacket(Packet* p) {
    bool ret;

    ret = Skip(PACKET_HEADER_SIZE);
    if (!ret)
        return false;

    return p->Read(*this);
}

bool SocketInputStream::Peek(char* buf, unsigned int len) {
    if (len == 0)
        return false;

    if (len > Length())
        return false;

    if (m_Head < m_Tail) {
        memcpy(buf, &m_Buffer[m_Head], len);

    } else {
        unsigned int rightLen = m_BufferLen - m_Head;
        if (len <= rightLen) {
            memcpy(&buf[0], &m_Buffer[m_Head], len);
        } else {
            memcpy(&buf[0], &m_Buffer[m_Head], rightLen);
            memcpy(&buf[rightLen], &m_Buffer[0], len - rightLen);
        }
    }

    return true;
}

bool SocketInputStream::Find(char* buf) {
    bool IsFind = false;
    while (Length() > PACK_COMPART_SIZE) {
        if (m_Head < m_Tail) {
            memcpy(buf, &m_Buffer[m_Head], PACK_COMPART_SIZE);
        } else {
            unsigned int rightLen = m_BufferLen - m_Head;
            if (PACK_COMPART_SIZE <= rightLen) {
                memcpy(&buf[0], &m_Buffer[m_Head], PACK_COMPART_SIZE);
            } else {
                memcpy(&buf[0], &m_Buffer[m_Head], rightLen);
                memcpy(&buf[rightLen], &m_Buffer[0], PACK_COMPART_SIZE - rightLen);
            }
        }
        if (strstr(buf, PACK_COMPART) == NULL) {
            m_Head++;
            if (m_Head >= m_BufferLen) {
                m_Head -= m_BufferLen;
            }
            continue;
        } else {
            IsFind = true;
            break;
        }
    }

    if (!IsFind) {
        return false;
    }

    m_Head += (unsigned int) PACK_COMPART_SIZE;
    if (m_Head >= m_BufferLen) {
        m_Head -= m_BufferLen;
    }
    return true;
}

bool SocketInputStream::Skip(unsigned int len) {
    if (len == 0)
        return false;

    if (len > Length())
        return false;

    m_Head = (m_Head + len) % m_BufferLen;

    return true;
}

void SocketInputStream::Initsize() {
    m_Head = 0;
    m_Tail = 0;

    if (m_Buffer != NULL) {
        delete[] m_Buffer;
        m_Buffer = NULL;
    }

    m_Buffer = new char[DEFAULTSOCKETINPUTBUFFERSIZE];
    m_BufferLen = DEFAULTSOCKETINPUTBUFFERSIZE;

    memset(m_Buffer, 0, m_BufferLen);
}

unsigned int SocketInputStream::Fill() {
    unsigned int nFilled = 0;
    unsigned int nReceived = 0;
    unsigned int nFree = 0;

    if (m_Head <= m_Tail) {
        if (m_Head == 0) {
            //
            // H   T		LEN=10
            // 0123456789
            // abcd......
            //

            nReceived = 0;
            nFree = m_BufferLen - m_Tail - 1;
            if (nFree != 0) {
                nReceived = m_pSocket->receive(&m_Buffer[m_Tail], nFree);
                if (nReceived == SOCKET_ERROR_WOULDBLOCK) return 0;
                if (nReceived == SOCKET_ERROR) return SOCKET_ERROR - 1;
                if (nReceived == 0) return SOCKET_ERROR - 2;

                m_Tail += nReceived;
                nFilled += nReceived;
            }

            if (nReceived == nFree) {
                unsigned int available = m_pSocket->available();
                if (available > 0) {
                    if ((m_BufferLen + available + 1) > m_MaxBufferLen) {
                        Initsize();
                        return SOCKET_ERROR - 3;
                    }
                    if (!Resize(available + 1))
                        return 0;

                    nReceived = m_pSocket->receive(&m_Buffer[m_Tail], available);
                    if (nReceived == SOCKET_ERROR_WOULDBLOCK) return 0;
                    if (nReceived == SOCKET_ERROR) return SOCKET_ERROR - 4;
                    if (nReceived == 0) return SOCKET_ERROR - 5;

                    m_Tail += nReceived;
                    nFilled += nReceived;
                }
            }
        } else {
            //
            //    H   T		LEN=10
            // 0123456789
            // ...abcd...
            //

            nFree = m_BufferLen - m_Tail;
            nReceived = m_pSocket->receive(&m_Buffer[m_Tail], nFree);
            if (nReceived == SOCKET_ERROR_WOULDBLOCK) return 0;
            if (nReceived == SOCKET_ERROR) return SOCKET_ERROR - 6;
            if (nReceived == 0) return SOCKET_ERROR - 7;

            m_Tail = (m_Tail + nReceived) % m_BufferLen;
            nFilled += nReceived;

            if (nReceived == nFree) {
                //				Assert( m_Tail == 0 );

                nReceived = 0;
                nFree = m_Head - 1;
                if (nFree != 0) {
                    nReceived = m_pSocket->receive(&m_Buffer[0], nFree);
                    if (nReceived == SOCKET_ERROR_WOULDBLOCK) return 0;
                    if (nReceived == SOCKET_ERROR) return SOCKET_ERROR - 8;
                    if (nReceived == 0) return SOCKET_ERROR - 9;

                    m_Tail += nReceived;
                    nFilled += nReceived;
                }

                if (nReceived == nFree) {
                    unsigned int available = m_pSocket->available();
                    if (available > 0) {
                        if ((m_BufferLen + available + 1) > m_MaxBufferLen) {
                            Initsize();
                            return SOCKET_ERROR - 10;
                        }
                        if (!Resize(available + 1))
                            return 0;

                        nReceived = m_pSocket->receive(&m_Buffer[m_Tail], available);
                        if (nReceived == SOCKET_ERROR_WOULDBLOCK) return 0;
                        if (nReceived == SOCKET_ERROR) return SOCKET_ERROR - 11;
                        if (nReceived == 0) return SOCKET_ERROR - 12;

                        m_Tail += nReceived;
                        nFilled += nReceived;
                    }
                }
            }
        }

    } else {
        //
        //     T  H		LEN=10
        // 0123456789
        // abcd...efg
        //

        nReceived = 0;
        nFree = m_Head - m_Tail - 1;
        if (nFree != 0) {
            nReceived = m_pSocket->receive(&m_Buffer[m_Tail], nFree);
            if (nReceived == SOCKET_ERROR_WOULDBLOCK) return 0;
            if (nReceived == SOCKET_ERROR) return SOCKET_ERROR - 13;
            if (nReceived == 0) return SOCKET_ERROR - 14;

            m_Tail += nReceived;
            nFilled += nReceived;
        }
        if (nReceived == nFree) {
            unsigned int available = m_pSocket->available();
            if (available > 0) {
                if ((m_BufferLen + available + 1) > m_MaxBufferLen) {
                    Initsize();
                    return SOCKET_ERROR - 15;
                }
                if (!Resize(available + 1))
                    return 0;

                nReceived = m_pSocket->receive(&m_Buffer[m_Tail], available);
                if (nReceived == SOCKET_ERROR_WOULDBLOCK) return 0;
                if (nReceived == SOCKET_ERROR) return SOCKET_ERROR - 16;
                if (nReceived == 0) return SOCKET_ERROR - 17;

                m_Tail += nReceived;
                nFilled += nReceived;
            }
        }
    }

    return nFilled;
}

bool SocketInputStream::Resize(int size) {
    size = std::max(size, (int) (m_BufferLen >> 1));
    unsigned int newBufferLen = m_BufferLen + size;
    unsigned int len = Length();

    if (size < 0) {
        if (newBufferLen < len)
            return false;
    }

    char * newBuffer = new char[newBufferLen];

    if (m_Head < m_Tail) {
        memcpy(newBuffer, &m_Buffer[m_Head], m_Tail - m_Head);
    } else if (m_Head > m_Tail) {
        memcpy(newBuffer, &m_Buffer[m_Head], m_BufferLen - m_Head);
        memcpy(&newBuffer[ m_BufferLen - m_Head ], m_Buffer, m_Tail);
    }

    delete [] m_Buffer;

    m_Buffer = newBuffer;
    m_BufferLen = newBufferLen;
    m_Head = 0;
    m_Tail = len;

    return true;
}

void SocketInputStream::CleanUp() {
    m_Head = 0;
    m_Tail = 0;
}
