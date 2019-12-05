//
//  ConnectAckCommand.hpp
//  proto
//
//  Created by weiqinxiao on 2019/11/25.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef ConnectAckCommand_hpp
#define ConnectAckCommand_hpp

#include "AbstractCommand.hpp"

namespace mars {
    namespace stn {
        enum ConnectionStatus {
            ACCEPTED,
            UNACCEPTABLE_PROTOCOL_VERSION,
            IDENTIFIER_REJECTED,
            SERVER_UNAVAILABLE,
            BAD_USERNAME_OR_PASSWORD,
            NOT_AUTHORIZED,
            REDIRECT,
            PACKAGE_ERROR,
            APP_BLOCK_OR_DELETE,
            BLOCK,
            TOKEN_EXPIRE,
            DEVICE_ERROR,
            HOSTNAME_ERROR
        };
        
        class ConnectAckCommand : public AbstractCommand {
        private:
            ConnectionStatus state_;
            std::string userID_;
            std::string sessionID_;
            long connectedTime_;
            
            
        public:
            ConnectAckCommand(const CmdHeader header);
            
            void decodeMessage(unsigned int msgLen);
            
            ConnectionStatus getStatus() {
                return state_;
            }
            
            const char* getUserID() {
                return userID_.c_str();
            }
        };
    }
}
#endif /* ConnectAckCommand_hpp */
