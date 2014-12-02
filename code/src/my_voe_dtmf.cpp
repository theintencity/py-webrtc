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
#include "my_voe_dtmf.h"

using namespace webrtc;


class TelephoneEventCallback: public VoETelephoneEventObserver {
public:
    TelephoneEventCallback(PyObject* callback, PyObject* userdata);
    ~TelephoneEventCallback();
    void OnReceivedTelephoneEventInband(const int channel, const unsigned char eventCode, const bool endOfEvent);
    void OnReceivedTelephoneEventOutOfBand(const int channel, const unsigned char eventCode, const bool endOfEvent);
    
private:
    PyObject *callback;
    PyObject *userdata;
};

TelephoneEventCallback::TelephoneEventCallback(PyObject* callback, PyObject* userdata) {
    Py_XINCREF(callback);
    this->callback = callback;
    Py_XINCREF(userdata);
    this->userdata = userdata;
}

TelephoneEventCallback::~TelephoneEventCallback() {
    Py_XDECREF(this->callback);
    Py_XDECREF(this->userdata);
}

void TelephoneEventCallback::OnReceivedTelephoneEventInband(const int channel, const unsigned char eventCode, const bool endOfEvent)
{
    if (this->callback != NULL) {
        PyObject* arglist = Py_BuildValue("(OicOO)", this->userdata, channel, eventCode, PyBool_FromLong(endOfEvent), PyBool_FromLong(true));
        PyObject* output = PyObject_CallObject(this->callback, arglist);
        Py_DECREF(arglist);
    }
}

void TelephoneEventCallback::OnReceivedTelephoneEventOutOfBand(const int channel, const unsigned char eventCode, const bool endOfEvent)
{
    if (this->callback != NULL) {
        PyObject* arglist = Py_BuildValue("(OicOO)", this->userdata, channel, eventCode, PyBool_FromLong(endOfEvent), PyBool_FromLong(false));
        PyObject* output = PyObject_CallObject(this->callback, arglist);
        Py_DECREF(arglist);
    }
}



My_METHOD_ARGS_KWARGS(WebRtcVoiceEngine, PlayDtmfTone)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->dtmf, VoEDtmf)
    
    unsigned char eventCode;
    int lengthMs = 200;
    int attenuationDb = 10;
    static char *kwlist[] = {"event_code", "length_ms", "attenuation_db", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "c|ii", kwlist, &eventCode, &lengthMs, &attenuationDb)) {
        return NULL;
    }
    
    int er = self->dtmf->PlayDtmfTone(eventCode, lengthMs, attenuationDb);
    My_NULL_IF_NEG(er, self->base)
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_METHOD_ARGS_KWARGS(WebRtcVoiceEngine, StartPlayingDtmfTone)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->dtmf, VoEDtmf)
    
    unsigned char eventCode;
    int attenuationDb = 10;
    static char *kwlist[] = {"event_code", "attenuation_db", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "c|i", kwlist, &eventCode, &attenuationDb)) {
        return NULL;
    }
    
    int er = self->dtmf->StartPlayingDtmfTone(eventCode, attenuationDb);
    My_NULL_IF_NEG(er, self->base)
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_METHOD(WebRtcVoiceEngine, StopPlayingDtmfTone)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->dtmf, VoEDtmf)
    
    int er = self->dtmf->StopPlayingDtmfTone();
    My_NULL_IF_NEG(er, self->base)
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, DtmfFeedbackStatus)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->dtmf, VoEDtmf)
    
    bool enable, directFeedback;
    int er = self->dtmf->GetDtmfFeedbackStatus(enable, directFeedback);
    My_NULL_IF_NEG(er, self->base)
    
    return Py_BuildValue("O", PyBool_FromLong(enable));
}

My_PROPERTY_SETTER(WebRtcVoiceEngine, DtmfFeedbackStatus)
{
    My_ERROR_IF_NULL(self->base, VoEBase)
    My_ERROR_IF_NULL(self->dtmf, VoEDtmf)

    if (!PyBool_Check(value)) {
        PyErr_SetString(WebRtcError, "must be a bool");
        return -1;
    }
    
    bool enable = (value == Py_True);
    int er = self->dtmf->SetDtmfFeedbackStatus(enable);
    My_ERROR_IF_NEG(er, self->base)
    
    return 0;   
}

