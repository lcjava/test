/* 
 * File:   ChatDefine.h
 * Author: Vicky.H
 * Email:  eclipser@163.com
 *
 * Created on 2014年3月8日, 下午4:34
 */

#ifndef CN_VICKY__CHATDEFINE_H
#define	CN_VICKY__CHATDEFINE_H

#define MAX_CHAT_SIZE 256
#define MAX_CHARACTER_NAME 32

// 聊天类型
enum ENUM_CHAT_TYPE {
    CHAT_TYPE_INVALID = -1,

    CHAT_TYPE_NORMAL = 0,                                                       //普通说话消息
    CHAT_TYPE_TEAM,                                                             //队聊消息
    CHAT_TYPE_SCENE,                                                            //场景消息
    CHAT_TYPE_TELL,                                                             //私聊消息
    CHAT_TYPE_SYSTEM,                                                           //系统消息
    CHAT_TYPE_CHANNEL,                                                          //自建聊天频道消息
    CHAT_TYPE_GUILD,                                                            //帮派消息
    CHAT_TYPE_MENPAI,                                                           //门派消息
    CHAT_TYPE_SELF,                                                             //仅客户端使用的消息

    CHAT_TYPE_NUMBER                                                            //聊天类型的最大值
};


#endif	/* CN_VICKY__CHATDEFINE_H */

