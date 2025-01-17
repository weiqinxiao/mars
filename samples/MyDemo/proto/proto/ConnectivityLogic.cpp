//
//  ConnectivityLogic.cpp
//  proto
//
//  Created by weiqinxiao on 2019/11/15.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "ConnectivityLogic.hpp"
#include "ConnectCommand.hpp"
#include "ProtoClient.hpp"
#include "ConnectAckCommand.hpp"
#include "PublishCommand.hpp"
#include "pbdata.h"
#include "PingReqCommand.hpp"
#include "QueryCommand.hpp"

namespace mars {
    namespace stn {
        
        static ConnectivityLogic *instance_;
        
        ConnectivityLogic *ConnectivityLogic::Instance() {
            if (instance_ == NULL) {
                instance_ = new ConnectivityLogic();
            }
            return instance_;
        }

        ConnectivityLogic::ConnectivityLogic():connectionState_(Disconnected),connectionListener_(NULL) {
            struct pbc_slice slice;
            slice.len = sizeof(pbdata);
            slice.buffer = (void*)pbdata;
            pbEnv_ = pbc_new();
            pbc_register(pbEnv_, &slice);
        }
        
        void ConnectivityLogic::onConnectionStatusChanged(int state) {
            switch (state) {
                case mars::stn::kServerFailed:
                case mars::stn::kServerDown:
                case mars::stn::kGateWayFailed:
                    connectionState_ = Unavialable;
                    break;
                case mars::stn::kConnecting:
                case mars::stn::kConnected:
                    connectionState_ = Connecting;
                    break;
                case mars::stn::kNetworkUnkown:
                    connectionState_ = Unavialable;
                    return;
                default:
                    connectionState_ = Unavialable;
                    return;
            }
            if (connectionListener_ != NULL) {
                connectionListener_->onConnectionStatusChanged(connectionState_, NULL);
            }
        }
        
        void ConnectivityLogic::connectServer(const std::string &userID, const std::string &token, const std::string &host, const uint16_t port) {
            userID_ = userID;
            token_ = token;
            host_ = host;
            port_ = port;
            
            xinfo2(TSF"connect : %_, %_, %_", appKey_, host, port);

            std::vector<uint16_t> _ports;
            _ports.push_back(port);
            mars::stn::SetLonglinkSvrAddr(host, _ports, "");
            mars::stn::MakesureLonglinkConnected();
            mars::baseevent::OnForeground(true);
        }

        void ConnectivityLogic::sendConnect(AutoBuffer &pack) {
            xinfo2(TSF"[wei] connect server: appKey = %_", appKey_);
            ConnectCommand cmd(appKey_.c_str(), token_.c_str(), deviceID_.c_str(), appName_.c_str(), userAgent_.c_str());
            cmd.encode(pack);
        }
        
        void ConnectivityLogic::sendPing(AutoBuffer &pack) {
            xinfo2(TSF"[wei] send ping command");
            PingReqCommand cmd;
            cmd.encode(pack);
        }

        void ConnectivityLogic::pullMessage(const unsigned char* data, const size_t dataLen, AutoBuffer& pack) {
            xinfo2(TSF"[wei] send pull command");
            QueryCommand cmd(data, dataLen);
            cmd.encode(pack);
        }
        
        void ConnectivityLogic::pullConfirmMessage(const unsigned char* data, const size_t dataLen, AutoBuffer& pack) {
            xinfo2(TSF"[wei] send pull confirm command");
            QueryConfirmCommand cmd(data, dataLen);
            cmd.encode(pack);
        }
        
        void ConnectivityLogic::publishMessage(const unsigned char* data, const size_t dataLen, AutoBuffer& pack) {
            xinfo2(TSF"[wei] send publish command: %_", dataLen);
            PublishCommand cmd(data, dataLen);
            cmd.encode(pack);
        }
        
        bool ConnectivityLogic::onConnectAck(const CmdHeader header, const AutoBuffer &packed) {
            ConnectAckCommand *cmd = new ConnectAckCommand(header);
            cmd->decode(packed);
            ConnectionStatus state = cmd->getStatus();
            const char* userID = cmd->getUserID();
            if (cmd) delete cmd;
            xinfo2(TSF"[wei] : status = %_", state);
            bool result = false;
            switch (state) {
                case ACCEPTED:
                    connectionState_ = Connected;
                    result = true;
                    break;
                case REDIRECT:
                    connectionState_ = Redirected;
                    break;
                case IDENTIFIER_REJECTED:
                    connectionState_ = AppKeyReject;
                    break;
                case BLOCK:
                    connectionState_ = UserBlocked;
                    break;
                case APP_BLOCK_OR_DELETE:
                    connectionState_ = AppBlocked;
                    break;
                case TOKEN_EXPIRE:
                    connectionState_ = TokenExpired;
                    break;
                case BAD_USERNAME_OR_PASSWORD:
                    connectionState_ = TokenInvalid;
                    break;
                case UNACCEPTABLE_PROTOCOL_VERSION:
                    connectionState_ = Disconnected;
                    break;
                default:
                    break;
            }
            if (connectionListener_ != NULL) {
                connectionListener_->onConnectionStatusChanged(state, userID);
            }
            return result;
        }
    }
}
