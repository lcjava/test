/* 
 * File:   PacketFactoryManager.h
 * Author: Vicky.H
 * Email:  eclipser@163.com
 *
 * Created on 2014年2月27日, 下午8:47
 */

#ifndef CN_VICKY__PACKETFACTORYMANAGER_H
#define	CN_VICKY__PACKETFACTORYMANAGER_H


#include "Packet.h"
#include "PacketFactory.h"
#include "GameUtils.h"

class PacketFactoryManager {
public:
    PacketFactoryManager();
    ~PacketFactoryManager();
private:
    void AddFactory(PacketFactory* pFactory);

public:
    //外部调用通用接口

    //初始化接口
    bool Init();
    //根据消息类型从内存里分配消息实体数据（允许多线程同时调用）
    Packet* CreatePacket(PacketID_t packetID);
    //根据消息类型取得对应消息的最大尺寸（允许多线程同时调用）
    unsigned int GetPacketMaxSize(PacketID_t packetID);
    //删除消息实体（允许多线程同时调用）
    void RemovePacket(Packet* pPacket);

    void Lock() {
        m_Lock.Lock();
    };

    void Unlock() {
        m_Lock.Unlock();
    };

private:

    PacketFactory ** m_Factories;

    unsigned short m_Size;

    MyLock m_Lock;

public:
    unsigned int* m_pPacketAllocCount;

};

extern PacketFactoryManager* g_pPacketFactoryManager;

#endif	/* CN_VICKY__PACKETFACTORYMANAGER_H */

