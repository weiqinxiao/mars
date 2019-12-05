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
        PublishCommand::PublishCommand(const unsigned char* data, const size_t dataLen) : AbstractCommand(PUBLISH, CmdQos::QOS_AT_MOST_ONCE, CmdRetain::RETAIN_YES), _data(data), _dataLen(dataLen) {
        }
        
        PublishCommand::PublishCommand(CmdHeader header) : AbstractCommand(header), _dataLen(0), _data(NULL) {
            
        }
        
        void PublishCommand::decodeMessage(unsigned int msgLen) {
            readLong();//signature_
            readInt();//date
            _topic = readUTF8();//topic
            xinfo2(TSF"[wei] decode message: topic = %_", _topic);
        }
        
        std::string PublishCommand::getTopic() {
            return _topic;
        }

        
        size_t PublishCommand::encodeMessage() {
            write(_data, _dataLen);
            return _dataLen;
        }
        
        PublishAckCommand::PublishAckCommand(CmdHeader header) : AbstractCommand(header) {
            
        }
        
        void PublishAckCommand::decodeMessage(unsigned int msgLen) {
            _msgID = readByte() * 0x100 + readByte();
        }
    }
}
