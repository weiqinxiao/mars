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
        PullMessageTask::PullMessageTask(long lastSentTime, long lastRcvTime, unsigned short msgID):ProtoTask(TaskID::TaskID_PullMsg, Topic_pullMsg) {
            cmdid = CmdID_PullMsg;
            lastRcvTime_ = lastRcvTime;
            lastSentTime_ = lastSentTime;
            msgID_ = msgID;
        }
        
        void PullMessageTask::encodeMessage(pbc_env *env) {
            unsigned short lsb = msgID_ & 0xFF;
            unsigned short msb = (msgID_ & 0xFF00) >> 8;
            ProtoUtils::writeLong(signature_, body_);
            ProtoUtils::writeUTF8(topic_.c_str(), body_);
            body_.Write(msb);
            body_.Write(lsb);
            
            pbc_wmessage *msg = pbc_wmessage_new(env, "SyncRequestMsg");
            unsigned int low = lastRcvTime_ & 0xffffffff;
            unsigned int high = lastRcvTime_ >> 32;
            pbc_wmessage_integer(msg, "syncTime", low, high);
            pbc_wmessage_integer(msg, "ispolling", 0, 0);
            pbc_wmessage_integer(msg, "isPull", 1, 0);
            unsigned int low_send = lastSentTime_ & 0xffffffff;
            unsigned int high_send = lastSentTime_ >> 32;
            pbc_wmessage_integer(msg, "syncSend", low_send, high_send);
            struct pbc_slice slice;
            pbc_wmessage_buffer(msg, &slice);
            body_.Write(slice.buffer, slice.len);
            pbc_wmessage_delete(msg);
        }
        
        void PullMessageTask::decodeMessage(pbc_env *env, AutoBuffer &inBuffer) {
            msgID_ = ProtoUtils::readByte(inBuffer) * 0x100 + ProtoUtils::readByte(inBuffer);
            date_ = ProtoUtils::readInt(inBuffer);
            status_ = ProtoUtils::readByte(inBuffer) * 0x100 + ProtoUtils::readByte(inBuffer);
            
            pbc_slice slice;
            slice.buffer = inBuffer.PosPtr();
            slice.len = inBuffer.PosLength();
            pbc_rmessage *msg = pbc_rmessage_new(env, "HistoryMessagesO", &slice);
            unsigned int high = 0;
            unsigned int low = pbc_rmessage_integer(msg, "syncTime", 0, &high);
            lastRcvTime_ = ((int64_t)high << 32) + low;
            int msgCount = pbc_rmessage_size(msg, "list");

            finishPull_ = (1 == pbc_rmessage_integer(msg, "hasMsg", 0, NULL));
            
            for (int i = 0; i < msgCount; i++) {
                pbc_rmessage *pbMsg = pbc_rmessage_message(msg, "list", i);
                MessageContent content = decodeMessageContent(pbMsg);
                msgList.push_back(content);
            }
            pbc_rmessage_delete(msg);
        }
    }
}
