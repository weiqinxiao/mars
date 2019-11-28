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
        enum TaskID {
            TaskID_PullMsg = 0x10,
            
        };
        
        class ProtoTask : public Task {
        private:
            
        protected:
            pbc_wmessage *pbMsg_;
            AbstractCommand *cmd_;
            
        public:
            ProtoTask(TaskID cmdID);
            
            ~ProtoTask() {
                if (pbMsg_) pbc_wmessage_delete(pbMsg_);
                if (cmd_) delete cmd_;
            };
            
            virtual void encodeMessage(pbc_env *env, AutoBuffer &pack) = 0;
        };
    }
}
#endif /* AbstractTask_hpp */
