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
        PublishCommand::PublishCommand(const unsigned short msgID, const char *topic, const unsigned char *data, const int dataLen) : AbstractCommand(QUERY, CmdQos::QOS_AT_MOST_ONCE, CmdRetain::RETAIN_YES), topic_(topic), targetID_(NULL), signature_(0xff), data_(data), msgID_(msgID), dataLen_(dataLen) {
            
        }
        
        PublishCommand::PublishCommand(CmdHeader header) : AbstractCommand(header), topic_(NULL), targetID_(NULL), signature_(0xff), data_(NULL), msgID_(0), dataLen_(0) {
            
        }
        
        PublishCommand::~PublishCommand() {
            if (topic_) delete topic_;
            if (targetID_) delete targetID_;
        }
        
        size_t PublishCommand::encodeMessage() {
            unsigned short lsb = msgID_ & 0xFF;
            unsigned short msb = (msgID_ & 0xFF00) >> 8;
            wirteLong(signature_);
            writeUTF8(topic_);
            write(msb);
            write(lsb);
            write(data_, dataLen_);
            xinfo2(TSF"[wei] encode message: topic = %_", topic_);
            return payloadLength();
        }
        
        void PublishCommand::decodeMessage(unsigned int msgLen) {
            signature_ = readLong();
            date_ = readInt();
            topic_ = readUTF8();
            targetID_ = readUTF8();
            msgID_ = readByte() * 0x100 + readByte();
            data_ = readData(msgLen);
            dataLen_ = msgLen;
            xinfo2(TSF"[wei] decode message: topic = %_", topic_);
        }
        
        
        PublishAckCommand::PublishAckCommand() : AbstractCommand(PUBACK) {
            
        }
    }
}
