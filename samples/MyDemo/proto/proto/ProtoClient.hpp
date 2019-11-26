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

namespace mars {
    namespace stn {

        extern void initClient();
        
        extern void setEnviroment(const std::string &appKey, const std::string &appName, const std::string &deviceID, const std::string &manufacturer, const std::string &model, const std::string &osVersion);
        
        extern void unInitClient();
        
        extern void setConnectionListener(ConnectionStatusListener *listener);
        
        extern void connectServer(const std::string &userID, const std::string &token, const std::string &host, const uint16_t port);
    }
}

#endif /* ProtoClient_hpp */
