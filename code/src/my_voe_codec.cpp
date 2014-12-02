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
#include "my_voe_codec.h"

using namespace webrtc;

PyObject*
codec2object(CodecInst& data)
{
    PyObject* result = PyDict_New();
    PyDict_SetItemString(result, "type", PyInt_FromLong(data.pltype));
    PyDict_SetItemString(result, "name", PyString_FromString(data.plname));
    PyDict_SetItemString(result, "frequency", PyInt_FromLong(data.plfreq));
    PyDict_SetItemString(result, "size", PyInt_FromLong(data.pacsize));
    PyDict_SetItemString(result, "channels", PyInt_FromLong(data.channels));
    PyDict_SetItemString(result, "rate", PyInt_FromLong(data.rate));
    return result;
}

int
object2codec(PyObject* object, CodecInst&data)
{
    if (!PyDict_Check(object)) {
        PyErr_SetString(WebRtcError, "must be dict with keys type:int, name:str, frequency:int, size:int, channels:int, rate:int");
        return -1;
    }
    PyObject* type_o = PyDict_GetItemString(object, "type");
    PyObject* name_o = PyDict_GetItemString(object, "name");
    PyObject* frequency_o = PyDict_GetItemString(object, "frequency");
    PyObject* size_o = PyDict_GetItemString(object, "size");
    PyObject* channels_o = PyDict_GetItemString(object, "channels");
    PyObject* rate_o = PyDict_GetItemString(object, "rate");
    if (!PyInt_Check(type_o) || !PyString_Check(name_o) || !PyInt_Check(frequency_o) || !PyInt_Check(size_o) || !PyInt_Check(channels_o) || !PyInt_Check(rate_o)) {
        PyErr_SetString(WebRtcError, "must be dict with keys type:int, name:str, frequency:int, size:int, channels:int, rate:int");
        return -1;
        
    }
    
    data.pltype = PyInt_AsLong(type_o);
    const char* name = PyString_AsString(name_o);
    strncpy(data.plname, name, sizeof(data.plname));
    data.plfreq = PyInt_AsLong(frequency_o);
    data.pacsize = PyInt_AsLong(size_o);
    data.channels = PyInt_AsLong(channels_o);
    data.rate = PyInt_AsLong(rate_o);
    
    return 0;
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, NumOfCodecs)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->codec, VoECodec)
    
    int count = self->codec->NumOfCodecs();
    My_NULL_IF_NEG(count, self->base);
    
    return PyInt_FromLong(count);
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, Codecs)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->codec, VoECodec)
    
    int count = self->codec->NumOfCodecs();
    My_NULL_IF_NEG(count, self->base);
    
    PyObject *result = PyList_New(count);
    for (unsigned i=0; i<count; ++i) {
        CodecInst codec;
        int er = self->codec->GetCodec(i, codec);
        My_NULL_IF_NEG(er, self->base);
        PyObject *codec_o = codec2object(codec);
        PyList_SetItem(result, i, codec_o);
    }
    return Py_BuildValue("N", result);
}


My_PROPERTY_GETTER(WebRtcChannel, SendCodec)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->codec, VoECodec)
    My_NULL_IF_NEG2(self->ch)
    
    CodecInst codec;
    int er = self->voe->codec->GetSendCodec(self->ch, codec);
    My_NULL_IF_NEG(er, self->voe->base);
    
    PyObject* result = codec2object(codec);
    return result;
}

My_PROPERTY_SETTER(WebRtcChannel, SendCodec)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->codec, VoECodec)
    My_ERROR_IF_NEG2(self->ch)
    
    CodecInst codec;
    int er = object2codec(value, codec);
    if (er < 0) {
        return NULL;
    }
    
    er = self->voe->codec->SetSendCodec(self->ch, codec);
    My_ERROR_IF_NEG(er, self->voe->base)
    
    return 0;
}

