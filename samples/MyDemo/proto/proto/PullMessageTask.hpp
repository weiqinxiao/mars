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

namespace mars {
    namespace stn {
        class PullMessageTask : public ProtoTask {
        private:
            
        public:
            PullMessageTask();
            void encodeMessage(pbc_env *env);
        };
    }
}

#endif /* PullMessageTask_hpp */
