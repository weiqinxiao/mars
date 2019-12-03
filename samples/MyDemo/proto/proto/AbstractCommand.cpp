//
//  AbstractCommand.cpp
//  proto
//
//  Created by weiqinxiao on 2019/11/15.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "AbstractCommand.hpp"

namespace mars {
    namespace stn {
        
        unsigned char AbstractCommand::calculateCheckSum(size_t code, size_t msgLen, unsigned int* count, AutoBuffer &pack) {
            (*count) = 0;
            pack.Write((unsigned char)0);
            do {
                (*count)++;
                unsigned char b = (unsigned char) (msgLen & 0x7F);
                msgLen >>= 7;
                if (msgLen > 0) {
                    b |= 0x80;
                }
                code = code ^ b;
                pack.Write(b);
            } while (msgLen > 0);
            return code;
        }
        
        int AbstractCommand::decodedLength(unsigned char headerCode, unsigned char checksum, unsigned int* msgLen) {
            int digit;
            int msgLength = 0;
            int multiplier = 1;
            int lengthSize = 0;
            int code = headerCode;
            do {
                lengthSize++;
                digit = readByte();
                code = code ^ digit;
                msgLength += (digit & 0x7f) * multiplier;
                multiplier *= 128;
            } while ((digit&0x80) > 0);
            (*msgLen) = msgLength;
            return 2 + lengthSize;
        }

        void obfuscation(unsigned char* data, int start, size_t dataLen) {
            int b = 0;
            for (int i = start; i < dataLen; i += 8) {
                b = i;
                for (int j = 0; j < 8 && b < dataLen; j++, b++) {
                    data[b] = (data[b] ^ C_KEY[j]);
                }
            }
        }
        
        AbstractCommand::AbstractCommand(MqttCmd cmdid, CmdQos qos, CmdRetain retain) {
            header_.mqttcmd = cmdid;
            header_.qos = qos;
            header_.retain = retain;
        }
        
        AbstractCommand::AbstractCommand(CmdHeader header) {
            header_.mqttcmd = header.mqttcmd;
            header_.qos = header.qos;
            header_.retain = header.retain;
        }

        void AbstractCommand::encode(AutoBuffer &pack) {
            size_t msgLen = encodeMessage();
            unsigned int count = 0;
            unsigned char headerCode = header_.encode();
            pack.Write(headerCode);
            unsigned char checkSum = calculateCheckSum(headerCode, msgLen, &count, pack);
            ((unsigned char *)pack.Ptr())[1] = checkSum;
            pack.Write(payload_);
            obfuscation((unsigned char*)pack.Ptr(), 2 + count, pack.Length());
        }
        
        void AbstractCommand::decode(const AutoBuffer& pack) {
            write(pack);
            seekStart();
            unsigned char headerCode = readByte();
            unsigned char checksum = readByte();
            unsigned int msgLen = 0;
            int lengthSize = decodedLength(headerCode, checksum, &msgLen);
            xinfo2(TSF"decoded: lengthSize = %_, msgLen = %_, total = %_", lengthSize, msgLen, payloadLength());
            obfuscation((unsigned char*)payload_.Ptr(), lengthSize, payloadLength());
            decodeMessage(msgLen);
        }

        
        void AbstractCommand::writeUTF8(const char* str) {
            size_t len = strlen(str);
            unsigned short w = (unsigned short)((len>>8) | ((len&0xFF)<<8));
            payload_.Write(&w, 2);
            payload_.Write(str, len);
        }
        
        void AbstractCommand::wirteLong(long long v) {
            unsigned char flag[8] = {0};
            flag[0] = (unsigned char)(v >> 56);
            flag[1] = (unsigned char)(v >> 48);
            flag[2] = (unsigned char)(v >> 40);
            flag[3] = (unsigned char)(v >> 32);
            flag[4] = (unsigned char)(v >> 24);
            flag[5] = (unsigned char)(v >> 16);
            flag[6] = (unsigned char)(v >>  8);
            flag[7] = (unsigned char)(v >>  0);
            payload_.Write(flag, 8);
        }

        char* AbstractCommand::readUTF8() {
            unsigned char f = readByte();
            unsigned char s = readByte();
            unsigned short sz = (f>>8) | s;
            char* utf = (char*)malloc(sz + 1);
            memset(utf, 0, sz + 1);
            payload_.Read(utf, sz);
            return utf;
        }
        
        unsigned char AbstractCommand::readByte() {
            unsigned char flag = 0;
            payload_.Read(&flag, 1);
            return flag;
        }
        
        unsigned long long AbstractCommand::readLong() {
            unsigned char flag[8] = {0};
            payload_.Read(flag, 8);
            size_t sz = (((long)flag[0] << 56) +
             ((long)(flag[1] & 255) << 48) +
             ((long)(flag[2] & 255) << 40) +
             ((long)(flag[3] & 255) << 32) +
             ((long)(flag[4] & 255) << 24) +
             ((flag[5] & 255) << 16) +
             ((flag[6] & 255) <<  8) +
             ((flag[7] & 255) <<  0));
            
            return sz;
        }
        
        unsigned int AbstractCommand::readInt() {
            int flag[4] = {0};
            payload_.Read(flag, 4);
            return ((flag[0] << 24) + (flag[1] << 16) + (flag[2] << 8) + (flag[3] << 0));
        }
        
        unsigned char* AbstractCommand::readData(unsigned int msgLen) {
            unsigned char* data = (unsigned char*)malloc(msgLen);
            memset(data, 0, msgLen);
            payload_.Read(data, msgLen);
            return data;
        }
    }
}
