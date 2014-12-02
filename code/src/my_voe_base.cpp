/*
 * Copyright (c) 2011, Kundan Singh. All Rights Reserved.
 * Copyright (c) 2014, Intencity Cloud Technologies <theintencity@gmail.com>.
 */

#include <Python.h>

#include "common_types.h"

#include "my_macro.h"
#include "my_error.h"
#include "my_common_types.h"
#include "my_channel.h"
#include "my_voe_base.h"

using namespace webrtc;

class VoiceEngineCallback: VoiceEngineObserver {
public:
    VoiceEngineCallback(PyObject* callback, PyObject* userdata);
    ~VoiceEngineCallback();
    void CallbackOnError(const int channel, const int errCode);
private:
    PyObject *callback;
    PyObject *userdata;
};

VoiceEngineCallback::VoiceEngineCallback(PyObject* callback, PyObject* userdata) {
    Py_XINCREF(callback);
    this->callback = callback;
    Py_XINCREF(userdata);
    this->userdata = userdata;
}

VoiceEngineCallback::~VoiceEngineCallback() {
    Py_XDECREF(this->callback);
    Py_XDECREF(this->userdata);
}

void VoiceEngineCallback::CallbackOnError(const int channel, const int errCode) {
    if (this->callback != NULL) {
        PyObject* arglist = Py_BuildValue("(Ois)", this->userdata, channel, Error2String(errCode));
        PyObject* output = PyObject_CallObject(this->callback, arglist);
        Py_DECREF(arglist);
    }
}


My_PROPERTY_SETTER(WebRtcVoiceEngine, TraceFile)
{
    My_ERROR_IF_NULL(self->voe, VoEVoiceEngine)
    My_ERROR_IF_NULL(self->base, VoEBase)
    My_STRING_OR_NONE(value, valuestr)
    
    int er = self->voe->SetTraceFile(valuestr);
    My_ERROR_IF_NEG(er, self->base)
    
    return 0;
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, MaxNumOfChannels)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    int ch = self->base->MaxNumOfChannels();
    return PyInt_FromLong(ch);
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, Version)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    char version[1024];
    int er = self->base->GetVersion(version);
    My_NULL_IF_NEG(er, self->base)
    return PyString_FromString(version);
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, LastError)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    int er = self->base->LastError();
    return PyString_FromFormat("%d %s", er, Error2String(er));
}


My_METHOD_ARGS_KWARGS(WebRtcVoiceEngine, RegisterVoiceEngineObserver)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    
    PyObject* callback = NULL;
    PyObject* userdata = Py_None;
    static char *kwlist[] = {"callback", "userdata", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|O", kwlist, &callback, &userdata)) {
        return NULL;
    }

    if (callback != Py_None && !PyCallable_Check(callback)) {
        PyErr_SetString(WebRtcError, "callback must be callable or None");
        return NULL;
    }
    
    if (self->callback != NULL) {
        delete self->callback;
        self->callback = NULL;
    }
    
    if (callback != Py_None) {
        self->callback = new VoiceEngineCallback(callback, userdata);
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}





My_METHOD_ARGS_KWARGS(WebRtcChannel, SetLocalReceiver)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NEG2(self->ch)
    
    int port = -1;
    int rtcp_port = kVoEDefault;
    const char* ip = NULL;
    const char* multicast = NULL;
    static char *kwlist[] = {"port", "rtcp_port", "ip", "multicast", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i|iss", kwlist, &port, &rtcp_port, &ip, &multicast)) {
        return NULL;
    }
    
    int er = self->voe->base->SetLocalReceiver(self->ch, port, rtcp_port, ip, multicast);
    My_NULL_IF_NEG(er, self->voe->base)
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_GETTER(WebRtcChannel, LocalReceiver)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NEG2(self->ch)
    
    int port = -1;
    int rtcp_port = -1;
    char ip[64];
    int er = self->voe->base->GetLocalReceiver(self->ch, port, rtcp_port, ip);
    My_NULL_IF_NEG(er, self->voe->base)
    
    PyObject* result = PyTuple_New(3);
    PyTuple_SetItem(result, 0, PyString_FromString(ip));
    PyTuple_SetItem(result, 1, PyInt_FromLong(port));
    PyTuple_SetItem(result, 2, PyInt_FromLong(rtcp_port));
    
    return Py_BuildValue("N", result);
}

