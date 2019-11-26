//
//  AbstractCommand.cpp
//  protocol
//
//  Created by weiqinxiao on 2019/11/6.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "AbstractCommand.hpp"

namespace mars {
    namespace cmd {
        
        AbstractCommand::AbstractCommand(CmdID id, char qos, bool retain) {
            unsigned char flag = (unsigned char)((id << 4) | (qos << 1));
        }
        
        
    }
}
