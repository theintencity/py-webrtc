#include <Python.h>

#include "common_types.h"

#include "my_macro.h"
#include "my_error.h"
#include "my_common_types.h"
#include "my_channel.h"
#include "my_voe_ap.h"

using namespace webrtc;


class RxVadCallback: VoERxVadCallback {
public:
    RxVadCallback(PyObject* callback, PyObject* userdata);
    ~RxVadCallback();
    void OnRxVad(int channel, int vadDecision);
private:
    PyObject *callback;
    PyObject *userdata;
};


RxVadCallback::RxVadCallback(PyObject* callback, PyObject* userdata) {
    Py_XINCREF(callback);
    this->callback = callback;
    Py_XINCREF(userdata);
    this->userdata = userdata;
}

RxVadCallback::~RxVadCallback() {
    Py_XDECREF(this->callback);
    Py_XDECREF(this->userdata);
}

void RxVadCallback::OnRxVad(int channel, int vadDecision) {
    if (this->callback != NULL) {
        PyObject* arglist = Py_BuildValue("(Oii)", this->userdata, channel, vadDecision);
        PyObject* output = PyObject_CallObject(this->callback, arglist);
        Py_DECREF(arglist);
    }
}


My_PROPERTY_SETTER(WebRtcVoiceEngine, DebugRecording)
{
    My_ERROR_IF_NULL(self->base, VoEBase)
    My_ERROR_IF_NULL(self->ap, VoEAudioProcessing)
    My_STRING_OR_NONE(value, valuestr)

    int er = valuestr != NULL ? self->ap->StartDebugRecording(valuestr) : self->ap->StopDebugRecording();
    My_ERROR_IF_NEG(er, self->base)

    return 0;
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, TypingDetectionStatus)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->ap, VoEAudioProcessing)
    
    bool status;
    int er = self->ap->GetTypingDetectionStatus(status);
    My_NULL_IF_NEG(er, self->base);
    
    return PyBool_FromLong(status);
}

My_PROPERTY_SETTER(WebRtcVoiceEngine, TypingDetectionStatus)
{
    My_ERROR_IF_NULL(self->base, VoEBase)
    My_ERROR_IF_NULL(self->ap, VoEAudioProcessing)
    
    if (!PyBool_Check(value)) {
        PyErr_SetString(WebRtcError, "must be a boolean");
        return -1;
    }
    
    bool status = (value == Py_True);
    
    int er = self->ap->SetTypingDetectionStatus(status);
    My_ERROR_IF_NEG(er, self->base)
    
    return 0;
}


My_PROPERTY_GETTER(WebRtcVoiceEngine, NsStatus)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->ap, VoEAudioProcessing)
    
    bool enabled;
    NsModes status;
    int er = self->ap->GetNsStatus(enabled, status);
    My_NULL_IF_NEG(er, self->base);
    
    if (enabled) {
        const char *valuestr = My_ATTR2STRING(status, NsModes);
        if (valuestr != NULL) {
            return Py_BuildValue("s", valuestr);
        }
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_SETTER(WebRtcVoiceEngine, NsStatus)
{
    My_ERROR_IF_NULL(self->base, VoEBase)
    My_ERROR_IF_NULL(self->ap, VoEAudioProcessing)
    My_STRING_OR_NONE(value, valuestr)
    
    int er;
    if (valuestr == NULL) {
        er = self->ap->SetNsStatus(false);
    }
    else {
        int status = My_STRING2ATTR(valuestr, NsModes);
        My_ERROR_IF_NEG3(status, valuestr)
        er = self->ap->SetNsStatus(true, (NsModes) status);
    }
    My_ERROR_IF_NEG(er, self->base)
    return 0;
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, AgcStatus)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->ap, VoEAudioProcessing)
    
    bool enabled;
    AgcModes status;
    int er = self->ap->GetAgcStatus(enabled, status);
    My_NULL_IF_NEG(er, self->base);
    
    if (enabled) {
        const char *valuestr = My_ATTR2STRING(status, AgcModes);
        if (valuestr != NULL) {
            return Py_BuildValue("s", valuestr);
        }
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_SETTER(WebRtcVoiceEngine, AgcStatus)
{
    My_ERROR_IF_NULL(self->base, VoEBase)
    My_ERROR_IF_NULL(self->ap, VoEAudioProcessing)
    My_STRING_OR_NONE(value, valuestr)
    
    int er;
    if (valuestr == NULL) {
        er = self->ap->SetAgcStatus(false);
    }
    else {
        int status = My_STRING2ATTR(valuestr, AgcModes);
        My_ERROR_IF_NEG3(status, valuestr)
        er = self->ap->SetAgcStatus(true, (AgcModes) status);
    }
    My_ERROR_IF_NEG(er, self->base)
    return 0;
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, AgcConfig)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->ap, VoEAudioProcessing)
    
    AgcConfig status;
    int er = self->ap->GetAgcConfig(status);
    My_NULL_IF_NEG(er, self->base);
    
    PyObject *result = PyTuple_New(3);
    PyTuple_SetItem(result, 0, PyInt_FromLong(status.targetLeveldBOv));
    PyTuple_SetItem(result, 1, PyInt_FromLong(status.digitalCompressionGaindB));
    PyTuple_SetItem(result, 2, PyBool_FromLong(status.limiterEnable));
    return Py_BuildValue("N", result);
}

