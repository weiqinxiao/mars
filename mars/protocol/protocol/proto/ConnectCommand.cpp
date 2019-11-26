//
//  ConnectCommand.cpp
//  protocol
//
//  Created by weiqinxiao on 2019/11/6.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "ConnectCommand.hpp"


namespace mars {
    namespace cmd {
        ConnectCommand::ConnectCommand(std::string &platform, std::string &version, std::string deviceID, char qos, bool retain) : AbstractCommand(CONNECT, qos, retain), mPlatform(platform), mVersion(version), mDeviceID(deviceID) {
            
        }
    }
}
