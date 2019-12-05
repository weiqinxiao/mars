//
//  ConnectAckCommand.cpp
//  proto
//
//  Created by weiqinxiao on 2019/11/25.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "ConnectAckCommand.hpp"

namespace mars {
    namespace stn {
        ConnectAckCommand::ConnectAckCommand(const CmdHeader header):AbstractCommand(header) {
        }
        
        void ConnectAckCommand::decodeMessage(unsigned int msgLen) {
            readByte();//ignore checksum
            int state = readByte();
            state_ = ConnectionStatus(state);
            if (state_ == ACCEPTED) {
                userID_ = readUTF8();
                sessionID_ = readUTF8();
                connectedTime_ = readLong();
            }
        }
    }
}
