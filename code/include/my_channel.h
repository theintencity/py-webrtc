/*
 * Copyright (c) 2011, Kundan Singh. All Rights Reserved.
 * Copyright (c) 2014, Intencity Cloud Technologies <theintencity@gmail.com>.
 */

#ifndef MY_CHANNEL_H
#define MY_CHANNEL_H 1

#include <Python.h>
#include <structmember.h>

#include "common_types.h"
#include "my_voice_engine.h";

using namespace webrtc;

class RxVadCallback;
class TelephoneEventCallback;
class EncryptionCallback;
class MediaProcessCallback;

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
    int ch;
    WebRtcVoiceEngine *voe;
    RxVadCallback *rx_vad_callback;
    TelephoneEventCallback *telephone_event_callback;
    EncryptionCallback *encryption_callback;
    MediaProcessCallback *media_process_callback;
} WebRtcChannel;


extern int my_channel_preinit();
extern void my_channel_addobjects(PyObject* m);

#endif
