/* 
 * File:   SCChartMessage.h
 * Author: Vicky.H
 * Email:  eclipser@163.com
 *
 * Created on 2014年3月8日, 下午4:30
 */

#ifndef CN_VICKY__SCcharTMESSAGE_H
#define	CN_VICKY__SCcharTMESSAGE_H

#include "Packet.h"
#include "PacketFactory.h"
#include "CSChat.h"
#include "ChatDefine.h"

namespace packet {

    class SCChatMessage : public Packet {
    public:

        SCChatMessage() {
            Reset();
        };

        virtual ~SCChatMessage() {
        };


        //公用继承接口
        virtual bool Read(SocketInputStream& iStream);
        virtual bool Write(SocketOutputStream& oStream)const;
        virtual unsigned int Execute(Player* pPlayer);

        virtual PacketID_t GetPacketID()const {
            return PACKET_SC_CHAT;
        }

        virtual unsigned int GetPacketSize()const {
            if (m_ChatType == CHAT_TYPE_NORMAL) {
                return sizeof (BYTE) +
                        sizeof (BYTE) +
                        m_ContexSize * sizeof (char) +
                        sizeof (BYTE) +
                        m_SourNameSize * sizeof (char) +
                        sizeof (ObjID_t) +
                        sizeof (CampID_t) +
                        sizeof (UINT);
            } else {
                return sizeof (BYTE) +
                        sizeof (BYTE) +
                        m_ContexSize * sizeof (char) +
                        sizeof (BYTE) +
                        m_SourNameSize * sizeof (char) +
                        sizeof (CampID_t) +
                        sizeof (UINT);
            }
        }

        void Reset() {
            m_ChatType = 0;
            m_ContexSize = 0;
            m_SourNameSize = 0;
            memset(m_Contex, 0, MAX_CHAT_SIZE);
            memset(m_SourName, 0, MAX_charACTER_NAME);
        };

    public:
        //使用数据接口

        //设置、读取聊天类型 见：CGChat::CHAT_TYPE

        void SetChatType(BYTE bType) {
            m_ChatType = bType;
        };

        BYTE GetChatType() {
            return m_ChatType;
        };

        //设置、读取聊天内容

        void SetContexSize(BYTE bSize) {
            m_ContexSize = bSize;
        };

        BYTE GetContexSize() {
            return m_ContexSize;
        };

        void SetContex(char* pContex) {
            strncpy(m_Contex, pContex, MAX_CHAT_SIZE - 1);
            m_Contex[MAX_CHAT_SIZE - 1] = 0;
        };

        char* GetContex() {
            return (char*) m_Contex;
        };

        //设置、读取说话人的名字

        void SetSourNameSize(BYTE bSize) {
            m_SourNameSize = bSize;
        };

        BYTE GetSourNameSize() {
            return m_SourNameSize;
        };

        void SetSourName(const char* pSourName) {
            strncpy(m_SourName, pSourName, MAX_charACTER_NAME - 1);
            m_SourName[MAX_charACTER_NAME - 1] = 0;
        };

        char* GetSourName() {
            return (char*) m_SourName;
        };

    private:
        //数据
        BYTE m_ChatType; //聊天消息类型 enum CGChat::CHAT_TYPE

        //聊天内容数据
        BYTE m_ContexSize;
        char m_Contex[MAX_CHAT_SIZE];

        //说话者名字
        BYTE m_SourNameSize;
        char m_SourName[MAX_charACTER_NAME];
    };

    class SCChatFactory : public PacketFactory {
    public:

        Packet* CreatePacket() {
            return new SCChatMessage();
        }

        PacketID_t GetPacketID()const {
            return PACKET_GC_CHAT;
        }

        UINT GetPacketMaxSize()const {
            return sizeof (BYTE) +
                    sizeof (BYTE) +
                    sizeof (CHAR) * MAX_CHAT_SIZE +
                    sizeof (BYTE) +
                    sizeof (CHAR) * MAX_CHARACTER_NAME +
                    sizeof (ObjID_t) +
                    sizeof (CampID_t) +
                    sizeof (UINT);
        }
    };

    class SCChatHandler {
    public:
        static unsigned int Execute(SCChatMessage* pPacket, Player* pPlayer);
    };

};


#endif	/* CN_VICKY__SCcharTMESSAGE_H */

