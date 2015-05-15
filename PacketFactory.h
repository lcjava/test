/* 
 * File:   PacketFactory.h
 * Author: Vicky.H
 * Email:  eclipser@163.com
 *
 * Created on 2014年2月27日, 下午8:44
 */

#ifndef CN_VICKY__PACKETFACTORY_H
#define	CN_VICKY__PACKETFACTORY_H

#include "Packet.h"

class PacketFactory {
public:

    virtual ~PacketFactory() {
    }

    // 分配包
    virtual Packet* CreatePacket() = 0;
    // 获得包ID
    virtual PacketID_t GetPacketID()const = 0;
    // 最大包
    virtual unsigned int GetPacketMaxSize()const = 0;

};


#endif	/* CN_VICKY__PACKETFACTORY_H */

