//
//  ViewController.m
//  MyDemo
//
//  Created by weiqinxiao on 2019/11/14.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#import "ViewController.h"
#import "proto/ProtoClient.hpp"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "proto/ProtoListener.h"

@interface ViewController ()
@property (weak, nonatomic) IBOutlet UITextView *connectState;

@end



class TSConnectionListener : public mars::stn::ConnectionStatusListener {
public:
    virtual ~TSConnectionListener(){}
    virtual void onConnectionStatusChanged(int state, const char* userID) {
        NSLog(@"[mydemo] onConnectionStatusChanged: %s", userID);
    }
};

class TSMessageReceiver : public mars::stn::ReceiveMessageListener {
public:
    
    void onReceivedMessages(std::list<mars::stn::MessageContent> msgList) {
        NSLog(@"[mydemo] received msg: %d", msgList.size());
    }
};

class TSSendMsgCallBack : public mars::stn::SendMessageCallBack {
public:
    
    void onSuccess(std::string msgUID) {
        NSLog(@"[mydemo] sent msg: %s", msgUID.c_str());
    }
    
    void onError(int code) {
        NSLog(@"[mydemo] sent msg error: %d", code);
    }
    
};

@implementation ViewController


- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    NSLog(@"[mydemo] view loaded");

    mars::stn::initClient();
    mars::stn::setEnviroment([@"pkfcgjstf71m8" cStringUsingEncoding:kCFStringEncodingUTF8],
                             [@"test.com" cStringUsingEncoding:kCFStringEncodingUTF8],
                             [@"deviceID" cStringUsingEncoding:kCFStringEncodingUTF8],
                             [@"Apple" cStringUsingEncoding:kCFStringEncodingUTF8],
                             [@"iPhone 6 Simulator" cStringUsingEncoding:kCFStringEncodingUTF8],
                             [@"8.1.1" cStringUsingEncoding:kCFStringEncodingUTF8]);
    mars::stn::setConnectionListener(new TSConnectionListener());
    mars::stn::setReceiveMessageListener(new TSMessageReceiver());
}

- (IBAction)connectServer:(id)sender {
    NSLog(@"[mydemo] connectServer");
    
    mars::stn::connectServer([@"userId120" cStringUsingEncoding:kCFStringEncodingUTF8],
                             [@"AAAAWj7GY/twyF4rcqMfRit6vjqxwYPkP+8mVRYPcrhngjgzGD45GDoniioKgpOhg9kCKQ==" cStringUsingEncoding:kCFStringEncodingUTF8],
                             [@"10.12.9.46" cStringUsingEncoding:kCFStringEncodingUTF8],
                             8043);
}
- (IBAction)sendMessage:(id)sender {
    NSLog(@"[mydemo] sendMessage");
    
    mars::stn::sendMessage(mars::stn::ConversationType::PRIVATE,
                           [@"userId120" cStringUsingEncoding:kCFStringEncodingUTF8],
                           [@"RC:Txt" cStringUsingEncoding:kCFStringEncodingUTF8],
                           "",
                           "",
                           [@"{\"content\":\"2222\"}" cStringUsingEncoding:kCFStringEncodingUTF8],
                           new TSSendMsgCallBack());
}

@end
