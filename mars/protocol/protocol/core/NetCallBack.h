//
//  NetCallBack.hpp
//  protocol
//
//  Created by weiqinxiao on 2019/11/7.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#ifndef NetCallBack_hpp
#define NetCallBack_hpp

#include <stdio.h>
#include "mars/stn/stn_logic.h"

namespace LinkClient {
    class NetCallBack : public mars::stn::Callback {
        private:
            static NetCallBack* ins;
            int connectionStatus;
            
            NetCallBack();
            
        public:
            static NetCallBack *instance();
            
            bool MakesureAuthed(const std::string& _host);
            
            //流量统计
            void TrafficData(ssize_t _send, ssize_t _recv);
            
            //底层询问上层该host对应的ip列表
            std::vector<std::string> OnNewDns(const std::string& host);
            //网络层收到push消息回调
            void OnPush(uint64_t _channel_id, uint32_t _cmdid, uint32_t _taskid, const AutoBuffer& _body, const AutoBuffer& _extend);
            //底层获取task要发送的数据
            bool Req2Buf(uint32_t _taskid, void* const _user_context, AutoBuffer& outbuffer, AutoBuffer& extend, int& error_code, const int channel_select, const std::string& host);
            //底层回包返回给上层解析
            int Buf2Resp(uint32_t _taskid, void* const _user_context, const AutoBuffer& _inbuffer, const AutoBuffer& _extend, int& _error_code, const int _channel_select);
            //任务执行结束
            int  OnTaskEnd(uint32_t _taskid, void* const _user_context, int _error_type, int _error_code);
            
            //上报网络连接状态
            void ReportConnectStatus(int _status, int _longlink_status);
            
            //长连信令校验 ECHECK_NOW, ECHECK_NEXT = 1, ECHECK_NEVER = 2
            int  GetLonglinkIdentifyCheckBuffer(AutoBuffer& _identify_buffer, AutoBuffer& _buffer_hash, int32_t& _cmdid);
            //长连信令校验回包
            bool OnLonglinkIdentifyResponse(const AutoBuffer& _response_buffer, const AutoBuffer& _identify_buffer_hash);
            
            
            void RequestSync();
        };
}
#endif /* NetCallBack_hpp */
