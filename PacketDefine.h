/* 
 * File:   PacketDefine.h
 * Author: Vicky.H
 * Email:  eclipser@163.com
 *
 * Created on 2014年2月27日, 下午8:41
 */

#ifndef CN_VICKY__PACKETDEFINE_H
#define	CN_VICKY__PACKETDEFINE_H


namespace packet {

    enum PACKET_DEF {
        PACKET_NONE = 0,                                                        //0，空
        PACKET_CS_CHAT,                                                         //聊天
        PACKET_SC_CHAT,                                                         //聊天
        PACKET_MAX                                                              //消息数量
    };
}


#endif	/* CN_VICKY__PACKETDEFINE_H */