My_PROPERTY_GETTER(WebRtcChannel, RecCodec)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->codec, VoECodec)
    My_NULL_IF_NEG2(self->ch)
    
    CodecInst codec;
    int er = self->voe->codec->GetRecCodec(self->ch, codec);
    if (er < 0) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    //My_NULL_IF_NEG(er, self->voe->base);
    
    PyObject* result = codec2object(codec);
    return result;
}

My_PROPERTY_SETTER(WebRtcChannel, ISACInitTargetRate)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->codec, VoECodec)
    My_ERROR_IF_NEG2(self->ch)
    My_INT(value, valueint)
    
    int er = self->voe->codec->SetISACInitTargetRate(self->ch, valueint);
    My_ERROR_IF_NEG(er, self->voe->base)
    
    return 0;
}

My_PROPERTY_SETTER(WebRtcChannel, ISACMaxRate)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->codec, VoECodec)
    My_ERROR_IF_NEG2(self->ch)
    My_INT(value, valueint)
    
    int er = self->voe->codec->SetISACMaxRate(self->ch, valueint);
    My_ERROR_IF_NEG(er, self->voe->base)
    
    return 0;
}

My_PROPERTY_SETTER(WebRtcChannel, ISACMaxPayloadSize)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->codec, VoECodec)
    My_ERROR_IF_NEG2(self->ch)
    My_INT(value, valueint)
    
    int er = self->voe->codec->SetISACMaxPayloadSize(self->ch, valueint);
    My_ERROR_IF_NEG(er, self->voe->base)
    
    return 0;
}

My_PROPERTY_GETTER(WebRtcChannel, RecPayloadType)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->codec, VoECodec)
    My_NULL_IF_NEG2(self->ch)
    
    CodecInst codec;
    int er = self->voe->codec->GetRecPayloadType(self->ch, codec);
    My_NULL_IF_NEG(er, self->voe->base);
    
    PyObject* result = codec2object(codec);
    return result;
}

My_PROPERTY_SETTER(WebRtcChannel, RecPayloadType)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->codec, VoECodec)
    My_ERROR_IF_NEG2(self->ch)
    
    CodecInst codec;
    int er = object2codec(value, codec);
    if (er < 0) {
        return NULL;
    }
    
    er = self->voe->codec->SetRecPayloadType(self->ch, codec);
    My_ERROR_IF_NEG(er, self->voe->base)
    
    return 0;
}

My_PROPERTY_SETTER(WebRtcChannel, SendCNPayloadType)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->codec, VoECodec)
    My_ERROR_IF_NEG2(self->ch)
    
    if (!PyTuple_Check(value) || PyTuple_Size(value) != 2) {
        PyErr_SetString(WebRtcError, "must be a tuple (type:int, frequency:int)");
        return -1;
    }
    
    PyObject *type_o = PyTuple_GetItem(value, 0);
    PyObject *frequency_o = PyTuple_GetItem(value, 1);
    if (!PyInt_Check(type_o) || !PyInt_Check(frequency_o)) {
        PyErr_SetString(WebRtcError, "must be a tuple (type:int, frequency:int)");
        return -1;
    }
    
    int type = PyInt_AsLong(type_o);
    int frequency = PyInt_AsLong(frequency_o);
    if (frequency == 0) {
        frequency = kFreq16000Hz;
    }
    
    int er = self->voe->codec->SetSendCNPayloadType(self->ch, type, (PayloadFrequencies) frequency);
    My_ERROR_IF_NEG(er, self->voe->base)
    return 0;
}


