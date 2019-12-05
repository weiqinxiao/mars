//
//  ProtoUtils.hpp
//  proto
//
//  Created by weiqinxiao on 2019/12/4.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef ProtoUtils_hpp
#define ProtoUtils_hpp

#include <stdio.h>
#include "mars/comm/autobuffer.h"
#include <string>

namespace mars {
    namespace stn {
        class ProtoUtils {
        public:
            static void writeUTF8(const char* str, AutoBuffer &out);
            static void writeLong(long v, AutoBuffer &out);
            static long long readLong(AutoBuffer &in);
            static int readInt(AutoBuffer &in);
            static unsigned char readByte(AutoBuffer &in);
            static std::string readUTF8(AutoBuffer &in);
        };
    }
}
#endif /* ProtoUtils_hpp */
