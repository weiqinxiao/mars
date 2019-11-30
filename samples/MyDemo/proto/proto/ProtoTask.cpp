//
//  AbstractTask.cpp
//  proto
//
//  Created by weiqinxiao on 2019/11/27.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "ProtoTask.hpp"

namespace mars {
    namespace stn {
        ProtoTask::ProtoTask(TaskID taskID, std::string topic) : Task(taskID), topic_(topic) {
            user_context = this;
            channel_select = Task::kChannelLong;
            limit_flow = false;
            limit_frequency = false;
            need_authed = false;
        }
    }
}
