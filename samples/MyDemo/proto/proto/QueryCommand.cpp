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
        //set qos 1, server will retry send query ack command if not receive query ack confirm command.
        QueryCommand::QueryCommand(const unsigned char *data, const size_t dataLen) : AbstractCommand(QUERY, QOS_AT_LEAST_ONCE), _data(data), _dataLen(dataLen) {
            
        }
        
        QueryCommand::QueryCommand(MqttCmd cmd, const unsigned char *data, const size_t dataLen) : AbstractCommand(cmd), _data(data), _dataLen(dataLen) {
            
        }
        
        size_t QueryCommand::encodeMessage() {
            write(_data, _dataLen);
            return _dataLen;
        }

        
        QueryAckCommand::QueryAckCommand(CmdHeader header) : AbstractCommand(header) {
            
        }
        
        QueryConfirmCommand::QueryConfirmCommand(const unsigned char *data, const size_t dataLen) : QueryCommand(QUERYCON, data, dataLen) {
            
        }
    }
}
