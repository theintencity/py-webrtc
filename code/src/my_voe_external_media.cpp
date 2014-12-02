/*
 * Copyright (c) 2011, Kundan Singh. All Rights Reserved.
 * Copyright (c) 2014, Intencity Cloud Technologies <theintencity@gmail.com>.
 */

#include <Python.h>

#include "common_types.h"

#include "my_macro.h"
#include "my_error.h"
#include "my_common_types.h"
#include "my_voice_engine.h"
#include "my_channel.h"
#include "my_voe_external_media.h"

using namespace webrtc;


class MediaProcessCallback: public VoEMediaProcess {
public:
    MediaProcessCallback(PyObject* callback, PyObject* userdata);
    ~MediaProcessCallback();
    void Process(const int channel, const ProcessingTypes type, WebRtc_Word16 audio10ms[], const int length, const int samplingFreq, const bool isStereo);
    
private:
    PyObject *callback;
    PyObject *userdata;
};

MediaProcessCallback::MediaProcessCallback(PyObject* callback, PyObject* userdata)
{
    Py_XINCREF(callback);
    this->callback = callback;
    Py_XINCREF(userdata);
    this->userdata = userdata;
}

MediaProcessCallback::~MediaProcessCallback() {
    Py_XDECREF(this->callback);
    Py_XDECREF(this->userdata);
}

void MediaProcessCallback::Process(const int channel, const ProcessingTypes type, WebRtc_Word16 audio10ms[], const int length, const int samplingFreq, const bool isStereo)
{
    if (this->callback != NULL) {
        PyObject* input = PyString_FromStringAndSize((const char*) audio10ms, length*2);
        PyObject* arglist = Py_BuildValue("(OiOiO)", this->userdata, channel, input, samplingFreq, PyBool_FromLong(isStereo));
        PyObject* output = PyObject_CallObject(this->callback, arglist);
        Py_DECREF(arglist);
        if (PyString_Check(output)) {
            memcpy(audio10ms, PyString_AsString(output), length*2);
        }
        Py_DECREF(output);
    }
}


My_METHOD_ARGS_KWARGS(WebRtcChannel, RegisterExternalMediaProcessing)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->encryption, VoEEncryption)
    My_NULL_IF_NEG2(self->ch)
    
    PyObject* callback = NULL;
    const char* typestr = NULL;
    PyObject* userdata = Py_None;
    static char *kwlist[] = {"type", "callback", "userdata", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "sO|O", kwlist, &typestr, &callback, &userdata)) {
        return NULL;
    }

    if (callback != Py_None && !PyCallable_Check(callback)) {
        PyErr_SetString(WebRtcError, "callback must be callable or None");
        return NULL;
    }
    
    int type = My_STRING2ATTR(typestr, ProcessingTypes);
    My_NULL_IF_NEG3(type, typestr)
    
    if (self->media_process_callback != NULL) {
        delete self->media_process_callback;
        self->media_process_callback = NULL;
    }
    
    if (callback != Py_None) {
        self->media_process_callback = new MediaProcessCallback(callback, userdata);
        self->voe->external_media->RegisterExternalMediaProcessing(self->ch, (ProcessingTypes) type, *self->media_process_callback);
    }
    else {
        self->voe->external_media->DeRegisterExternalMediaProcessing(self->ch, (ProcessingTypes) type);
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}



My_METHOD_ARGS_KWARGS(WebRtcVoiceEngine, ExternalRecordingInsertData)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->external_media, VoEExternalMedia)
    
    PyObject* speech_data_o = NULL;
    int sampling_frequency = -1;
    int current_delay_ms = -1;
    static char *kwlist[] = {"speech_data_10ms", "sampling_frequency", "current_delay_ms", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "Oii", kwlist, &speech_data_o, &sampling_frequency, &current_delay_ms)) {
        return NULL;
    }
    if (!PyString_Check(speech_data_o)) {
        PyErr_SetString(WebRtcError, "speech_data_10ms must be a string");
        return NULL;
    }
    
    const char* speech_data = PyString_AsString(speech_data_o);
    int length_samples = PyString_Size(speech_data_o) / 2; // assuming 16 bit samples
    
    int er = self->external_media->ExternalRecordingInsertData((const WebRtc_Word16*) speech_data, length_samples, sampling_frequency, current_delay_ms);
    My_NULL_IF_NEG(er, self->base)
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_METHOD_ARGS_KWARGS(WebRtcVoiceEngine, ExternalPlayoutGetData)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->external_media, VoEExternalMedia)
    
    int sampling_frequency = -1;
    int current_delay_ms = -1;
    static char *kwlist[] = {"sampling_frequency", "current_delay_ms", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ii", kwlist, &sampling_frequency, &current_delay_ms)) {
        return NULL;
    }
    
    WebRtc_Word16 speech_data[480];
    int length_samples = 0;
    
    int er = self->external_media->ExternalRecordingInsertData((const WebRtc_Word16*) speech_data, sampling_frequency, current_delay_ms, length_samples);
    My_NULL_IF_NEG(er, self->base)
    
    if (length_samples > 0) {
        PyObject* speech_data_o = PyString_FromStringAndSize((const char*) speech_data, length_samples*2);
        return speech_data_o;
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}


My_PROPERTY_SETTER(WebRtcVoiceEngine, ExternalRecordingStatus)
{
    My_ERROR_IF_NULL(self->base, VoEBase)
    My_ERROR_IF_NULL(self->external_media, VoEExternalMedia)

    if (!PyBool_Check(value)) {
        PyErr_SetString(WebRtcError, "must be a bool");
        return -1;
    }
    
    bool enable = (value == Py_True);
    int er = self->external_media->SetExternalRecordingStatus(enable);
    My_ERROR_IF_NEG(er, self->base)
    
    return 0;   
}


My_PROPERTY_SETTER(WebRtcVoiceEngine, ExternalPlayoutStatus)
{
    My_ERROR_IF_NULL(self->base, VoEBase)
    My_ERROR_IF_NULL(self->external_media, VoEExternalMedia)

    if (!PyBool_Check(value)) {
        PyErr_SetString(WebRtcError, "must be a bool");
        return -1;
    }
    
    bool enable = (value == Py_True);
    int er = self->external_media->SetExternalPlayoutStatus(enable);
    My_ERROR_IF_NEG(er, self->base)
    
    return 0;   
}