My_METHOD_ARGS_KWARGS(WebRtcChannel, SendTelephoneEvent)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->dtmf, VoEDtmf)
    My_NULL_IF_NEG2(self->ch)
    
    unsigned char eventCode;
    bool outOfBand = true;
    int lengthMs = 160;
    int attenuationDb = 10;
    static char *kwlist[] = {"event_code", "out_of_band", "length_ms", "attenuation_db", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "c|iii", kwlist, &eventCode, &outOfBand, &lengthMs, &attenuationDb)) {
        return NULL;
    }
    
    int er = self->voe->dtmf->SendTelephoneEvent(self->ch, eventCode, outOfBand, lengthMs, attenuationDb);
    My_NULL_IF_NEG(er, self->voe->base)
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_METHOD_ARGS_KWARGS(WebRtcChannel, RegisterTelephoneEventDetection)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->dtmf, VoEDtmf)
    My_NULL_IF_NEG2(self->ch)
    
    PyObject* callback = NULL;
    PyObject* userdata = Py_None;
    const char* detection_method_str = NULL;
    static char *kwlist[] = {"callback", "detection_method", "userdata", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OO|O", kwlist, &callback, &detection_method_str, &userdata)) {
        return NULL;
    }

    if (callback != Py_None && !PyCallable_Check(callback)) {
        PyErr_SetString(WebRtcError, "callback must be callable or None");
        return NULL;
    }
    
    int detection_method = My_STRING2ATTR(detection_method_str, TelephoneEventDetectionMethods);
    My_NULL_IF_NEG3(detection_method, detection_method_str)
    
    if (self->telephone_event_callback != NULL) {
        delete self->telephone_event_callback;
        self->telephone_event_callback = NULL;
    }
    
    if (callback != Py_None) {
        self->telephone_event_callback = new TelephoneEventCallback(callback, userdata);
        self->voe->dtmf->RegisterTelephoneEventDetection(self->ch,
                            (TelephoneEventDetectionMethods) detection_method, *self->telephone_event_callback);
    }
    else {
        self->voe->dtmf->DeRegisterTelephoneEventDetection(self->ch);
    }
    
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_GETTER(WebRtcChannel, SendTelephoneEventPayloadType)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->dtmf, VoEDtmf)
    My_NULL_IF_NEG2(self->ch)
    
    unsigned char status;
    int er = self->voe->dtmf->GetSendTelephoneEventPayloadType(self->ch, status);
    My_NULL_IF_NEG(er, self->voe->base)
    
    return Py_BuildValue("i", (int) status);
}

My_PROPERTY_SETTER(WebRtcChannel, SendTelephoneEventPayloadType)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->dtmf, VoEDtmf)
    My_ERROR_IF_NEG2(self->ch)
    My_INT(value, valueint)
    
    int er = self->voe->dtmf->SetSendTelephoneEventPayloadType(self->ch, (unsigned char) valueint);
    My_ERROR_IF_NEG(er, self->voe->base)
    
    return 0;
}

My_PROPERTY_GETTER(WebRtcChannel, DtmfPlayoutStatus)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->dtmf, VoEDtmf)
    My_NULL_IF_NEG2(self->ch)
    
    bool status;
    int er = self->voe->dtmf->GetDtmfPlayoutStatus(self->ch, status);
    My_NULL_IF_NEG(er, self->voe->base)
    
    return PyBool_FromLong(status);
}

My_PROPERTY_SETTER(WebRtcChannel, DtmfPlayoutStatus)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->dtmf, VoEDtmf)
    My_ERROR_IF_NEG2(self->ch)
    
    if (!PyBool_Check(value)) {
        PyErr_SetString(WebRtcError, "must be a bool");
        return -1;
    }
    
    bool valuebool = (value == Py_True);
    int er = self->voe->dtmf->SetDtmfPlayoutStatus(self->ch, valuebool);
    My_ERROR_IF_NEG(er, self->voe->base)
    
    return 0;
}

My_PROPERTY_GETTER(WebRtcChannel, TelephoneEventDetectionStatus)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->dtmf, VoEDtmf)
    My_NULL_IF_NEG2(self->ch)
    
    bool enabled;
    TelephoneEventDetectionMethods detectionMethod;
    int er = self->voe->dtmf->GetTelephoneEventDetectionStatus(self->ch, enabled, detectionMethod);
    My_NULL_IF_NEG(er, self->voe->base)
    
    if (enabled) {
        const char* valuestr = My_ATTR2STRING(detectionMethod, TelephoneEventDetectionMethods);
        return Py_BuildValue("s", valuestr);
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}
