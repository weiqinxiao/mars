//
//  PublishCommand.hpp
//  proto
//
//  Created by weiqinxiao on 2019/11/26.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef PublishCommand_hpp
#define PublishCommand_hpp

#include <stdio.h>
#include "AbstractCommand.hpp"

namespace mars {
    namespace stn {
        class PublishCommand : public AbstractCommand {
        private:
            const unsigned char* _data;
            const size_t _dataLen;
            std::string _topic;
            
        protected:
            size_t encodeMessage();
            void decodeMessage(unsigned int msgLen);

        public:
            PublishCommand(const unsigned char* data, const size_t dataLen);
            PublishCommand(CmdHeader header);
            
            std::string getTopic();
        };
        
        class PublishAckCommand : public AbstractCommand {
        private:
            unsigned short _msgID;
            
        public:
            PublishAckCommand(CmdHeader header);
            void decodeMessage(unsigned int msgLen);
            unsigned short getMessageID() {
                return _msgID;
            }
        };
    }
}

#endif /* PublishCommand_hpp */
