//
//  ConnectCommand.hpp
//  proto
//
//  Created by weiqinxiao on 2019/11/15.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef ConnectCommand_hpp
#define ConnectCommand_hpp

#include <stdio.h>
#include <string>
#include "AbstractCommand.hpp"

namespace mars {
    namespace stn {
        class ConnectCommand : public AbstractCommand {
        private:
            const char* appKey_;
            const char* appName_;
            const char* deviceID_;
            const char* userAgent_;
            const char* token_;
            
        public:
            ConnectCommand(const char* appKey, const char* token, const char* deviceID, const char* appName, const char* userAgent);
            
        protected:
            size_t encodeMessage();
        };
    }
}

#endif /* ConnectCommand_hpp */
