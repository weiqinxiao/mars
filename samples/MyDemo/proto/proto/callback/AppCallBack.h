//
//  AppCallback.hpp
//  proto
//
//  Created by weiqinxiao on 2019/11/14.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef AppCallback_hpp
#define AppCallback_hpp

#include <stdio.h>
#import "mars/app/app.h"
#import "mars/app/app_logic.h"

namespace mars {
    namespace app {
        class AppCallBack : public Callback {
            
        private:
            AppCallBack();
            ~AppCallBack() {}
            AppCallBack& operator = (AppCallBack&);
            
            
        public:
            static AppCallBack* Instance();
            static void Release();
            
            virtual std::string GetAppFilePath();
            
            virtual AccountInfo GetAccountInfo();
            
            virtual unsigned int GetClientVersion();
            
            virtual DeviceInfo GetDeviceInfo();
            
        private:
            static AppCallBack* instance_;
        };
    }
}

#endif /* AppCallback_hpp */
