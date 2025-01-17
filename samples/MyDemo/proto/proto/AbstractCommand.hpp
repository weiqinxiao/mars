//
//  AbstractCommand.hpp
//  proto
//
//  Created by weiqinxiao on 2019/11/15.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef AbstractCommand_hpp
#define AbstractCommand_hpp

#include <stdio.h>
#include <string>
#include "mars/comm/autobuffer.h"
#include "mars/comm/xlogger/xlogger.h"

namespace mars {
    namespace stn {
        static unsigned char C_KEY[] = {106,79,19,35,14,41,20,121};
        
        typedef enum _CmdID {
            CmdID_Connect = 10,
            CmdID_Pull = 11,
            CmdID_PullConfirm = 12,
            CmdID_Publish = 13,
            CmdID_Notify = 14,
        } CmdID;


        typedef enum _MqttCmd {
            CONNECT     = 1,
            CONNACK     = 2,
            PUBLISH     = 3,
            PUBACK      = 4,
            QUERY       = 5,
            QUERYACK    = 6,
            QUERYCON    = 7,
            PCONNECT    = 8,
            PCONNECTACK = 9,
            UNSUBSCRIBE = 10,
            UNSUBACK    = 11,
            PINGREQ     = 12,
            PINGRESP    = 13,
            DISCONNECT  = 14,
        } MqttCmd;
        
        typedef enum _CmdQos {
            QOS_AT_MOST_ONCE = 0,
            QOS_AT_LEAST_ONCE = 1,
            QOS_EXACTLY_ONCE = 2,
            QOS_DEFAULT = 3,
        } CmdQos;
        
        typedef enum _CmdRetain {
            RETAIN_NO = 0,
            RETAIN_YES = 1,
        } CmdRetain;
        
        typedef enum _CmdFlag {
            FLAG_APPID = 0x80,
            FLAG_TOKEN = 0x40,
            FLAG_RETAIN = 0x20,
            FLAG_APPNAME = 0x04,
            FLAG_RECONNECT = 0x02,
            FLAG_CLIENTIP = 0x01,
        } CmdFlag;
        
        struct CmdHeader {
            MqttCmd mqttcmd;
            CmdRetain retain;
            CmdQos qos;
            
            CmdHeader(unsigned char flag) {
                mqttcmd =  MqttCmd((flag>>4)&0xf);
                qos = CmdQos((flag&0x6)>>1);
                retain = CmdRetain(flag&0x1);
            }
            
            unsigned char encode() {
                unsigned char flag = 0;
                flag |= mqttcmd << 4;
                flag |= retain;
                flag |= qos << 1;
                return flag;
            }
            
            CmdHeader(){}
        };
        
        class AbstractCommand {
        private:
            AutoBuffer payload_;

        protected:
            CmdHeader header_;
            const char* protoID = "MQTT";
            const char protoVersion = 0x03;
            const unsigned short protoKeepAlive = 0xA;
            
        private:
            int decodedLength(unsigned char headerCode, unsigned char checksum, unsigned int* msgLen);

        protected:
            void writeUTF8(const char* str);
            template<class T> void write(const T& _val) {
                payload_.Write(_val);
            };
            void write(const void* ptr, size_t len) {
                payload_.Write(ptr, len);
            };
            void wirteLong(long long v);
            void seekStart() {
                payload_.Seek(0, AutoBuffer::ESeekStart);
            }
            unsigned int readInt();
            std::string readUTF8();
            unsigned char readByte();
            unsigned long long readLong();
            unsigned char* readData(unsigned int msgLen);
            size_t payloadLength() {return payload_.Length();};
            virtual size_t encodeMessage() {return 0;};
            virtual void decodeMessage(unsigned int msgLen) {};
            
        public:
            AbstractCommand(MqttCmd cmdid, CmdQos qos = QOS_AT_MOST_ONCE, CmdRetain retain = RETAIN_NO);
            AbstractCommand(CmdHeader header);
            ~AbstractCommand(){};

            void encode(AutoBuffer& pack);
            void decode(const AutoBuffer& pack);
            
            unsigned char* getPayload() {
                return (unsigned char*)payload_.PosPtr();
            }
            
            size_t getPayloadLength() {
                return payload_.PosLength();
            }
        };
    }
}

#endif /* AbstractCommand_hpp */