My_PROPERTY_SETTER(WebRtcVoiceEngine, AgcConfig)
{
    My_ERROR_IF_NULL(self->base, VoEBase)
    My_ERROR_IF_NULL(self->ap, VoEAudioProcessing)
    
    if (!PyTuple_Check(value) || PyTuple_Size(value) != 3) {
        PyErr_SetString(WebRtcError, "must be a tuple (targetLeveldBOv:int, digitalCompressionGaindB:int, limiterEnable:bool)");
        return -1;
    }
    
    PyObject *targetLeveldBOv_o = PyTuple_GetItem(value, 0);
    PyObject *digitalCompressionGaindB_o = PyTuple_GetItem(value, 1);
    PyObject *limiterEnable_o = PyTuple_GetItem(value, 2);
    if (!PyInt_Check(targetLeveldBOv_o) || !PyInt_Check(digitalCompressionGaindB_o) || !PyBool_Check(limiterEnable_o)) {
        PyErr_SetString(WebRtcError, "must be a tuple (targetLeveldBOv:int, digitalCompressionGaindB:int, limiterEnable:bool)");
        return -1;
    }
    
    AgcConfig status;
    status.targetLeveldBOv = PyInt_AsLong(targetLeveldBOv_o);
    status.digitalCompressionGaindB = PyInt_AsLong(digitalCompressionGaindB_o);
    status.limiterEnable = PyInt_AsLong(limiterEnable_o);
    
    int er = self->ap->SetAgcConfig(status);
    My_ERROR_IF_NEG(er, self->base)
    return 0;
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, EcStatus)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->ap, VoEAudioProcessing)
    
    bool enabled;
    EcModes status;
    int er = self->ap->GetEcStatus(enabled, status);
    My_NULL_IF_NEG(er, self->base);
    
    if (enabled) {
        const char *valuestr = My_ATTR2STRING(status, EcModes);
        if (valuestr != NULL) {
            return Py_BuildValue("s", valuestr);
        }
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_SETTER(WebRtcVoiceEngine, EcStatus)
{
    My_ERROR_IF_NULL(self->base, VoEBase)
    My_ERROR_IF_NULL(self->ap, VoEAudioProcessing)
    My_STRING_OR_NONE(value, valuestr)
    
    int er;
    if (valuestr == NULL) {
        er = self->ap->SetEcStatus(false);
    }
    else {
        int status = My_STRING2ATTR(valuestr, EcModes);
        My_ERROR_IF_NEG3(status, valuestr)
        er = self->ap->SetEcStatus(true, (EcModes) status);
    }
    My_ERROR_IF_NEG(er, self->base)
    return 0;
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, AecmMode)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->ap, VoEAudioProcessing)
    
    bool enabledCNG;
    AecmModes status;
    int er = self->ap->GetAecmMode(status, enabledCNG);
    My_NULL_IF_NEG(er, self->base);
    
    const char *valuestr = My_ATTR2STRING(status, AecmModes);
    if (valuestr != NULL) {
        return Py_BuildValue("s", valuestr);
    }
    // TODO: add CNG to result
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_SETTER(WebRtcVoiceEngine, AecmMode)
{
    My_ERROR_IF_NULL(self->base, VoEBase)
    My_ERROR_IF_NULL(self->ap, VoEAudioProcessing)
    My_STRING(value, valuestr)
    
    int status = My_STRING2ATTR(valuestr, AecmModes);
    My_ERROR_IF_NEG3(status, valuestr)
    int er = self->ap->SetAecmMode((AecmModes) status);
    My_ERROR_IF_NEG(er, self->base)
    // TODO: add CNG
    
    return 0;
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, MetricsStatus)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->ap, VoEAudioProcessing)
    
    bool status;
    int er = self->ap->GetMetricsStatus(status);
    My_NULL_IF_NEG(er, self->base);
    
    return PyBool_FromLong(status);
}

