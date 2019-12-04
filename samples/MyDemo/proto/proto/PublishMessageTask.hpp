//
//  PublishMessageTask.hpp
//  proto
//
//  Created by weiqinxiao on 2019/12/4.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef PublishMessageTask_hpp
#define PublishMessageTask_hpp

#include <stdio.h>
#include "ProtoTask.hpp"

namespace mars {
    namespace stn {
        class PublishMessageTask : public ProtoTask {
            
        public:
            PublishMessageTask(std::string topic);
            void encodeMessage(pbc_env *env){};
            void decodeMessage(pbc_env *env, unsigned char* data, size_t dataLen);
        };
    }
}
#endif /* PublishMessageTask_hpp */
