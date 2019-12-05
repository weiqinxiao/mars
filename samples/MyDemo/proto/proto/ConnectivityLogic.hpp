//
//  ConnectivityLogic.hpp
//  proto
//
//  Created by weiqinxiao on 2019/11/15.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef ConnectivityLogic_hpp
#define ConnectivityLogic_hpp

#include <stdio.h>
#include <string>
#include "mars/stn/stn.h"
#include "mars/stn/stn_logic.h"
#include "mars/baseevent/base_logic.h"
#include "mars/comm/xlogger/xlogger.h"
#include "mars/app/app_logic.h"
#include "mars/comm/autobuffer.h"
#include "ConnectCommand.hpp"
#include "ProtoListener.h"
#include "pbc/pbc.h"


namespace mars {
    namespace stn {
        
        enum ConnectionState {
            Connecting,
            Connected,
            Unavialable,
            Disconnected,
            TokenExpired,
            TokenInvalid,
            UserBlocked,
            AppBlocked,
            Redirected,
            AppKeyReject,
        };
        
        class ConnectivityListener {

        public:
            virtual void onConnectionStatusChanged(int state) = 0;
        };
        
        class ConnectivityLogic : public ConnectivityListener {
        private:
            ConnectionStatusListener *connectionListener_;
            
        private:
            ConnectionState connectionState_;
            std::string appKey_;
            std::string appName_;
            std::string deviceID_;
            std::string userID_;
            std::string userAgent_;
            std::string token_;
            std::string host_;
            uint16_t port_;
            pbc_env *pbEnv_;
            
        public:
            static ConnectivityLogic *Instance();
            static void Release();
            
            ConnectivityLogic();
            
            ~ConnectivityLogic() {
                connectionListener_ = NULL;
            }
            
            void setEnviroment(const std::string &appKey, const std::string &appName, const std::string &deviceID, const std::string &userAgent) {
                appKey_ = appKey;
                appName_ = appName;
                deviceID_ = deviceID;
                userAgent_ = userAgent;
            }
            
            void setConnectionListener(ConnectionStatusListener *listener) {
                connectionListener_ = listener;
            }
            
            void connectServer(const std::string &userID, const std::string &token, const std::string &host, const uint16_t port);
            
            void onConnectionStatusChanged(int state);
            
            ConnectionState getConnectivityState() {
                return connectionState_;
            }
            
            void sendConnect(AutoBuffer &pack);
            void sendPing(AutoBuffer &pack);
            void pullMessage(const char* topic, const unsigned char* data, const size_t dataLen, AutoBuffer& pack);
            void publishMessage(const unsigned char* data, const size_t dataLen, AutoBuffer& pack);
            
            bool onConnectAck(const CmdHeader header, const AutoBuffer &packed);
            
            pbc_env *getPBEnv() {
                return pbEnv_;
            }
        };
    }
}


#endif /* ConnectivityLogic_hpp */
