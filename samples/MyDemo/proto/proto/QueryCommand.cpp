//
//  QueryCommand.cpp
//  proto
//
//  Created by weiqinxiao on 2019/12/3.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "QueryCommand.hpp"

namespace mars {
    namespace stn {
        QueryCommand::QueryCommand(const unsigned char *data, const size_t dataLen) : AbstractCommand(QUERY, CmdQos::QOS_AT_MOST_ONCE, CmdRetain::RETAIN_YES), _data(data), _dataLen(dataLen) {
            
        }
        
        size_t QueryCommand::encodeMessage() {
            write(_data, _dataLen);
            return _dataLen;
        }

        
        QueryAckCommand::QueryAckCommand(CmdHeader header) : AbstractCommand(header) {
            
        }
    }
}
