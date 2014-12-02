#ifndef MY_VOE_EXTERNAL_MEDIA_H
#define MY_VOE_EXTERNAL_MEDIA_H 1

#include <Python.h>
#include "common_types.h"

#include "my_macro.h"
#include "my_channel.h"

extern My_METHOD_ARGS_KWARGS(WebRtcVoiceEngine, ExternalRecordingInsertData);
extern My_METHOD_ARGS_KWARGS(WebRtcVoiceEngine, ExternalPlayoutGetData);

extern My_PROTO_WRITEONLY(WebRtcVoiceEngine, ExternalRecordingStatus);
extern My_PROTO_WRITEONLY(WebRtcVoiceEngine, ExternalPlayoutStatus);


extern My_METHOD_ARGS_KWARGS(WebRtcChannel, RegisterExternalMediaProcessing);


#endif
