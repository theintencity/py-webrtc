/*
 * Copyright (c) 2011, Kundan Singh. All Rights Reserved.
 * Copyright (c) 2014, Intencity Cloud Technologies <theintencity@gmail.com>.
 */

#ifndef MY_VOE_DTMF_H
#define MY_VOE_DTMF_H 1

#include <Python.h>
#include "common_types.h"

#include "my_macro.h"
#include "my_channel.h"


extern My_METHOD_ARGS_KWARGS(WebRtcVoiceEngine, PlayDtmfTone);
extern My_METHOD_ARGS_KWARGS(WebRtcVoiceEngine, StartPlayingDtmfTone);
extern My_METHOD(WebRtcVoiceEngine, StopPlayingDtmfTone);

extern My_PROTO_READWRITE(WebRtcVoiceEngine, DtmfFeedbackStatus);

extern My_METHOD_ARGS_KWARGS(WebRtcChannel, SendTelephoneEvent);
extern My_METHOD_ARGS_KWARGS(WebRtcChannel, RegisterTelephoneEventDetection);
    
extern My_PROTO_READWRITE(WebRtcChannel, SendTelephoneEventPayloadType);
extern My_PROTO_READWRITE(WebRtcChannel, DtmfPlayoutStatus);
extern My_PROTO_READONLY(WebRtcChannel, TelephoneEventDetectionStatus);

#endif
