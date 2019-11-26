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
        
        unsigned char AbstractCommand::encodedLength(size_t code, size_t len, unsigned int* count, AutoBuffer &pack) {
            (*count) = 1;
            pack.Write((unsigned char)0);
            do {
                unsigned char b = (unsigned char) (len & 0x7F);
                len >>= 7;
                if (len > 0) {
                    b |= 0x80;
                    (*count)++;
                }
                code = code ^ b;
                pack.Write(b);
            } while (len > 0);
            return code;
        }
        
        unsigned long AbstractCommand::decodedLength() {
            unsigned char headerCode = readByte();
            unsigned char checksum = readByte();
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
            } while ((digit&0x80)>0);
            return 2 + lengthSize;
        }
        
        unsigned char checkSum(unsigned char ch, unsigned char* pLenBuff, unsigned int nLength)
        {
            for (unsigned int i = 0; i < nLength; i++)
                ch ^= pLenBuff[i];
            return ch;
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
        
        AbstractCommand::AbstractCommand(CmdID cmdid, CmdQos qos, CmdRetain retain) {
            header_.cmdid = cmdid;
            header_.qos = qos;
            header_.retain = retain;
        }
        
        AbstractCommand::AbstractCommand(CmdHeader header) {
            header_.cmdid = header.cmdid;
            header_.qos = header.qos;
            header_.retain = header.retain;
        }

        void AbstractCommand::encode(AutoBuffer &pack) {
            size_t len = encodeMessage();
            unsigned int count = 0;
            unsigned char headerCode = header_.encode();
            pack.Write(headerCode);
            unsigned char encodedCode = encodedLength(headerCode, len, &count, pack);
            unsigned char checksum = checkSum(headerCode, (unsigned char*)&encodedCode, count);
            ((unsigned char *)pack.Ptr())[1] = checksum;
            pack.Write(payload_);
            obfuscation((unsigned char*)pack.Ptr(), 2 + count, pack.Length());
        }
        
        void AbstractCommand::decode(const AutoBuffer& pack) {
            write(pack);
            seekStart();
            int len = decodedLength();
            obfuscation((unsigned char*)payload_.Ptr(), len, payloadLength());
            decodeMessage();
        }

        
        void AbstractCommand::writeUTF8(const char* str) {
            size_t len = strlen(str);
            unsigned short w = (unsigned short)((len>>8) | ((len&0xFF)<<8));
            payload_.Write(&w, 2);
            payload_.Write(str, len);
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
        
        
    }
}
