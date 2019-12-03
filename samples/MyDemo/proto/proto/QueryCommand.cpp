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
        QueryCommand::QueryCommand(const unsigned short msgID, const char *topic, const unsigned char *data, const size_t dataLen) : AbstractCommand(QUERY, CmdQos::QOS_AT_MOST_ONCE, CmdRetain::RETAIN_YES), topic_(topic), targetID_(NULL), signature_(0xff), data_(data), msgID_(msgID), dataLen_(dataLen) {
            
        }
        
        size_t QueryCommand::encodeMessage() {
            unsigned short lsb = msgID_ & 0xFF;
            unsigned short msb = (msgID_ & 0xFF00) >> 8;
            wirteLong(signature_);
            writeUTF8(topic_);
            write(msb);
            write(lsb);
            write(data_, dataLen_);
            return payloadLength();
        }
        
        QueryAckCommand::QueryAckCommand(CmdHeader header) : AbstractCommand(header), data_(NULL), dataLen_(0), msgID_(0) {
            
        }
        
        void QueryAckCommand::decodeMessage(unsigned int msgLen) {
            msgID_ = readByte() * 0x100 + readByte();
            date_ = readInt();
            state_ = readByte() * 0x100 + readByte();
            data_ = readData(msgLen);
            dataLen_ = msgLen;
        }
        
    }
}
