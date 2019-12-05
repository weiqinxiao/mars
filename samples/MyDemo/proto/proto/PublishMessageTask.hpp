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
#include "ProtoListener.h"

namespace mars {
    namespace stn {
        
        class PublishMessageTask : public ProtoTask {
        private:
            SendMessageCallBack *_sendMsgCallBack;
            
        private:
            long long _sentTime;
            
        public:
            PublishMessageTask(ConversationType type, const std::string &targetID);
            void encodeMessage(pbc_env *env, MessageTag tag, const std::string &objName, const std::string &pushContent, const std::string &pushData, const std::string &content);
            void decodeMessage(pbc_env *env, AutoBuffer &inBuffer);
            
            void setCallBack(SendMessageCallBack *callback) {
                _sendMsgCallBack = callback;
            }
            
            SendMessageCallBack *callback() {
                return _sendMsgCallBack;
            }
            
            long long getMessageSentTime() {
                return _sentTime;
            }
        };
    }
}
#endif /* PublishMessageTask_hpp */
