//
//  QueryCommand.hpp
//  proto
//
//  Created by weiqinxiao on 2019/12/3.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef QueryCommand_hpp
#define QueryCommand_hpp

#include <stdio.h>
#include "mars/comm/autobuffer.h"
#include "AbstractCommand.hpp"

namespace mars {
    namespace stn {
        class QueryCommand : public AbstractCommand {
        private:
            const char *topic_;
            const char *targetID_;
            const long long signature_;
            const unsigned char *data_;
            const unsigned short msgID_;
            unsigned int dataLen_;
            
        protected:
            size_t encodeMessage();
            
            void decodeMessage(unsigned int msgLen) {};
            
        public:
            QueryCommand(const unsigned short msgID, const char *topic, const unsigned char *data, const size_t dataLen);
        };
        
        class QueryAckCommand : public AbstractCommand {
        private:
            unsigned char *data_;
            unsigned short msgID_;
            unsigned int dataLen_;
            int state_;
            int date_;
            
        protected:
            size_t encodeMessage() {return 0;};
            
            void decodeMessage(unsigned int msgLen);
            
        public:
            QueryAckCommand(CmdHeader header);
            
            unsigned char * getData() {
                return data_;
            }
            
            unsigned int getDataLength() {
                return dataLen_;
            }
        };
    }
}
#endif /* QueryCommand_hpp */
