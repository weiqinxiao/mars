//
//  PullMessageTask.hpp
//  proto
//
//  Created by weiqinxiao on 2019/11/26.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef PullMessageTask_hpp
#define PullMessageTask_hpp

#include <stdio.h>
#include "pbc/pbc.h"
#include "ProtoTask.hpp"
#include <list>

namespace mars {
    namespace stn {
        class PullMessageTask : public ProtoTask {
        private:
            std::list<MessageContent> msgList;
            bool finishPull_;
            
        public:
            PullMessageTask(long lastSentTime, long lastRcvTime, unsigned short msgID);
            ~PullMessageTask() {}
            void encodeMessage(pbc_env *env);
            void decodeMessage(pbc_env *env, AutoBuffer &inBuffer);
            
            long getLastSentTime() {
                return lastSentTime_;
            }
            
            long getLastReceiveTime() {
                return lastRcvTime_;
            }
            
            bool isFinishPull() {
                return finishPull_;
            }
            
            std::list<MessageContent> getPulledMessages() {
                return msgList;
            }
        };
        
        class PullConfirmTask : public ProtoTask {
        public:
            PullConfirmTask(unsigned short msgID);
            void encodeMessage(pbc_env *env);
        };
    }
}

#endif /* PullMessageTask_hpp */
