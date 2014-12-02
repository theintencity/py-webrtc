#ifndef My_VOE_AP_H
#define My_VOE_AP_H 1

#include "my_macro.h"
#include "my_channel.h"

extern My_METHOD_ARGS_KWARGS(WebRtcChannel, SetRxVadObserver);

extern My_PROTO_READWRITE(WebRtcVoiceEngine, NsStatus);
extern My_PROTO_READWRITE(WebRtcVoiceEngine, AgcStatus);
extern My_PROTO_READWRITE(WebRtcVoiceEngine, AgcConfig);
extern My_PROTO_READWRITE(WebRtcVoiceEngine, EcStatus);
extern My_PROTO_READWRITE(WebRtcVoiceEngine, AecmMode);
extern My_PROTO_READWRITE(WebRtcVoiceEngine, MetricsStatus);
extern My_PROTO_READONLY(WebRtcVoiceEngine, SpeechMetrics);
extern My_PROTO_READONLY(WebRtcVoiceEngine, NoiseMetrics);
extern My_PROTO_READONLY(WebRtcVoiceEngine, EchoMetrics);
extern My_PROTO_WRITEONLY(WebRtcVoiceEngine, DebugRecording);
extern My_PROTO_READWRITE(WebRtcVoiceEngine, TypingDetectionStatus);

extern My_PROTO_READWRITE(WebRtcChannel, RxNsStatus);
extern My_PROTO_READWRITE(WebRtcChannel, RxAgcStatus);
extern My_PROTO_READWRITE(WebRtcChannel, RxAgcConfig);
extern My_PROTO_READONLY(WebRtcChannel, VoiceActivityIndicator);


#endif
