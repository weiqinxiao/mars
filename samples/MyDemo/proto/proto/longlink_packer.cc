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
#include "ProtoTask.hpp"
#include "QueryCommand.hpp"
#include "PublishCommand.hpp"

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
            xinfo2(TSF"[wei] pack : seq = %_, cmdid = %_", _seq, _cmdid);
            switch (_cmdid) {
                case CmdID_Connect:
                {
                    ConnectivityLogic::Instance()->sendConnect(_packed);
                    break;
                }
                case NOOP_CMDID:
                {
                    ConnectivityLogic::Instance()->sendPing(_packed);
                    break;
                }
                case CmdID_Pull:
                {
                    ConnectivityLogic::Instance()->pullMessage((unsigned char*)_body.Ptr(), _body.Length(), _packed);
                    break;
                }
                case CmdID_Publish:
                {
                    ConnectivityLogic::Instance()->publishMessage((unsigned char*)_body.Ptr(), _body.Length(), _packed);
                    break;
                }
                case CmdID_PullConfirm:
                {
                    ConnectivityLogic::Instance()->pullConfirmMessage((unsigned char*)_body.Ptr(), _body.Length(), _packed);
                    break;
                }
            }

            _packed.Seek(0, AutoBuffer::ESeekStart);
        };
        
        
        int (*longlink_unpack)(const AutoBuffer& _packed, uint32_t& _cmdid, uint32_t& _seq, size_t& _package_len, AutoBuffer& _body, AutoBuffer& _extension, longlink_tracker* _tracker)
        = [](const AutoBuffer& _packed, uint32_t& _cmdid, uint32_t& _seq, size_t& _package_len, AutoBuffer& _body, AutoBuffer& _extension, longlink_tracker* _tracker) {
            size_t len = _packed.Length();
            if (len < 2) {
                xwarn2(TSF"unpack: size less 2, %_", len);
                return LONGLINK_UNPACK_CONTINUE;
            }
            mars::stn::CmdHeader header(((unsigned char*)_packed.Ptr())[0]);
            xinfo2(TSF"[wei] unpack: mqttcmd = %_, _package_len = %_", header.mqttcmd, len);
            switch (header.mqttcmd) {
                case PINGRESP:
                {
                    _package_len = len;
                    _cmdid = NOOP_CMDID;
                    _seq = Task::kNoopTaskID;
                    break;
                }
                case CONNACK:
                {
                    _body.Write(_packed);
                    _package_len = len;
                    _cmdid = CmdID_Connect;
                    _seq = Task::kLongLinkIdentifyCheckerTaskID;
                    break;
                }
                case PUBLISH:
                {
                    _cmdid = CmdID_Notify;
                    _package_len = len;
                    PublishCommand pubCmd(header);
                    pubCmd.decode(_packed);
                    _seq = PUSH_DATA_TASKID;
                    _body.Write(pubCmd.getTopic().c_str());
                    break;
                }
                case QUERYACK:
                {
                    _cmdid = CmdID_Pull;
                    _package_len = len;
                    _seq = TaskID::TaskID_Pull;
                    QueryAckCommand qryAckCmd(header);
                    qryAckCmd.decode(_packed);
                    _body.Write(qryAckCmd.getPayload(), qryAckCmd.getPayloadLength());
                    break;
                }
                case PUBACK:
                {
                    _cmdid = CmdID_Publish;
                    _package_len = len;
                    PublishAckCommand pubAckCmd(header);
                    pubAckCmd.decode(_packed);
                    _body.Write(pubAckCmd.getPayload(), pubAckCmd.getPayloadLength());
                    _seq = TaskID::TaskID_Publish;
                    break;
                }
                default:
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
            return 20 * 1000;
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
