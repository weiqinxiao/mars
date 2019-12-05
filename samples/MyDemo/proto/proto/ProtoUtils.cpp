//
//  ProtoUtils.cpp
//  proto
//
//  Created by weiqinxiao on 2019/12/4.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "ProtoUtils.hpp"
#include <string>

namespace mars {
    namespace stn {
        void ProtoUtils::writeUTF8(const char* str, AutoBuffer &out) {
            size_t len = strlen(str);
            unsigned short w = (unsigned short)((len>>8) | ((len&0xFF)<<8));
            out.Write(&w, 2);
            out.Write(str, len);
        }
        
        void ProtoUtils::writeLong(long v, AutoBuffer &out) {
            unsigned char flag[8] = {0};
            flag[0] = (unsigned char)(v >> 56);
            flag[1] = (unsigned char)(v >> 48);
            flag[2] = (unsigned char)(v >> 40);
            flag[3] = (unsigned char)(v >> 32);
            flag[4] = (unsigned char)(v >> 24);
            flag[5] = (unsigned char)(v >> 16);
            flag[6] = (unsigned char)(v >>  8);
            flag[7] = (unsigned char)(v >>  0);
            out.Write(flag, 8);
        }
        
        long long ProtoUtils::readLong(AutoBuffer &in) {
            unsigned char flag[8] = {0};
            in.Read(flag, 8);
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
        
        int ProtoUtils::readInt(AutoBuffer &in) {
            int flag[4] = {0};
            in.Read(flag, 4);
            return ((flag[0] << 24) + (flag[1] << 16) + (flag[2] << 8) + (flag[3] << 0));
        }
        
        unsigned char ProtoUtils::readByte(AutoBuffer &in) {
            unsigned char flag = 0;
            in.Read(&flag, 1);
            return flag;
        }
        
        std::string ProtoUtils::readUTF8(AutoBuffer &in) {
            unsigned char f = readByte(in);
            unsigned char s = readByte(in);
            unsigned short sz = (f>>8) | s;
            char* utf = (char*)malloc(sz + 1);
            memset(utf, 0, sz + 1);
            in.Read(utf, sz);
            std::string result = utf;
            delete utf;
            return result;
        }
    }
}
