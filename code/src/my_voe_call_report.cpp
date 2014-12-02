#include <Python.h>

#include "common_types.h"

#include "my_macro.h"
#include "my_error.h"
#include "my_common_types.h"
#include "my_voice_engine.h"
#include "my_channel.h"
#include "my_voe_call_report.h"

using namespace webrtc;

My_METHOD_ARGS(WebRtcVoiceEngine, WriteReportToFile)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->call_report, VoECallReport)
    
    const char* filename = NULL;
    if (!PyArg_ParseTuple(args, "s", &filename)) {
        return NULL;
    }
    
    int er = self->call_report->WriteReportToFile(filename);
    My_NULL_IF_NEG(er, self->base)
    
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject* stat2object(const StatVal& stat)
{
    PyObject* result = PyTuple_New(3);
    PyTuple_SetItem(result, 0, PyInt_FromLong(stat.min));
    PyTuple_SetItem(result, 1, PyInt_FromLong(stat.max));
    PyTuple_SetItem(result, 2, PyInt_FromLong(stat.average));
    return result;
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, SpeechAndNoiseSummary)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->call_report, VoECallReport)
    
    LevelStatistics status;
    int er = self->call_report->GetSpeechAndNoiseSummary(status);
    My_NULL_IF_NEG(er, self->base);
    
    PyObject* result = PyDict_New();
    PyDict_SetItemString(result, "speech_rx", stat2object(status.speech_rx));
    PyDict_SetItemString(result, "speech_tx", stat2object(status.speech_tx));
    PyDict_SetItemString(result, "noise_rx", stat2object(status.noise_rx));
    PyDict_SetItemString(result, "noise_tx", stat2object(status.noise_tx));
    
    return Py_BuildValue("O", result);
}

My_PROPERTY_GETTER(WebRtcVoiceEngine, EchoMetricSummary)
{
    My_NULL_IF_NULL(self->base, VoEBase)
    My_NULL_IF_NULL(self->call_report, VoECallReport)
    
    EchoStatistics status;
    int er = self->call_report->GetEchoMetricSummary(status);
    My_NULL_IF_NEG(er, self->base);
    
    
    PyObject* result = PyDict_New();
    PyDict_SetItemString(result, "erl", stat2object(status.erl));
    PyDict_SetItemString(result, "erle", stat2object(status.erle));
    PyDict_SetItemString(result, "rerl", stat2object(status.rerl));
    PyDict_SetItemString(result, "a_nlp", stat2object(status.a_nlp));
    
    return Py_BuildValue("O", result);
}

My_METHOD(WebRtcChannel, ResetCallReportStatistics)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->call_report, VoECallReport)
    My_NULL_IF_NEG2(self->ch)
    
    int er = self->voe->call_report->ResetCallReportStatistics(self->ch);
    My_NULL_IF_NEG(er, self->voe->base)
    
    Py_INCREF(Py_None);
    return Py_None;
}

My_PROPERTY_GETTER(WebRtcChannel, RoundTripTimeSummary)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->call_report, VoECallReport)
    My_NULL_IF_NEG2(self->ch)

    StatVal status;
    int er = self->voe->call_report->GetRoundTripTimeSummary(self->ch, status);
    My_NULL_IF_NEG(er, self->voe->base)
    
    return Py_BuildValue("O", stat2object(status));
}

My_PROPERTY_GETTER(WebRtcChannel, DeadOrAliveSummary)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->call_report, VoECallReport)
    My_NULL_IF_NEG2(self->ch)
    
    int numOfDeadDetections, numOfAliveDetections;
    int er = self->voe->call_report->GetDeadOrAliveSummary(self->ch, numOfDeadDetections, numOfAliveDetections);
    My_NULL_IF_NEG(er, self->voe->base)
    
    PyObject* result = PyTuple_New(2);
    PyTuple_SetItem(result, 0, PyInt_FromLong(numOfDeadDetections));
    PyTuple_SetItem(result, 0, PyInt_FromLong(numOfAliveDetections));
    
    return Py_BuildValue("O", result);
}