My_PROPERTY_GETTER(WebRtcChannel, VADStatus)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->codec, VoECodec)
    My_NULL_IF_NEG2(self->ch)
    
    bool enabled;
    VadModes mode;
    bool disable_dtx;
    int er = self->voe->codec->GetVADStatus(self->ch, enabled, mode, disable_dtx);
    My_NULL_IF_NEG(er, self->voe->base);
    
    if (enabled) {
        const char* valuestr = My_ATTR2STRING(mode, VadModes);
        if (valuestr != NULL) {
            PyObject *result = PyTuple_New(2);
            PyTuple_SetItem(result, 0, PyString_FromString(valuestr));
            PyTuple_SetItem(result, 1, PyBool_FromLong(disable_dtx));
            return Py_BuildValue("N", result);
        }
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_SETTER(WebRtcChannel, VADStatus)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->codec, VoECodec)
    My_ERROR_IF_NEG2(self->ch)
    
    int er;
    if (value == Py_None || value == Py_False) {
        er = self->voe->codec->SetVADStatus(self->ch, false);
    }
    else if (value == Py_True) {
        er = self->voe->codec->SetVADStatus(self->ch, true);
    }
    else if (PyTuple_Check(value) && PyTuple_Size(value) == 2) {
        PyObject *mode_o = PyTuple_GetItem(value, 0);
        PyObject *disable_dtx_o = PyTuple_GetItem(value, 1);
        if (!PyString_Check(mode_o) || !PyBool_Check(disable_dtx_o)) {
            PyErr_SetString(WebRtcError, "must be None or tuple (mode:str, disable_dtx:bool)");
            return -1;
        }
        
        const char* modestr = PyString_AsString(mode_o);
        int mode = My_STRING2ATTR(modestr, VadModes);
        My_ERROR_IF_NEG3(mode, modestr)
        bool disable_dtx = (disable_dtx_o == Py_True);
        er = self->voe->codec->SetVADStatus(self->ch, true, (VadModes) mode, disable_dtx);
    }
    else {
        PyErr_SetString(WebRtcError, "must be None or tuple (mode:str, disable_dtx:bool)");
        return -1;
    }

    My_ERROR_IF_NEG(er, self->voe->base)
    return 0;
}

My_PROPERTY_SETTER(WebRtcChannel, AMREncFormat)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->codec, VoECodec)
    My_ERROR_IF_NEG2(self->ch)
    My_STRING(value, valuestr)
    
    int valueint = My_STRING2ATTR(valuestr, AmrMode);
    My_ERROR_IF_NEG3(valueint, valuestr);
    
    int er = self->voe->codec->SetAMREncFormat(self->ch, (AmrMode) valueint);
    My_ERROR_IF_NEG(er, self->voe->base)
    
    return 0;
}

My_PROPERTY_SETTER(WebRtcChannel, AMRDecFormat)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->codec, VoECodec)
    My_ERROR_IF_NEG2(self->ch)
    My_STRING(value, valuestr)
    
    int valueint = My_STRING2ATTR(valuestr, AmrMode);
    My_ERROR_IF_NEG3(valueint, valuestr);
    
    int er = self->voe->codec->SetAMRDecFormat(self->ch, (AmrMode) valueint);
    My_ERROR_IF_NEG(er, self->voe->base)
    
    return 0;
}

My_PROPERTY_SETTER(WebRtcChannel, AMRWbEncFormat)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->codec, VoECodec)
    My_ERROR_IF_NEG2(self->ch)
    My_STRING(value, valuestr)
    
    int valueint = My_STRING2ATTR(valuestr, AmrMode);
    My_ERROR_IF_NEG3(valueint, valuestr);
    
    int er = self->voe->codec->SetAMRWbEncFormat(self->ch, (AmrMode) valueint);
    My_ERROR_IF_NEG(er, self->voe->base)
    
    return 0;
}

My_PROPERTY_SETTER(WebRtcChannel, AMRWbDecFormat)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->codec, VoECodec)
    My_ERROR_IF_NEG2(self->ch)
    My_STRING(value, valuestr)
    
    int valueint = My_STRING2ATTR(valuestr, AmrMode);
    My_ERROR_IF_NEG3(valueint, valuestr);
    
    int er = self->voe->codec->SetAMRWbDecFormat(self->ch, (AmrMode) valueint);
    My_ERROR_IF_NEG(er, self->voe->base)
    
    return 0;
}

