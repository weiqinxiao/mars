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
            const char *topic_;
            const char *targetID_;
            const char *uuid_;
            unsigned long long signature_;
            int date_;
            const unsigned char *data_;
            unsigned short msgID_;
            int dataLen_;
            
        protected:
            size_t encodeMessage();
            
            void decodeMessage(unsigned int msgLen);
            
        public:
            PublishCommand(const unsigned short msgID, const char *topic, const unsigned char *data, const int dataLen);
            PublishCommand(CmdHeader header);
            ~PublishCommand();
            
            const unsigned char* getData() {
                return data_;
            }
            
            const int getDataLength() {
                return dataLen_;
            }
            
            const char* getTopic() {
                return topic_;
            }
        };
        
        class PublishAckCommand : public AbstractCommand {
        public:
            PublishAckCommand();
            size_t encodeMessage() {
                return 0;
            };
            
            void decodeMessage(unsigned int msgLen) {};
            
        };
    }
}

#endif /* PublishCommand_hpp */
