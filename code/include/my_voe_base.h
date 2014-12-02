/*
 * Copyright (c) 2011, Kundan Singh. All Rights Reserved.
 * Copyright (c) 2014, Intencity Cloud Technologies <theintencity@gmail.com>.
 */

#ifndef My_VOE_BASE_H
#define My_VOE_BASE_H 1

#include "my_channel.h"

extern My_METHOD_ARGS_KWARGS(WebRtcVoiceEngine, RegisterVoiceEngineObserver);

extern My_METHOD_ARGS_KWARGS(WebRtcChannel, SetLocalReceiver);
extern My_METHOD_ARGS_KWARGS(WebRtcChannel, SetSendDestination);
extern My_METHOD(WebRtcChannel, StartReceive);
extern My_METHOD(WebRtcChannel, StopReceive);
extern My_METHOD(WebRtcChannel, StartPlayout);
extern My_METHOD(WebRtcChannel, StopPlayout);
extern My_METHOD(WebRtcChannel, StartSend);
extern My_METHOD(WebRtcChannel, StopSend);

extern My_PROTO_READONLY(WebRtcVoiceEngine, Version);
extern My_PROTO_WRITEONLY(WebRtcVoiceEngine, TraceFile);
extern My_PROTO_READONLY(WebRtcVoiceEngine, LastError);
extern My_PROTO_READONLY(WebRtcVoiceEngine, MaxNumOfChannels);

extern My_PROTO_READONLY(WebRtcChannel, ID);
extern My_PROTO_READONLY(WebRtcChannel, LocalReceiver);
extern My_PROTO_READONLY(WebRtcChannel, SendDestination);
extern My_PROTO_READWRITE(WebRtcChannel, OnHoldStatus);
extern My_PROTO_READWRITE(WebRtcChannel, NetEQPlayoutMode);
extern My_PROTO_READWRITE(WebRtcChannel, NetEQBGNMode);


#endif
