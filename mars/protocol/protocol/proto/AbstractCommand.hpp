//
//  AbstractCommand.hpp
//  protocol
//
//  Created by weiqinxiao on 2019/11/6.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef AbstractCommand_hpp
#define AbstractCommand_hpp

#include <stdio.h>

namespace mars {
    namespace cmd {
        typedef enum _CmdID {
            CONNECT     = 1,
            CONNACK     = 2,
            PUBLISH     = 3,
            PUBACK      = 4,
            QUERY       = 5,
            QUERYACK    = 6,
            QUERYCON    = 7,
            PCONNECT    = 8,
            PCONNECTACK = 9,
            UNSUBSCRIBE = 10,
            UNSUBACK    = 11,
            PINGREQ     = 12,
            PINGRESP    = 13,
            DISCONNECT  = 14,
        } CmdID;
        
        typedef enum _CmdQos {
            AT_MOST_ONCE = 0,
            AT_LEAST_ONCE = 1,
            EXACTLY_ONCE = 2,
            DEFAULT = 3,
        } CmdQos;
        
        class AbstractCommand {
        private:
            unsigned char* mPayload;
            CmdID mID;
            char mQos;
            bool mRetain;
            
            
        public:
            AbstractCommand(CmdID id, char qos, bool retain);

            virtual void decode(unsigned char* body) = 0;
            
            virtual void encode(unsigned char* body) = 0;
        };
    }
}

#endif /* AbstractCommand_hpp */