My_METHOD_ARGS_KWARGS(WebRtcChannel, SetSendDestination)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NEG2(self->ch)
    
    int port = -1;
    int source_port = kVoEDefault;
    int rtcp_port = kVoEDefault;
    const char* ip = NULL;
    static char *kwlist[] = {"port", "ip", "source_port", "rtcp_port", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "is|ii", kwlist, &port, &ip, &source_port, &rtcp_port)) {
        return NULL;
    }
    
    int er = self->voe->base->SetSendDestination(self->ch, port, ip, source_port, rtcp_port);
    My_NULL_IF_NEG(er, self->voe->base)
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_GETTER(WebRtcChannel, SendDestination)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NEG2(self->ch)
    
    int port = -1;
    char ip[64];
    int source_port = -1;
    int rtcp_port = -1;
    int er = self->voe->base->GetSendDestination(self->ch, port, ip, source_port, rtcp_port);
    My_NULL_IF_NEG(er, self->voe->base)
    
    PyObject* result = PyTuple_New(4);
    PyTuple_SetItem(result, 0, PyString_FromString(ip));
    PyTuple_SetItem(result, 1, PyInt_FromLong(port));
    PyTuple_SetItem(result, 2, PyInt_FromLong(rtcp_port));
    PyTuple_SetItem(result, 3, PyInt_FromLong(source_port));
    
    return Py_BuildValue("N", result);
}

My_METHOD(WebRtcChannel, StartReceive)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NEG2(self->ch)
    
    int er = self->voe->base->StartReceive(self->ch);
    My_NULL_IF_NEG(er, self->voe->base)
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_METHOD(WebRtcChannel, StopReceive)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NEG2(self->ch)

    int er = self->voe->base->StopReceive(self->ch);
    My_NULL_IF_NEG(er, self->voe->base)
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_METHOD(WebRtcChannel, StartPlayout)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NEG2(self->ch)
    
    int er = self->voe->base->StartPlayout(self->ch);
    My_NULL_IF_NEG(er, self->voe->base)
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_METHOD(WebRtcChannel, StopPlayout)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NEG2(self->ch)

    int er = self->voe->base->StopPlayout(self->ch);
    My_NULL_IF_NEG(er, self->voe->base)
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_METHOD(WebRtcChannel, StartSend)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NEG2(self->ch)

    int er = self->voe->base->StartSend(self->ch);
    My_NULL_IF_NEG(er, self->voe->base)
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_METHOD(WebRtcChannel, StopSend)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NEG2(self->ch)

    int er = self->voe->base->StopSend(self->ch);
    My_NULL_IF_NEG(er, self->voe->base)
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_GETTER(WebRtcChannel, OnHoldStatus)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NEG2(self->ch)
    
    bool enabled;
    OnHoldModes status;
    int er = self->voe->base->GetOnHoldStatus(self->ch, enabled, status);
    My_NULL_IF_NEG(er, self->voe->base);
    
    if (enabled) {
        const char *valuestr = My_ATTR2STRING(status, OnHoldModes);
        if (valuestr != NULL) {
            return Py_BuildValue("s", valuestr);
        }
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_SETTER(WebRtcChannel, OnHoldStatus)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NEG2(self->ch)
    My_STRING_OR_NONE(value, valuestr)
    
    int er;
    if (valuestr == NULL) {
        er = self->voe->base->SetOnHoldStatus(self->ch, false);
    }
    else {
        int status = My_STRING2ATTR(valuestr, OnHoldModes);
        My_ERROR_IF_NEG3(status, valuestr)
        er = self->voe->base->SetOnHoldStatus(self->ch, true, (OnHoldModes) status);
    }
    My_ERROR_IF_NEG(er, self->voe->base)
    return 0;
}

My_PROPERTY_GETTER(WebRtcChannel, NetEQPlayoutMode)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NEG2(self->ch)
    
    NetEqModes status;
    int er = self->voe->base->GetNetEQPlayoutMode(self->ch, status);
    My_NULL_IF_NEG(er, self->voe->base)
    
    const char *valuestr = My_ATTR2STRING(status, NetEqModes);
    if (valuestr != NULL) {
        return Py_BuildValue("s", valuestr);
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_SETTER(WebRtcChannel, NetEQPlayoutMode)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NEG2(self->ch)
    My_STRING(value, valuestr)
    
    int status = My_STRING2ATTR(valuestr, NetEqModes);
    My_ERROR_IF_NEG3(status, valuestr)
    
    int er = self->voe->base->SetNetEQPlayoutMode(self->ch, (NetEqModes) status);
    My_ERROR_IF_NEG(er, self->voe->base)
    
    return 0;
}

My_PROPERTY_GETTER(WebRtcChannel, NetEQBGNMode)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NEG2(self->ch)
    
    NetEqBgnModes status;
    int er = self->voe->base->GetNetEQBGNMode(self->ch, status);
    My_NULL_IF_NEG(er, self->voe->base)
    
    const char *valuestr = My_ATTR2STRING(status, NetEqBgnModes);
    if (valuestr != NULL) {
        return Py_BuildValue("s", valuestr);
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_SETTER(WebRtcChannel, NetEQBGNMode)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NEG2(self->ch)
    My_STRING(value, valuestr)
    
    int status = My_STRING2ATTR(valuestr, NetEqBgnModes);
    My_ERROR_IF_NEG3(status, valuestr)
    
    int er = self->voe->base->SetNetEQBGNMode(self->ch, (NetEqBgnModes) status);
    My_ERROR_IF_NEG(er, self->voe->base)
    
    return 0;
}

