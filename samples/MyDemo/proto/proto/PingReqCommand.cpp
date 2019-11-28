//
//  PingCommand.cpp
//  proto
//
//  Created by weiqinxiao on 2019/11/27.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "PingReqCommand.hpp"
namespace mars {
    namespace stn {
        
        PingReqCommand::PingReqCommand() : AbstractCommand(PINGREQ) {
            
        }
        
        void PingReqCommand::encode(AutoBuffer& pack) {
            unsigned char flag = header_.encode();
            pack.Write(flag);
            //ignore body
            unsigned char ext[] = {(PINGREQ<<4)^0, 0};
            pack.Write(ext, 2);
        }
        
        PingRespCommand::PingRespCommand() : AbstractCommand(PINGRESP) {
            
        }
    }
}
