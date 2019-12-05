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
            const unsigned char *_data;
            unsigned int _dataLen;
            
        protected:
            size_t encodeMessage();
 
        public:
            QueryCommand(const unsigned char *data, const size_t dataLen);
            
        };
        
        class QueryAckCommand : public AbstractCommand {

        public:
            QueryAckCommand(CmdHeader header);
        };
    }
}
#endif /* QueryCommand_hpp */
