//
//  Utils.cpp
//  proto
//
//  Created by weiqinxiao on 2019/11/22.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include <stdio.h>
#include "mars/comm/autobuffer.h"

namespace mars {
    namespace stn {
        static void encode2UTF8(const char* str, AutoBuffer &buffer) {
            size_t len = strlen(str);
            unsigned short w = (unsigned short)((len>>8) | ((len&0xFF)<<8));
            buffer.Write(&w, 2);
            buffer.Write(str, len);
        }
    }
}
