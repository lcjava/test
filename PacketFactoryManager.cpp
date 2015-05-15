#include "PacketFactoryManager.h"
#include "PacketDefine.h"
#include <cassert>
#include <cstddef>
#include <cstdio>


PacketFactoryManager* g_pPacketFactoryManager = NULL;

PacketFactoryManager::PacketFactoryManager() {

    m_Factories = NULL;
    m_Size = packet::PACKET_MAX;

    assert(m_Size > 0);

    m_Factories = new PacketFactory*[m_Size];
    assert(m_Factories);
    m_pPacketAllocCount = new unsigned int[m_Size];
    assert(m_pPacketAllocCount);

    for (int i = 0; i < m_Size; i++) {
        m_Factories[i] = NULL;
        m_pPacketAllocCount[i] = 0;
    }

}

PacketFactoryManager::~PacketFactoryManager() {

    assert(m_Factories != NULL);

    for (int i = 0; i < m_Size; i++) {
        if (m_Factories[i] != NULL) {
            delete m_Factories[i];
            m_Factories[i] = NULL;
        }
    }

    if (m_Factories != NULL) {
        delete[] m_Factories;
        m_Factories = NULL;
    }

    if (m_pPacketAllocCount != NULL) {
        delete[] m_pPacketAllocCount;
        m_pPacketAllocCount = NULL;
    }
}

// TODO 每添加一个数据包，就创建一个对应的Factory，并且将Factory添加到管理中！

bool PacketFactoryManager::Init() {
    //    AddFactory(new CSLoginPacketFactory());
    return true;
}

Packet* PacketFactoryManager::CreatePacket(PacketID_t packetID) {

    if (packetID >= m_Size || m_Factories[packetID] == NULL) {
        assert(false);
        return NULL;
    }

    Packet* pPacket = NULL;
    Lock();
    try {
        pPacket = m_Factories[packetID]->CreatePacket();
        m_pPacketAllocCount[packetID]++;
    } catch (...) {
        pPacket = NULL;
    }
    Unlock();

    return pPacket;

}

unsigned int PacketFactoryManager::GetPacketMaxSize(PacketID_t packetID) {

    if (packetID >= m_Size || m_Factories[packetID] == NULL) {
        // char buff[256] = {0};
        // sprintf(buff, "PacketID= %d 消息没有注册到PacketFactoryManager上", packetID);
        assert(false);
        return 0;
    }

    Lock();
    unsigned int iRet = m_Factories[packetID]->GetPacketMaxSize();
    Unlock();

    return iRet;
}

void PacketFactoryManager::RemovePacket(Packet* pPacket) {
    if (pPacket == NULL) {
        assert(false);
        return;
    }

    PacketID_t packetID = pPacket->GetPacketID();
    if (packetID >= m_Size) {
        assert(false);
        return;
    }

    Lock();
    try {
        if (pPacket != NULL) {
            delete pPacket;
            pPacket = NULL;
        }
        m_pPacketAllocCount[packetID]--;
    } catch (...) {
    }
    Unlock();
}