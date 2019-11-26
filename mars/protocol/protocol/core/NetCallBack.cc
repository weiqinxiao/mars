//
//  NetCallBack.cpp
//  protocol
//
//  Created by weiqinxiao on 2019/11/7.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "NetCallBack.h"

namespace mars {
    namespace stn {
 
        NetCallBack::NetCallBack() : connectionStatus(0) {
            printf("NetCallBack construct");
        };
        
        NetCallBack* NetCallBack::instance() {
            if (ins == NULL) {
                printf("new instance ");
                ins = new NetCallBack();
            }
            return ins;
        }
        
        bool NetCallBack::MakesureAuthed(const std::string& _host) {
            printf("MakesureAuthed: host = %s", _host.c_str());
            return true;
        }
        
        void NetCallBack::ReportConnectStatus(int _status, int _longlink_status) {
            printf("connect status changed: %d, %d", _status, _longlink_status);
        }
    }
}
