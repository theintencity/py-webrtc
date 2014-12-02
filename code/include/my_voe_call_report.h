#ifndef MY_VOE_CALL_REPORT_H
#define MY_VOE_CALL_REPORT_H 1

#include <Python.h>
#include "common_types.h"

#include "my_macro.h"
#include "my_channel.h"

extern My_METHOD_ARGS(WebRtcVoiceEngine, WriteReportToFile);

extern My_PROTO_READONLY(WebRtcVoiceEngine, SpeechAndNoiseSummary);
extern My_PROTO_READONLY(WebRtcVoiceEngine, EchoMetricSummary);

extern My_METHOD(WebRtcChannel, ResetCallReportStatistics);

extern My_PROTO_READONLY(WebRtcChannel, RoundTripTimeSummary);
extern My_PROTO_READONLY(WebRtcChannel, DeadOrAliveSummary);

#endif
