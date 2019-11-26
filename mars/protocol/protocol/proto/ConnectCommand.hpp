//
//  ConnectCommand.hpp
//  protocol
//
//  Created by weiqinxiao on 2019/11/6.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef ConnectCommand_hpp
#define ConnectCommand_hpp

#include <stdio.h>
#include <string>

#include "AbstractCommand.hpp"

namespace mars {
    namespace cmd {
        class ConnectCommand : AbstractCommand {
        private:
            std::string mPlatform;
            std::string mVersion;
            std::string mDeviceID;
            
            ConnectCommand(std::string &platform, std::string &version, std::string deviceID, char qos, bool retain);
            
        };
    }
}
#endif /* ConnectCommand_hpp */
