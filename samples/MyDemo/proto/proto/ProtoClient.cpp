//
//  ProtoClient.cpp
//  proto
//
//  Created by weiqinxiao on 2019/11/14.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "ProtoClient.hpp"
#include "mars/stn/stn_logic.h"
#include "mars/baseevent/base_logic.h"
#include "mars/comm/xlogger/xlogger.h"
#include "mars/app/app_logic.h"
#include "mars/comm/assert/__assert.h"

#include "StnCallBack.hpp"
#include "AppCallBack.h"
#include "MessageService.hpp"

namespace mars {
    namespace stn {
        bool _gInitialized = false;
        static std::string _gSDKVersion = "1.0.0";
        
        void setEnviroment(const std::string &appKey, const std::string &appName, const std::string &deviceID, const std::string &manufacturer, const std::string &model, const std::string &osVersion) {
            ASSERT(_gInitialized);
            
            std::string platform = "iOS";
            #if defined(__ANDROID__)
                platform = "Android";
            #endif
            std::string ua = "";
            ua.append(platform)
                .append("-")
                .append(manufacturer)
                .append("|")
                .append(model)
                .append("|")
                .append(osVersion)
                .append("-")
                .append(_gSDKVersion);
            mars::stn::ConnectivityLogic::Instance()->setEnviroment(appKey, appName, deviceID, ua);
        }
        
        void initClient() {
            if (_gInitialized) {
                xerror2("SDK had initialized!");
                return;
            }
            mars::stn::StnCallBack *stnCallBack = mars::stn::StnCallBack::Instance();
            mars::stn::ConnectivityListener *listener = mars::stn::ConnectivityLogic::Instance();
            stnCallBack->setConnectivityListener(listener);
            mars::stn::SetCallback(stnCallBack);
            mars::app::SetCallback(mars::app::AppCallBack::Instance());
            mars::baseevent::OnCreate();
            mars::stn::MessageService::Instance();
            
            _gInitialized = true;
            //TODO DB
        }
        
        void unInitClient() {
            ASSERT(_gInitialized);
            mars::baseevent::OnDestroy();
            _gInitialized = false;
            //TODO DB
        }
        
        void setConnectionListener(ConnectionStatusListener *listener) {
            mars::stn::ConnectivityLogic::Instance()->setConnectionListener(listener);
        }
        
        void setReceiveMessageListener(ReceiveMessageListener *listener) {
            mars::stn::MessageService::Instance()->setReceiveMessageListener(listener);
        }
        
        void connectServer(const std::string &userID, const std::string &token, const std::string &host, const uint16_t port) {
            ASSERT(_gInitialized);
            xinfo2(TSF"connectServer: %_, %_", host, port);
            mars::stn::ConnectivityLogic::Instance()->connectServer(userID, token, host, port);
        }
        
        void sendMessage(ConversationType type, const std::string &targetID, const std::string &objName, const std::string &pushContent, const std::string &pushData, const std::string &msgContent, SendMessageCallBack *sendMsgCallBack) {
            mars::stn::MessageService::Instance()->sendMessage(type, targetID, objName, pushContent, pushData, msgContent, sendMsgCallBack);
        }
    }
}
