//
//  AbstractCommand.cpp
//  proto
//
//  Created by weiqinxiao on 2019/11/15.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "AbstractCommand.hpp"
#include "ProtoUtils.hpp"

namespace mars {
    namespace stn {
        
        unsigned char calculateCheckSum(size_t code, size_t msgLen, unsigned int* count, AutoBuffer &pack) {
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
            ProtoUtils::writeUTF8(str, payload_);
        }
        
        void AbstractCommand::wirteLong(long long v) {
            ProtoUtils::writeLong(v, payload_);
        }

        std::string AbstractCommand::readUTF8() {
            return ProtoUtils::readUTF8(payload_);
        }
        
        unsigned char AbstractCommand::readByte() {
            return ProtoUtils::readByte(payload_);
        }
        
        unsigned long long AbstractCommand::readLong() {
            return ProtoUtils::readLong(payload_);
        }
        
        unsigned int AbstractCommand::readInt() {
            return ProtoUtils::readInt(payload_);
        }
        
        unsigned char* AbstractCommand::readData(unsigned int msgLen) {
            unsigned char* data = (unsigned char*)malloc(msgLen);
            memset(data, 0, msgLen);
            payload_.Read(data, msgLen);
            return data;
        }
    }
}
