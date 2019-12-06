//
//  PublishMessageTask.cpp
//  proto
//
//  Created by weiqinxiao on 2019/12/4.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "PublishMessageTask.hpp"
#include "MessageTopic.h"

namespace mars {
    namespace stn {
        PublishMessageTask::PublishMessageTask(ConversationType type, const std::string &targetID) : ProtoTask(TaskID::TaskID_Publish, "") {
            switch (type) {
                case PRIVATE:
                {
                    topic_ = Topic_prvtMsg;
                    break;
                }
                case GROUP:
                {
                    topic_ = Topic_grpMsg;
                    break;
                }
                case CHATROOM:
                {
                    topic_ = Topic_chrMsg;
                    break;
                }
                default:
                    break;
            }
            targetID_ = targetID;
            cmdid = CmdID_Publish;
            _sendMsgCallBack = NULL;
            _sentTime = 0;
        }
        
        void PublishMessageTask::encodeMessage(pbc_env *env, MessageTag tag, const std::string &objName, const std::string &pushContent, const std::string &pushData, const std::string &content) {
            unsigned char lsb = msgID_ & 0xFF;
            unsigned char msb = (msgID_ & 0xFF00) >> 8;
            ProtoUtils::writeLong(signature_, body_);
            ProtoUtils::writeUTF8(topic_.c_str(), body_);
            ProtoUtils::writeUTF8(targetID_.c_str(), body_);
            body_.Write(msb);
            body_.Write(lsb);
            
            pbc_wmessage *msg = pbc_wmessage_new(env, "UpStreamMessage");
            pbc_wmessage_string(msg, "classname", objName.c_str(), (int)objName.size());
            pbc_wmessage_integer(msg, "sessionId", (unsigned int)tag, 0);
            pbc_wmessage_string(msg, "content", content.c_str(), (int)content.size());
            if (!pushContent.empty()) {
                pbc_wmessage_string(msg, "pushText", pushContent.c_str(), (int)pushContent.size());
            }
            if (!pushData.empty()) {
                pbc_wmessage_string(msg, "appData", pushData.c_str(), (int)pushData.size());
            }
            pbc_slice slice;
            pbc_wmessage_buffer(msg, &slice);
            body_.Write(slice.buffer, slice.len);
            pbc_wmessage_delete(msg);
            xinfo2(TSF"[wei] encode message: topic = %_, body = %_", topic_, body_.Length());
        }
        
        void PublishMessageTask::decodeMessage(pbc_env *env, AutoBuffer &inBuffer) {
            msgID_ = ProtoUtils::readByte(inBuffer) * 0x100 + ProtoUtils::readByte(inBuffer);
            date_ = ProtoUtils::readInt(inBuffer);
            status_ = ProtoUtils::readByte(inBuffer) * 0x100 + ProtoUtils::readByte(inBuffer);
            int ms = ProtoUtils::readByte(inBuffer) * 0x100 + ProtoUtils::readByte(inBuffer);
            uuid_ = ProtoUtils::readUTF8(inBuffer);
            _sentTime = ((long long) date_) * 1000 + ms;
        }
    }
}
