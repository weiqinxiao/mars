// Tencent is pleased to support the open source community by making Mars available.
// Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.

// Licensed under the MIT License (the "License"); you may not use this file except in 
// compliance with the License. You may obtain a copy of the License at
// http://opensource.org/licenses/MIT

// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied. See the License for the specific language governing permissions and
// limitations under the License.

//
//  appcomm_callback.h
//  MPApp
//
//  Created by yanguoyue on 16/3/3.
//  Copyright © 2016年 tencent. All rights reserved.
//

#ifndef appcomm_callback_h
#define appcomm_callback_h

#import "mars/app/app.h"
#import "mars/app/app_logic.h"

namespace mars {
    namespace app {


class AppCallBack : public Callback {
    
private:
    AppCallBack() {}
    ~AppCallBack() {}
    AppCallBack(AppCallBack&);
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
        
}}

#endif /* appcomm_callback_h */
