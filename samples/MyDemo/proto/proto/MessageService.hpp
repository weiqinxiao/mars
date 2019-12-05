//
//  MessageService.hpp
//  proto
//
//  Created by weiqinxiao on 2019/12/4.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef MessageService_hpp
#define MessageService_hpp

#include <stdio.h>
#include "ProtoListener.h"

namespace mars {
    namespace stn {
        
        class MessageService {
        private:
            ReceiveMessageListener* _rcvMsgListener;
            
        private:
            unsigned short _pullMsgID;
            long _lastSentTime;
            long _lastReceiveTime;
            MessageService();
            
        public:
            static MessageService *Instance();
            
            void setReceiveMessageListener(ReceiveMessageListener *listener) {
                _rcvMsgListener = listener;
            };
            
            ReceiveMessageListener* getReceiveMessageListener() {
                return _rcvMsgListener;
            };
            
            void syncMessage();
            void updateSyncTime(long sentTime, long receiveTime);

            void sendMessage(ConversationType type, const std::string &targetID, const std::string &objName, const std::string &pushContent, const std::string &pushData, const std::string &msgContent, SendMessageCallBack *sendMsgCallBack);
        };
    }
}


#endif /* MessageService_hpp */
