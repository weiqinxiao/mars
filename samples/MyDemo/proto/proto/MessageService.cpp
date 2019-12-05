//
//  MessageService.cpp
//  proto
//
//  Created by weiqinxiao on 2019/12/4.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "MessageService.hpp"
#include "PublishMessageTask.hpp"
#include "ConnectivityLogic.hpp"
#include "PullMessageTask.hpp"

namespace mars {
    namespace stn {
        static MessageService *instance_;
        
        MessageService *MessageService::Instance() {
            if (instance_ == NULL) {
                instance_ = new MessageService();
            }
            return instance_;
        }
        
        MessageService::MessageService() {
            _pullMsgID = 1;
            _lastReceiveTime = 0;
            _lastSentTime = 0;
        }
        
        void MessageService::sendMessage(ConversationType type, const std::string &targetID, const std::string &objName, const std::string &pushContent, const std::string &pushData, const std::string &msgContent, SendMessageCallBack *sendMsgCallBack) {
            PublishMessageTask* task = new PublishMessageTask(type, targetID);
            task->setCallBack(sendMsgCallBack);
            task->encodeMessage(ConnectivityLogic::Instance()->getPBEnv(), MessageTag::Persistent, objName, pushContent, pushData, msgContent);
            mars::stn::StartTask(*task);
            xinfo2(TSF"start publish task: %_, %_, obj = %_", type, targetID, objName);
        }
        
        void MessageService::syncMessage() {
            xinfo2(TSF"start pull message task");
            PullMessageTask *task = new PullMessageTask(_lastSentTime, _lastReceiveTime, _pullMsgID);
            mars::stn::StartTask(*task);
            _pullMsgID++;
            if (_pullMsgID == 65535) _pullMsgID = 1;
        }
        
        
        void MessageService::updateSyncTime(long sentTime, long receiveTime) {
            xinfo2(TSF"[wei] sentTime = %_, receiveTime = %_", sentTime, receiveTime);
            _lastReceiveTime = receiveTime;
            _lastSentTime = sentTime;
        }
    }
}
