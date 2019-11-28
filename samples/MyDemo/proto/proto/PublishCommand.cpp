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
        PublishCommand::PublishCommand(const char *topic, const unsigned char *data, const unsigned long dataLen) : AbstractCommand(PUBLISH, CmdQos::QOS_AT_MOST_ONCE, CmdRetain::RETAIN_YES), topic_(topic), targetID_(NULL), signature_(0xff), data_(data), msgID_(0), dataLen_(dataLen) {
            
        }
        
        size_t PublishCommand::encodeMessage() {
            int lsb = msgID_ & 0xFF;
            int msb = (msgID_ & 0xFF00) >> 8;
            write(&signature_, 8);
            writeUTF8(topic_);
            write(msb);
            write(lsb);
            write(data_, dataLen_);
            return payloadLength();
        }
        
        void PublishCommand::decodeMessage() {
            
        }
    }
}
