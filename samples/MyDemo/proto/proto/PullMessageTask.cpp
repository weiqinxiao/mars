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
        PullMessageTask::PullMessageTask(long lastSentTime, long lastRcvTime):ProtoTask(TaskID::TaskID_PullMsg, Topic_pullMsg) {
            cmdid = CmdID_PullMsg;
            lastRcvTime_ = lastRcvTime;
            lastSentTime_ = lastSentTime;
        }
        
//        message SyncRequestMsg  //客户端请求服务器同步消息
//        {
//            required int64 syncTime = 1;//同步时间戳
//            required bool ispolling = 2; //是否轮询
//            optional bool isweb=3;//web是否需要拉取离线
//            optional bool isPullSend=4;//是否需要拉取发件箱，false不要，true需要
//            optional bool isKeeping=5;//拉取消息confirm时是否删除，默认false删除，true不删除
//            optional int64 sendBoxSyncTime = 6;//发件箱同步时间戳
//        }
        void PullMessageTask::encodeMessage(pbc_env *env) {
            pbMsg_ = pbc_wmessage_new(env, "SyncRequestMsg");
            unsigned int low = lastRcvTime_ & 0xffffffff;
            unsigned int high = lastRcvTime_ >> 32;
            pbc_wmessage_integer(pbMsg_, "syncTime", low, high);
            pbc_wmessage_integer(pbMsg_, "ispolling", 0, 0);
            pbc_wmessage_integer(pbMsg_, "isPull", 1, 0);
            unsigned int low_send = lastSentTime_ & 0xffffffff;
            unsigned int high_send = lastSentTime_ >> 32;
            pbc_wmessage_integer(pbMsg_, "syncSend", low_send, high_send);
            struct pbc_slice slice;
            pbc_wmessage_buffer(pbMsg_, &slice);
            data_ = (unsigned char*)slice.buffer;
            dataLen_ = slice.len;
        }
        
        void PullMessageTask::decodeMessage(pbc_env *env, unsigned char* data, size_t dataLen) {
            pbc_slice slice;
            slice.buffer = data;
            slice.len = dataLen;
            pbc_rmessage *msg = pbc_rmessage_new(env, "HistoryMessagesO", &slice);
            unsigned int high = 0;
            unsigned int low = pbc_rmessage_integer(msg, "syncTime", 0, &high);
            lastRcvTime_ = ((int64_t)high << 32) + low;
            int msgCount = pbc_rmessage_size(msg, "list");
            finishPull_ = (1 == pbc_rmessage_integer(msg, "hasMsg", 0, NULL));
            
            for (int i = 0; i < msgCount; i++) {
                pbc_rmessage *pbMsg = pbc_rmessage_message(msg, "list", i);
                MessageContent *content = decodeMessageContent(pbMsg);
                msgList.push_back(content);
            }
            pbc_rmessage_delete(msg);
        }
    }
}
