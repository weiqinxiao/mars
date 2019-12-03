//
//  PublishCommand.cpp
//  proto
//
//  Created by weiqinxiao on 2019/11/26.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "PublishCommand.hpp"


namespace mars {
    namespace stn {
        PublishCommand::PublishCommand(const unsigned short msgID, const char *topic, const unsigned char *data, const size_t dataLen) : AbstractCommand(QUERY, CmdQos::QOS_AT_MOST_ONCE, CmdRetain::RETAIN_YES), topic_(topic), targetID_(NULL), signature_(0xff), data_(data), msgID_(msgID), dataLen_(dataLen) {
            
        }
        
        size_t PublishCommand::encodeMessage() {
            unsigned short lsb = msgID_ & 0xFF;
            unsigned short msb = (msgID_ & 0xFF00) >> 8;
            wirteLong(signature_);
            writeUTF8(topic_);
            write(msb);
            write(lsb);
            write(data_, dataLen_);
            return payloadLength();
        }
        
        void PublishCommand::decodeMessage(unsigned int msgLen) {
            
        }
    }
}
