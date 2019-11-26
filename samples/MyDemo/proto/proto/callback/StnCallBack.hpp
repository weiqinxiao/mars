//
//  StnCallback.h
//  MyDemo
//
//  Created by weiqinxiao on 2019/11/14.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef StnCallback_h
#define StnCallback_h

#include <stdio.h>
#include "mars/stn/stn_logic.h"
#include "ConnectivityLogic.hpp"

namespace mars {
    namespace stn {
        class StnCallBack : public Callback {
            
        private:
            StnCallBack() {}
            ~StnCallBack() {}
            StnCallBack(StnCallBack&);
            StnCallBack& operator = (StnCallBack&);
            ConnectivityListener *listener_;
            
        public:
            static StnCallBack* Instance();
            static void Release();
            
            void setConnectivityListener(ConnectivityListener *listener) {
                listener_ = listener;
            }
            
            virtual bool MakesureAuthed(const std::string& _host);
            
            //流量统计
            virtual void TrafficData(ssize_t _send, ssize_t _recv);
            
            //底层询问上层该host对应的ip列表
            virtual std::vector<std::string> OnNewDns(const std::string& _host);
            //网络层收到push消息回调
            virtual void OnPush(uint64_t _channel_id, uint32_t _cmdid, uint32_t _taskid, const AutoBuffer& _body, const AutoBuffer& _extend);
            //底层获取task要发送的数据
            virtual bool Req2Buf(uint32_t _taskid, void* const _user_context, AutoBuffer& _outbuffer, AutoBuffer& _extend, int& _error_code, const int _channel_select, const std::string& host);
            //底层回包返回给上层解析
            virtual int Buf2Resp(uint32_t _taskid, void* const _user_context, const AutoBuffer& _inbuffer, const AutoBuffer& _extend, int& _error_code, const int _channel_select);
            //任务执行结束
            virtual int  OnTaskEnd(uint32_t _taskid, void* const _user_context, int _error_type, int _error_code);
            
            //上报网络连接状态
            virtual void ReportConnectStatus(int _status, int longlink_status);
            //长连信令校验 ECHECK_NOW, ECHECK_NEVER = 1, ECHECK_NEXT = 2
            virtual int  GetLonglinkIdentifyCheckBuffer(AutoBuffer& _identify_buffer, AutoBuffer& _buffer_hash, int32_t& _cmdid);
            //长连信令校验回包
            virtual bool OnLonglinkIdentifyResponse(const AutoBuffer& _response_buffer, const AutoBuffer& _identify_buffer_hash);
            //
            virtual void RequestSync();
            
            
        private:
            static StnCallBack* instance_;
            
        };
    }
}

#endif /* StnCallback_h */
