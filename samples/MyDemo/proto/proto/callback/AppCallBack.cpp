//
//  AppCallback.cpp
//  proto
//
//  Created by weiqinxiao on 2019/11/14.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "AppCallBack.h"
#include "mars/comm/autobuffer.h"
#include "mars/comm/xlogger/xlogger.h"

namespace mars {
    namespace app {
        AppCallBack* AppCallBack::instance_ = NULL;
        
        AppCallBack* AppCallBack::Instance() {
            if(instance_ == NULL) {
                instance_ = new AppCallBack();
            }
            
            return instance_;
        }
        
        AppCallBack::AppCallBack():Callback() {
            xinfo2(TSF"AppCallBack");
        }
        
        void AppCallBack::Release() {
            delete instance_;
            instance_ = NULL;
        }
        
        // return your app path
        std::string AppCallBack::GetAppFilePath(){
            return "";
        }
        
        AccountInfo AppCallBack::GetAccountInfo() {
            AccountInfo info;
            
            return info;
        }
        
        unsigned int AppCallBack::GetClientVersion() {
            
            return 0;
        }
        
        DeviceInfo AppCallBack::GetDeviceInfo() {
            DeviceInfo info;
            
            info.devicename = "";
            info.devicetype = 1;
            
            return info;
        }
    }
    
}