My_PROPERTY_SETTER(WebRtcVoiceEngine, MetricsStatus)
{
    My_ERROR_IF_NULL(self->base, VoEBase)
    My_ERROR_IF_NULL(self->ap, VoEAudioProcessing)
    
    if (!PyBool_Check(value)) {
        PyErr_SetString(WebRtcError, "must be bool");
        return -1;
    }
    
    bool status = (value == Py_True);
    int er = self->ap->SetMetricsStatus(status);
    My_ERROR_IF_NEG(er, self->base)
    
    return 0;
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, SpeechMetrics)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->ap, VoEAudioProcessing)
    
    int levelTx, levelRx;
    int er = self->ap->GetSpeechMetrics(levelTx, levelRx);
    My_NULL_IF_NEG(er, self->base);
    
    PyObject *result = PyTuple_New(2);
    PyTuple_SetItem(result, 0, PyInt_FromLong(levelTx));
    PyTuple_SetItem(result, 1, PyInt_FromLong(levelRx));
    
    return Py_BuildValue("N", result);
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, NoiseMetrics)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->ap, VoEAudioProcessing)
    
    int levelTx, levelRx;
    int er = self->ap->GetNoiseMetrics(levelTx, levelRx);
    My_NULL_IF_NEG(er, self->base);
    
    PyObject *result = PyTuple_New(2);
    PyTuple_SetItem(result, 0, PyInt_FromLong(levelTx));
    PyTuple_SetItem(result, 1, PyInt_FromLong(levelRx));
    
    return Py_BuildValue("N", result);
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, EchoMetrics)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->ap, VoEAudioProcessing)
    
    int ERL, ERLE, RERL, A_NLP;
    int er = self->ap->GetEchoMetrics(ERL, ERLE, RERL, A_NLP);
    My_NULL_IF_NEG(er, self->base);
    
    PyObject *result = PyTuple_New(4);
    PyTuple_SetItem(result, 0, PyInt_FromLong(ERL));
    PyTuple_SetItem(result, 1, PyInt_FromLong(ERLE));
    PyTuple_SetItem(result, 2, PyInt_FromLong(RERL));
    PyTuple_SetItem(result, 3, PyInt_FromLong(A_NLP));
    
    return Py_BuildValue("N", result);
}


My_PROPERTY_GETTER(WebRtcChannel, RxNsStatus)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->ap, VoEAudioProcessing)
    My_NULL_IF_NEG2(self->ch)
    
    bool enabled;
    NsModes status;
    int er = self->voe->ap->GetRxNsStatus(self->ch, enabled, status);
    My_NULL_IF_NEG(er, self->voe->base);
    
    if (enabled) {
        const char *valuestr = My_ATTR2STRING(status, NsModes);
        if (valuestr != NULL) {
            return Py_BuildValue("s", valuestr);
        }
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_SETTER(WebRtcChannel, RxNsStatus)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->ap, VoEAudioProcessing)
    My_ERROR_IF_NEG2(self->ch)
    My_STRING_OR_NONE(value, valuestr)
    
    int er;
    if (valuestr == NULL) {
        er = self->voe->ap->SetRxNsStatus(self->ch, false);
    }
    else {
        int status = My_STRING2ATTR(valuestr, NsModes);
        My_ERROR_IF_NEG3(status, valuestr)
        er = self->voe->ap->SetRxNsStatus(self->ch, true, (NsModes) status);
    }
    My_ERROR_IF_NEG(er, self->voe->base)
    return 0;
}

