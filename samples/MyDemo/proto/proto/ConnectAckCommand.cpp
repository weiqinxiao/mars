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
        ConnectAckCommand::ConnectAckCommand(const CmdHeader header):AbstractCommand(header), userID_(NULL), sessionID_(NULL) {
        }
        
        void ConnectAckCommand::decodeMessage() {
            readByte();//ignore checksum
            int state = readByte();
            state_ = ConnectionStatus(state);
            if (state_ == ACCEPTED) {
                userID_ = readUTF8();
                sessionID_ = readUTF8();
                connectedTime_ = readLong();
            }
        }

        ConnectAckCommand::~ConnectAckCommand() {
            if (userID_) delete userID_;
            if (sessionID_) delete sessionID_;
        }
    }
}
