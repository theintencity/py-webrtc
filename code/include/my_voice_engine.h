#ifndef MY_VOICE_ENGINE_H
#define MY_VOICE_ENGINE_H 1

#include <Python.h>
#include <structmember.h>

#include "voe_base.h"
#include "voe_audio_processing.h"
#include "voe_codec.h"
#include "voe_call_report.h"
#include "voe_dtmf.h"
#include "voe_encryption.h"
#include "voe_external_media.h"

using namespace webrtc;

class VoiceEngineCallback;

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
    VoiceEngine *voe;
    
    VoEBase *base;
    VoEAudioProcessing *ap;
    VoECodec *codec;
    VoECallReport *call_report;
    VoEDtmf *dtmf;
    VoEEncryption *encryption;
    VoEExternalMedia *external_media;
    
    VoiceEngineCallback *callback;
} WebRtcVoiceEngine;


extern int my_voice_engine_preinit();
extern void my_voice_engine_addobjects(PyObject* m);

#endif
