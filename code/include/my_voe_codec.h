#ifndef MY_VOE_CODEC_H
#define MY_VOE_CODEC_H 1

#include <Python.h>
#include "common_types.h"
#include "voe_codec.h"

#include "my_macro.h"

extern PyObject* codec2object(webrtc::CodecInst& data);
extern int object2codec(PyObject* object, webrtc::CodecInst&data);

extern My_PROTO_READONLY(WebRtcVoiceEngine, NumOfCodecs);
extern My_PROTO_READONLY(WebRtcVoiceEngine, Codecs);

extern My_PROTO_READWRITE(WebRtcChannel, SendCodec);
extern My_PROTO_READONLY(WebRtcChannel, RecCodec);
extern My_PROTO_WRITEONLY(WebRtcChannel, ISACInitTargetRate);
extern My_PROTO_WRITEONLY(WebRtcChannel, ISACMaxRate);
extern My_PROTO_WRITEONLY(WebRtcChannel, ISACMaxPayloadSize);
extern My_PROTO_READWRITE(WebRtcChannel, RecPayloadType);
extern My_PROTO_WRITEONLY(WebRtcChannel, SendCNPayloadType);
extern My_PROTO_READWRITE(WebRtcChannel, VADStatus);
//extern My_PROTO_WRITEONLY(WebRtcChannel, AMREncFormat);
//extern My_PROTO_WRITEONLY(WebRtcChannel, AMRDecFormat);
//extern My_PROTO_WRITEONLY(WebRtcChannel, AMRWbEncFormat);
//extern My_PROTO_WRITEONLY(WebRtcChannel, AMRWbDecFormat);

#endif
