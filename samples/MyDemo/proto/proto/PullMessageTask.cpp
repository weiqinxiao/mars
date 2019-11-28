//
//  PullMessageTask.cpp
//  proto
//
//  Created by weiqinxiao on 2019/11/26.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "PullMessageTask.hpp"
#include "PublishCommand.hpp"
#include "MessageTopic.h"
#include "pbc/pbc.h"

namespace mars {
    namespace stn {
        PullMessageTask::PullMessageTask():ProtoTask(TaskID::TaskID_PullMsg) {
            cmdid = MqttCmd::PUBLISH;
        }
        
        void PullMessageTask::encodeMessage(pbc_env *env, AutoBuffer &pack) {
            pbMsg_ = pbc_wmessage_new(env, "SyncRequestMsg");
            long syncTime = 0;
            long sendTime = 0;
            unsigned int low = syncTime & 0xffffffff;
            unsigned int high = syncTime >> 32;
            pbc_wmessage_integer(pbMsg_, "syncTime", low, high);
            pbc_wmessage_integer(pbMsg_, "ispolling", 0, 0);
            pbc_wmessage_integer(pbMsg_, "isPull", 1, 0);
            unsigned int low_send = sendTime & 0xffffffff;
            unsigned int high_send = sendTime >> 32;
            pbc_wmessage_integer(pbMsg_, "syncSend", low_send, high_send);
            struct pbc_slice slice;
            pbc_wmessage_buffer(pbMsg_, &slice);
            cmd_ = new PublishCommand(Topic_pullMsg, (const unsigned char*)slice.buffer, (unsigned long)slice.len);
            cmd_->encode(pack);
        }
    }
}
