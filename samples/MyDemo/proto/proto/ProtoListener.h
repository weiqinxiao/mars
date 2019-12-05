//
//  ProtoListener.h
//  proto
//
//  Created by weiqinxiao on 2019/11/25.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef ProtoListener_h
#define ProtoListener_h
#include "mars/comm/xlogger/xlogger.h"
#include "ProtoTask.hpp"
#include <list>

namespace mars {
    namespace stn {
        class ConnectionStatusListener {
        public:
            virtual ~ConnectionStatusListener(){
                xerror2(TSF"free ConnectionStatusListener");
            }
            
            virtual void onConnectionStatusChanged(int state, const char* userID) = 0;
        };
        
        class ReceiveMessageListener {
        public:
            virtual void onReceivedMessages(std::list<MessageContent> msgList) = 0;
        };
        
        class SendMessageCallBack {
        public:
            virtual void onSuccess(std::string msgUID) = 0;
            virtual void onError(int code) = 0;
        };
    }
}
#endif /* ProtoListener_h */
