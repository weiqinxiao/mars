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
            const long long signature_;
            const unsigned char *data_;
            const unsigned short msgID_;
            const size_t dataLen_;
            
        protected:
            size_t encodeMessage();
            
            void decodeMessage(unsigned int msgLen);
            
        public:
            PublishCommand(const unsigned short msgID, const char *topic, const unsigned char *data, const size_t dataLen);
        };
    }
}

#endif /* PublishCommand_hpp */
