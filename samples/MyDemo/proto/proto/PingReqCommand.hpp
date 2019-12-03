//
//  PingCommand.hpp
//  proto
//
//  Created by weiqinxiao on 2019/11/27.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef PingCommand_hpp
#define PingCommand_hpp

#include <stdio.h>
#include "AbstractCommand.hpp"

namespace mars {
    namespace stn {
        class PingReqCommand : public AbstractCommand {
            
        public:
            PingReqCommand();
            
            size_t encodeMessage(){return 0;};
            
            void decodeMessage(unsigned int msgLen){};
            
            void encode(AutoBuffer& pack);
        };
        
        class PingRespCommand : public AbstractCommand {
            
        public:
            PingRespCommand();
            
            size_t encodeMessage(){return 0;};
            
            void decodeMessage(unsigned int msgLen){};//no body to decode
        };
    }
}
#endif /* PingCommand_hpp */
