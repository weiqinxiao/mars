//
//  ProtoClient.hpp
//  proto
//
//  Created by weiqinxiao on 2019/11/14.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef ProtoClient_hpp
#define ProtoClient_hpp

#include <stdio.h>
#include <string>
#include "ProtoListener.h"
#include "ProtoTask.hpp"

namespace mars {
    namespace stn {

        extern void initClient();
        
        extern void setEnviroment(const std::string &appKey, const std::string &appName, const std::string &deviceID, const std::string &manufacturer, const std::string &model, const std::string &osVersion);
        
        extern void unInitClient();
        
        extern void setConnectionListener(ConnectionStatusListener *listener);
        
        extern void setReceiveMessageListener(ReceiveMessageListener *listener);
        
        extern void connectServer(const std::string &userID, const std::string &token, const std::string &host, const uint16_t port);
        
        extern void sendMessage(ConversationType type, const std::string &targetID, const std::string & objName, const std::string &pushContent, const std::string &pushData, const std::string &msgContent, SendMessageCallBack *sendMsgCallBack);
    }
}

#endif /* ProtoClient_hpp */
