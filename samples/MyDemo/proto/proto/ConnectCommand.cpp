//
//  ConnectCommand.cpp
//  proto
//
//  Created by weiqinxiao on 2019/11/15.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "ConnectCommand.hpp"

namespace mars {
    namespace stn {
        ConnectCommand::ConnectCommand(const char* appKey, const char* token, const char* deviceID, const char* appName, const char* userAgent):AbstractCommand(CONNECT, QOS_EXACTLY_ONCE, RETAIN_YES), appKey_(appKey), token_(token), deviceID_(deviceID), appName_(appName), userAgent_(userAgent) {

        };
        
        size_t ConnectCommand::encodeMessage() {
            unsigned char flag = 0;
            flag |= CmdFlag::FLAG_APPID;
            flag |= CmdFlag::FLAG_TOKEN;
            flag |= CmdFlag::FLAG_APPNAME;
            flag |= CmdQos::QOS_EXACTLY_ONCE;
            flag |= CmdRetain::RETAIN_YES;
            writeUTF8(protoID);
            write(protoVersion);
            write(flag);
            write(protoKeepAlive);
            writeUTF8(deviceID_);
            writeUTF8(appName_);
            writeUTF8(userAgent_);//platform-pacakge-version
            writeUTF8(appKey_);
            writeUTF8(token_);
            return payloadLength();
        }
    }
}