My_PROPERTY_GETTER(WebRtcChannel, RxAgcStatus)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->ap, VoEAudioProcessing)
    My_NULL_IF_NEG2(self->ch)
    
    bool enabled;
    AgcModes status;
    int er = self->voe->ap->GetRxAgcStatus(self->ch, enabled, status);
    My_NULL_IF_NEG(er, self->voe->base);
    
    if (enabled) {
        const char *valuestr = My_ATTR2STRING(status, AgcModes);
        if (valuestr != NULL) {
            return Py_BuildValue("s", valuestr);
        }
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_SETTER(WebRtcChannel, RxAgcStatus)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->ap, VoEAudioProcessing)
    My_ERROR_IF_NEG2(self->ch)
    My_STRING_OR_NONE(value, valuestr)
    
    int er;
    if (valuestr == NULL) {
        er = self->voe->ap->SetRxAgcStatus(self->ch, false);
    }
    else {
        int status = My_STRING2ATTR(valuestr, AgcModes);
        My_ERROR_IF_NEG3(status, valuestr)
        er = self->voe->ap->SetRxAgcStatus(self->ch, true, (AgcModes) status);
    }
    My_ERROR_IF_NEG(er, self->voe->base)
    return 0;
}

My_PROPERTY_GETTER(WebRtcChannel, RxAgcConfig)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->ap, VoEAudioProcessing)
    My_NULL_IF_NEG2(self->ch)
    
    AgcConfig status;
    int er = self->voe->ap->GetRxAgcConfig(self->ch, status);
    My_NULL_IF_NEG(er, self->voe->base);
    
    PyObject *result = PyTuple_New(3);
    PyTuple_SetItem(result, 0, PyInt_FromLong(status.targetLeveldBOv));
    PyTuple_SetItem(result, 1, PyInt_FromLong(status.digitalCompressionGaindB));
    PyTuple_SetItem(result, 2, PyBool_FromLong(status.limiterEnable));
    return Py_BuildValue("N", result);
}

My_PROPERTY_SETTER(WebRtcChannel, RxAgcConfig)
{
    My_ERROR_IF_NULL(self->voe->base, VoEBase)
    My_ERROR_IF_NULL(self->voe->ap, VoEAudioProcessing)
    My_ERROR_IF_NEG2(self->ch)
    
    if (!PyTuple_Check(value) || PyTuple_Size(value) != 3) {
        PyErr_SetString(WebRtcError, "must be a tuple (targetLeveldBOv:int, digitalCompressionGaindB:int, limiterEnable:bool)");
        return -1;
    }
    
    PyObject *targetLeveldBOv_o = PyTuple_GetItem(value, 0);
    PyObject *digitalCompressionGaindB_o = PyTuple_GetItem(value, 1);
    PyObject *limiterEnable_o = PyTuple_GetItem(value, 2);
    if (!PyInt_Check(targetLeveldBOv_o) || !PyInt_Check(digitalCompressionGaindB_o) || !PyBool_Check(limiterEnable_o)) {
        PyErr_SetString(WebRtcError, "must be a tuple (targetLeveldBOv:int, digitalCompressionGaindB:int, limiterEnable:bool)");
        return -1;
    }
    
    AgcConfig status;
    status.targetLeveldBOv = PyInt_AsLong(targetLeveldBOv_o);
    status.digitalCompressionGaindB = PyInt_AsLong(digitalCompressionGaindB_o);
    status.limiterEnable = PyInt_AsLong(limiterEnable_o);
    
    int er = self->voe->ap->SetRxAgcConfig(self->ch, status);
    My_ERROR_IF_NEG(er, self->voe->base)
    return 0;
}

My_METHOD_ARGS_KWARGS(WebRtcChannel, SetRxVadObserver)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->ap, VoEAudioProcessing)
    My_NULL_IF_NEG2(self->ch)
    
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
    
    if (self->rx_vad_callback != NULL) {
        delete self->rx_vad_callback;
        self->rx_vad_callback = NULL;
    }
    
    if (callback != Py_None) {
        self->rx_vad_callback = new RxVadCallback(callback, userdata);
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_GETTER(WebRtcChannel, VoiceActivityIndicator)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->ap, VoEAudioProcessing)
    My_NULL_IF_NEG2(self->ch)
    
    int status = self->voe->ap->VoiceActivityIndicator(self->ch);
    return PyBool_FromLong(status);
}

