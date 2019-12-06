//
//  AbstractTask.cpp
//  proto
//
//  Created by weiqinxiao on 2019/11/27.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "ProtoTask.hpp"
#include "mars/comm/time_utils.h"

namespace mars {
    namespace stn {
        ProtoTask::ProtoTask(TaskID taskID, std::string topic) : Task(taskID), topic_(topic), signature_(0xff) {
            user_context = this;
            channel_select = Task::kChannelLong;
            limit_flow = false;
            limit_frequency = false;
            need_authed = false;
        }
        
        ProtoTask::ProtoTask(TaskID taskID, unsigned short msgID) {
            ProtoTask(taskID, "");
            msgID_ = msgID;
        }
        
        MessageContent ProtoTask::decodeMessageContent(pbc_rmessage *message) {
            MessageContent msgContent;
            msgContent.senderID = pbc_rmessage_string(message, "fromUserId", 0, NULL);
            unsigned int conversationType = pbc_rmessage_integer(message, "type", 0, NULL);
            if (conversationType == GROUP ||
                conversationType == CHATROOM) {
                msgContent.targetID = pbc_rmessage_string(message, "groupId", 0, NULL);
            } else {
                msgContent.targetID = msgContent.senderID;
            }
            msgContent.objName = pbc_rmessage_string(message, "classname", 0, NULL);
            msgContent.content = pbc_rmessage_string(message, "content", 0, NULL);
            unsigned int high = 0;
            unsigned int low = pbc_rmessage_integer(message, "dataTime", 0, &high);
            msgContent.sentTime = ((int64_t)high << 32) + low;
            msgContent.sendStatus = 0;
            msgContent.uid = pbc_rmessage_string(message, "msgId", 0, NULL);
            msgContent.direction = MessageDirection(pbc_rmessage_integer(message, "direction", 0, NULL));
            msgContent.rcvTime = timeMs();
            
            int state = pbc_rmessage_integer(message, "status", 0, NULL);
            if (state & 0x1) {//更新拉取时间
                if (msgContent.direction == MessageDirection::Send) {
                    lastSentTime_ = msgContent.sentTime;
                }
            }
            xinfo2(TSF"[wei] decoded msg: %_", msgContent.objName);
            return msgContent;
        }
    }
}
