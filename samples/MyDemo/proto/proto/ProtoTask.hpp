//
//  AbstractTask.hpp
//  proto
//
//  Created by weiqinxiao on 2019/11/27.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef AbstractTask_hpp
#define AbstractTask_hpp

#include <stdio.h>
#include "mars/stn/stn_logic.h"
#include "pbc/pbc.h"
#include "AbstractCommand.hpp"

namespace mars {
    namespace stn {
        enum ConversationType {
            PRIVATE=1,
            DISCUSSION,
            GROUP,
            CHATROOM,
            RECEPTION,
            SYSTEM,
            MC,
            MP,
            RCPUSH,
            RCSUPERGROUP,
            RCENCRYPTEDCHAT,
            RCRTCROOM
        };
        
        enum TaskID {
            TaskID_PullMsg = 0x10,
            TaskID_PublishMsg = 0x11,
        };
        
        enum MessageDirection {
            Receive = 0,
            Send = 1,
        };
        
        struct MessageContent {
            std::string          targetID;
            std::string          senderID;
            std::string          objName;
            std::string          content;
            std::string          extra;//数据存储的扩展字段
            std::string          uid;//消息唯一 ID
            
            int             conversationType;
            long            messageID;//消息 ID，数据库存储 ID
            MessageDirection             direction;//发件箱为1，收件箱为0
            int             readStatus;//已读状态 0 未读，1 已读
            int             sendStatus;//成功 0，失败 1
            int64_t         sentTime;//消息发送成功时间
            int64_t         rcvTime;//消息接收时间，当前时间
            int             messageType;      //0-realtime message, 1-history roaming
            int64_t         deleteTime;
            
            std::string          m_RRReqInfo;
            std::string          m_Source;
            
            ~MessageContent(){};
        };
        
        class ProtoTask : public Task {
        private:
            
        protected:
            std::string topic_;
            unsigned char* data_;
            size_t dataLen_;
            pbc_wmessage *pbMsg_;
            long lastSentTime_;
            long lastRcvTime_;
            
        protected:
            MessageContent decodeMessageContent(pbc_rmessage *message);
            
        public:
            ProtoTask(TaskID taskID, std::string topic);
            
            ~ProtoTask() {
                if (pbMsg_) {
                    pbc_wmessage_delete(pbMsg_);
                    data_ = NULL;
                    dataLen_ = 0;
                }
            };
            
            virtual void encodeMessage(pbc_env *env){};
            virtual void decodeMessage(pbc_env *env, unsigned char* data, size_t dataLen){};
            
            const char* getTopic() {
                return topic_.c_str();
            }
            
            unsigned char *getData() {
                return data_;
            }
            
            size_t getDataLen() {
                return dataLen_;
            }
        };
    }
}
#endif /* AbstractTask_hpp */
