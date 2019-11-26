//
//  ProtoListener.h
//  proto
//
//  Created by weiqinxiao on 2019/11/25.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef ProtoListener_h
#define ProtoListener_h
#include "mars/comm/xlogger/xlogger.h"


namespace mars {
    namespace stn {
        class ConnectionStatusListener {
        public:
            virtual ~ConnectionStatusListener(){
                xerror2(TSF"free ConnectionStatusListener");
            }
            
            virtual void onConnectionStatusChanged(int state, const char* userID) = 0;
        };
        
        
    }
}
#endif /* ProtoListener_h */
