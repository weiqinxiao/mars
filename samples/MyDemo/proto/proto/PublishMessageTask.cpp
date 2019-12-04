//
//  PublishMessageTask.cpp
//  proto
//
//  Created by weiqinxiao on 2019/12/4.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "PublishMessageTask.hpp"

namespace mars {
    namespace stn {
        PublishMessageTask::PublishMessageTask(std::string topic):ProtoTask(TaskID::TaskID_PublishMsg, topic) {
            
        }
        
        void PublishMessageTask::decodeMessage(pbc_env *env, unsigned char* data, size_t dataLen) {
            
        }
    }
}
