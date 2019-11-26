//
//  Connect.cpp
//  protocol
//
//  Created by weiqinxiao on 2019/11/7.
//  Copyright © 2019 club.linkyou. All rights reserved.
//
#include <sys/time.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <iterator>
#include <fstream>
#include <sstream>

#include "NetCallBack.h"

std::string gUserID;
std::string gToken;
std::string gHost;
uint16_t gPort;

namespace LinkClient {
//    namespace you {

        bool initSDK(const std::string &userID) {
            printf("initSDK: %s", userID.c_str());
            gUserID = userID;
            printf("initSDK");
            
            mars::stn::SetCallback(NetCallBack::instance());
            return true;
        };

//        bool connect(const std::string &host, const uint16_t port) {
//            gHost = host;
//            gPort = port;
//
//            std::vector<std::string> _hosts;
//            _hosts.push_back(host);
//            std::vector<uint16_t> _ports;
//            _ports.push_back(port);
//            NetSource::SetLongLink(_hosts, _ports, NULL);
//
//            mars::stn::MakesureLonglinkConnected();
//
//            return true;
//        }

//    }
}
