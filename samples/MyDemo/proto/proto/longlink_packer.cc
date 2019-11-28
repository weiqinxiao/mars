// Tencent is pleased to support the open source community by making Mars available.
// Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.

// Licensed under the MIT License (the "License"); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://opensource.org/licenses/MIT

// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied. See the License for the specific language governing permissions and
// limitations under the License.


/*
 * longlink_packer.cc
 *
 *  Created on: 2012-7-18
 *      Author: yerungui, caoshaokun
 */

#include "longlink_packer.h"

#include <arpa/inet.h>

//#ifdef __APPLE__
//#include "mars/xlog/xlogger.h"
//#else
#include "mars/comm/xlogger/xlogger.h"
//#endif
#include "mars/comm/autobuffer.h"
#include "mars/stn/stn.h"
#include "ConnectivityLogic.hpp"
#include "AbstractCommand.hpp"

static uint32_t sg_client_version = 0;

#pragma pack(push, 1)
struct __STNetMsgXpHeader {
    uint32_t    head_length;
    uint32_t    client_version;
    uint32_t    cmdid;
    uint32_t    seq;
    uint32_t    body_length;
};
#pragma pack(pop)

namespace mars {
    namespace stn {
        
#define NOOP_CMDID 6
#define SIGNALKEEP_CMDID 243
#define PUSH_DATA_TASKID 0
        
        longlink_tracker* (*longlink_tracker::Create)()
        = []() {
            return new longlink_tracker;
        };
        
        void SetClientVersion(uint32_t _client_version)  {
            sg_client_version = _client_version;
        }
        
        void (*longlink_pack)(uint32_t _cmdid, uint32_t _seq, const AutoBuffer& _body, const AutoBuffer& _extension, AutoBuffer& _packed, longlink_tracker* _tracker)
        = [](uint32_t _cmdid, uint32_t _seq, const AutoBuffer& _body, const AutoBuffer& _extension, AutoBuffer& _packed, longlink_tracker* _tracker) {
            xinfo2(TSF"longlink-log pack: %_", _cmdid);
            switch (_cmdid) {
                case CmdID_Connect:
                    ConnectivityLogic::Instance()->doConnect(_packed);
                    break;
                case PUBLISH:
                    _packed.Write(_body);
                    break;
                case NOOP_CMDID:
                    ConnectivityLogic::Instance()->doPing(_packed);
                    break;
            }

            _packed.Seek(0, AutoBuffer::ESeekStart);
        };
        
        
        int (*longlink_unpack)(const AutoBuffer& _packed, uint32_t& _cmdid, uint32_t& _seq, size_t& _package_len, AutoBuffer& _body, AutoBuffer& _extension, longlink_tracker* _tracker)
        = [](const AutoBuffer& _packed, uint32_t& _cmdid, uint32_t& _seq, size_t& _package_len, AutoBuffer& _body, AutoBuffer& _extension, longlink_tracker* _tracker) {
            size_t len = _packed.Length();
            if (_packed.Length() < 2) {
                xwarn2(TSF"longlink-log unpack: size less 2, %_", len);
                return LONGLINK_UNPACK_CONTINUE;
            }
            unsigned char flag = ((unsigned char*)_packed.Ptr())[0];
            mars::stn::CmdHeader header(flag);
            xinfo2(TSF"longlink-log unpack: %_", header.mqttcmd);
            switch (header.mqttcmd) {
                case CONNACK:
                    ConnectivityLogic::Instance()->onConnectCallBack(header, _packed);
                    _package_len = _packed.Length();
                    _cmdid = CmdID_Connect;
                    _seq = Task::kLongLinkIdentifyCheckerTaskID;
                    break;
                case PUBLISH:
                    break;
                case PINGRESP:
                    _package_len = _packed.Length();
                    _cmdid = NOOP_CMDID;
                    break;
            }
            
            return LONGLINK_UNPACK_OK;
        };
        
        
        uint32_t (*longlink_noop_cmdid)()
        = []() -> uint32_t {
            return NOOP_CMDID;
        };
        
        bool  (*longlink_noop_isresp)(uint32_t _taskid, uint32_t _cmdid, uint32_t _recv_seq, const AutoBuffer& _body, const AutoBuffer& _extend)
        = [](uint32_t _taskid, uint32_t _cmdid, uint32_t _recv_seq, const AutoBuffer& _body, const AutoBuffer& _extend) {
            return Task::kNoopTaskID == _taskid && NOOP_CMDID == _cmdid;
        };
        
        uint32_t (*signal_keep_cmdid)()
        = []() -> uint32_t {
            return SIGNALKEEP_CMDID;
        };
        
        void (*longlink_noop_req_body)(AutoBuffer& _body, AutoBuffer& _extend)
        = [](AutoBuffer& _body, AutoBuffer& _extend) {
            
        };
        
        void (*longlink_noop_resp_body)(const AutoBuffer& _body, const AutoBuffer& _extend)
        = [](const AutoBuffer& _body, const AutoBuffer& _extend) {
            
        };
        
        uint32_t (*longlink_noop_interval)()
        = []() -> uint32_t {
            return 10 * 1000;
        };
        
        bool (*longlink_complexconnect_need_verify)()
        = []() {
            return false;
        };
        
        bool (*longlink_ispush)(uint32_t _cmdid, uint32_t _taskid, const AutoBuffer& _body, const AutoBuffer& _extend)
        = [](uint32_t _cmdid, uint32_t _taskid, const AutoBuffer& _body, const AutoBuffer& _extend) {
            return PUSH_DATA_TASKID == _taskid;
        };
        
        bool (*longlink_identify_isresp)(uint32_t _sent_seq, uint32_t _cmdid, uint32_t _recv_seq, const AutoBuffer& _body, const AutoBuffer& _extend)
        = [](uint32_t _sent_seq, uint32_t _cmdid, uint32_t _recv_seq, const AutoBuffer& _body, const AutoBuffer& _extend) {
            return _sent_seq == _recv_seq && 0 != _sent_seq;
        };
        
    }
}
