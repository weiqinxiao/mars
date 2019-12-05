//
//  StnCallback.cpp
//  MyDemo
//
//  Created by weiqinxiao on 2019/11/14.
//  Copyright © 2019 club.linkyou. All rights reserved.
//

#include "StnCallBack.hpp"
#include "mars/comm/autobuffer.h"
#include "mars/comm/xlogger/xlogger.h"
#include "mars/stn/stn.h"
#include "AbstractCommand.hpp"
#include "PublishCommand.hpp"
#include "PullMessageTask.hpp"
#include "ConnectivityLogic.hpp"
#include "QueryCommand.hpp"
#include "MessageTopic.h"
#include "MessageService.hpp"
#include "PublishMessageTask.hpp"

namespace mars {
    namespace stn {
        StnCallBack* StnCallBack::instance_ = NULL;
        
        StnCallBack* StnCallBack::Instance() {
            if(instance_ == NULL) {
                instance_ = new StnCallBack();
            }
            
            return instance_;
        }
        
        void StnCallBack::Release() {
            delete instance_;
            instance_ = NULL;
        }
        
        bool StnCallBack::MakesureAuthed(const std::string& host) {
            xinfo2(TSF"MakesureAuthed: %_", host);
            return true;
        }
        
        
        void StnCallBack::TrafficData(ssize_t _send, ssize_t _recv) {
            xdebug2(TSF"send:%_, recv:%_", _send, _recv);
        }
        
        std::vector<std::string> StnCallBack::OnNewDns(const std::string& _host) {
            xinfo2(TSF"dns: %_", _host);
            std::vector<std::string> vector;
            vector.push_back(_host);
            return vector;//TODO
        }
        
        void StnCallBack::OnPush(uint64_t _channel_id, uint32_t _cmdid, uint32_t _taskid, const AutoBuffer& _body, const AutoBuffer& _extend) {
            xinfo2(TSF"[wei] _cmdid = %_, taskId = %_", _cmdid, _taskid);
            if (_cmdid == CmdID_Notify) {
                char* topic = (char*)(_body.Ptr());
                std::string str(Topic_notify);
                if (topic == str) {
                    MessageService::Instance()->syncMessage();
                    xinfo2(TSF"[wei] start task TaskID_PullMsg");
                }
            }
        }
        
        bool StnCallBack::Req2Buf(uint32_t _taskid, void* const _user_context, AutoBuffer& _outbuffer, AutoBuffer& _extend, int& _error_code, const int _channel_select, const std::string& host) {
            xinfo2(TSF"[wei] taskid = %_", _taskid);
            if (_taskid == TaskID::TaskID_PullMsg) {
                PullMessageTask *pullTask = (PullMessageTask *)_user_context;
                pullTask->encodeMessage(ConnectivityLogic::Instance()->getPBEnv());
                _outbuffer.Write(pullTask->getData(), pullTask->getDataLen());
                _extend.Write(pullTask->getTopic());
            } else if (_taskid == TaskID::TaskID_PublishMsg) {
                PublishMessageTask *pubTask = (PublishMessageTask*)_user_context;
                _outbuffer.Write(pubTask->getData(), pubTask->getDataLen());
            }
            return true;
        }
        
        int StnCallBack::Buf2Resp(uint32_t _taskid, void* const _user_context, const AutoBuffer& _inbuffer, const AutoBuffer& _extend, int& _error_code, const int _channel_select) {
            xinfo2(TSF"[wei] _taskid = %_, channel = %_", _taskid, _channel_select);
            if (_taskid == TaskID::TaskID_PullMsg) {
                PullMessageTask *pullTask = (PullMessageTask *)_user_context;
                AutoBuffer body;
                body.AllocWrite(_inbuffer.Length());
                body.Write(_inbuffer);
                body.Seek(0, AutoBuffer::TSeek::ESeekStart);
                pullTask->decodeMessage(ConnectivityLogic::Instance()->getPBEnv(), body);
                MessageService::Instance()->updateSyncTime(pullTask->getLastSentTime(), pullTask->getLastReceiveTime());
                ReceiveMessageListener *rcvMsgListener = MessageService::Instance()->getReceiveMessageListener();
                if (rcvMsgListener && pullTask->getPulledMessages().size() > 0) {
                    rcvMsgListener->onReceivedMessages(pullTask->getPulledMessages());
                }
                if (!pullTask->isFinishPull()) {
                    MessageService::Instance()->syncMessage();
                }
            } else if (_taskid == TaskID_PublishMsg) {
                PublishMessageTask *pubTask = (PublishMessageTask*)_user_context;
                AutoBuffer body;
                body.AllocWrite(_inbuffer.Length());
                body.Write(_inbuffer);
                body.Seek(0, AutoBuffer::TSeek::ESeekStart);
                pubTask->decodeMessage(ConnectivityLogic::Instance()->getPBEnv(), body);
                //TODO 时间？
            }
            return 0;
        }
        
        int StnCallBack::OnTaskEnd(uint32_t _taskid, void* const _user_context, int _error_type, int _error_code) {
            xinfo2(TSF"[wei] delete task: taskid = %_", _taskid);
            if (_user_context) {
                if (_taskid == TaskID_PublishMsg) {
                    PublishMessageTask *task = (PublishMessageTask*)_user_context;
                    task->callback()->onSuccess(task->getMessageUID());
                    delete task;
                } else {
                    ProtoTask *task = (ProtoTask*)_user_context;
                    delete task;
                }
            }
            return 0;
        }
        
        void StnCallBack::ReportConnectStatus(int _status, int longlink_status) {
            xinfo2(TSF"[wei] connection status changed: %_, %_", _status, longlink_status);
            listener_->onConnectionStatusChanged(longlink_status);            
        }
        
        // synccheck：长链成功后由网络组件触发
        // 需要组件组包，发送一个req过去，网络成功会有resp，但没有taskend，处理事务时要注意网络时序
        // 不需组件组包，使用长链做一个sync，不用重试
        int  StnCallBack::GetLonglinkIdentifyCheckBuffer(AutoBuffer& _identify_buffer, AutoBuffer& _buffer_hash, int32_t& _cmdid) {
            xinfo2(TSF"[wei] cmdid = %_", _cmdid);
            _cmdid = CmdID_Connect;
            return IdentifyMode::kCheckNow;
        }
        
        bool StnCallBack::OnLonglinkIdentifyResponse(const AutoBuffer& _response_buffer, const AutoBuffer& _identify_buffer_hash) {
            unsigned char flag = ((unsigned char*)_response_buffer.Ptr())[0];
            mars::stn::CmdHeader header(flag);
            bool accpted = ConnectivityLogic::Instance()->onConnectAck(header, _response_buffer);
            xinfo2(TSF"[wei] accpted = %_", accpted);
            if (accpted) {
                MessageService::Instance()->syncMessage();
            } else {
                
            }
            return accpted;
        }
        //
        void StnCallBack::RequestSync() {
            
        }
        
    }
}
