//
//  main.m
//  MyDemo
//
//  Created by weiqinxiao on 2019/11/14.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <sys/xattr.h>

#import "AppDelegate.h"
#import <mars/comm/xlogger/xloggerbase.h>
#import <mars/comm/xlogger/xlogger.h>
#import <mars/log/appender.h>

int main(int argc, char * argv[]) {
    @autoreleasepool {
        NSString* logPath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingString:@"/log"];
        
        // set do not backup for logpath
        const char* attrName = "com.apple.MobileBackup";
        u_int8_t attrValue = 1;
        setxattr([logPath UTF8String], attrName, &attrValue, sizeof(attrValue), 0, 0);
        
        // init xlog
#if DEBUG
        xlogger_SetLevel(kLevelDebug);
        appender_set_console_log(true);
#else
        xlogger_SetLevel(kLevelInfo);
        appender_set_console_log(false);
#endif
        appender_open(kAppednerAsync, [logPath UTF8String], "Test", "");

        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
