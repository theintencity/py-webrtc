#include <Python.h>
#include <structmember.h>

#include "common_types.h"
#include "voe_base.h"
#include "voe_audio_processing.h"
#include "voe_codec.h"

#include "my_macro.h"
#include "my_error.h"
#include "my_common_types.h"
#include "my_voice_engine.h"

#include "my_voe_base.h"
#include "my_voe_ap.h"
#include "my_voe_codec.h"
#include "my_voe_call_report.h"
#include "my_voe_dtmf.h"
#include "my_voe_encryption.h"
#include "my_voe_external_media.h"

using namespace webrtc;


#define MODULE_RELEASE(name)  if (self->name != NULL) { self->name->Release(); self->name = NULL; }
#define MODULE_GET(name,type) type *name = type::GetInterface(voe);
#define MODULE_ASSIGN(name)   self->name = name;


static void
WebRtcVoiceEngine_dealloc(WebRtcVoiceEngine* self)
{
    if (self->base != NULL) {
        self->base->Terminate();
    }
    
    MODULE_RELEASE(base)
    MODULE_RELEASE(ap)
    MODULE_RELEASE(codec)
    MODULE_RELEASE(call_report)
    MODULE_RELEASE(dtmf)
    MODULE_RELEASE(encryption)
    MODULE_RELEASE(external_media)

    if (self->voe != NULL) {
        VoiceEngine::Delete(self->voe);
        self->voe = NULL;
    }
    
    self->ob_type->tp_free((PyObject*) self);
}

static int
WebRtcVoiceEngine_init(WebRtcVoiceEngine *self, PyObject *args, PyObject *kwargs)
{
    VoiceEngine *voe = NULL;
    
    voe = VoiceEngine::Create();
    if (voe == NULL) {
        return NULL;
    }
    
    MODULE_GET(base, VoEBase)
    if (base == NULL) {
        VoiceEngine::Delete(voe);
        return NULL;
    }
    
    MODULE_GET(ap, VoEAudioProcessing)
    MODULE_GET(codec, VoECodec)
    MODULE_GET(call_report, VoECallReport)
    MODULE_GET(dtmf, VoEDtmf)
    MODULE_GET(encryption, VoEEncryption)
    MODULE_GET(external_media, VoEExternalMedia)
    
    if (base->Init() < 0) {
        PyErr_Format(WebRtcError, "%d %s", base->LastError(), Error2String(base->LastError()));
        base->Release();
        if (ap != NULL)
            ap->Release();
        VoiceEngine::Delete(voe);
        return NULL;
    }
    
    self->voe = voe;
    MODULE_ASSIGN(base)
    MODULE_ASSIGN(ap)
    MODULE_ASSIGN(codec)
    MODULE_ASSIGN(call_report)
    MODULE_ASSIGN(dtmf)
    MODULE_ASSIGN(encryption)
    MODULE_ASSIGN(external_media)
    
    return 0;
}


static PyMethodDef WebRtcVoiceEngine_methods[] = {
    // voe_base.h
    My_METHOD_DECL(WebRtcVoiceEngine, RegisterVoiceEngineObserver, METH_VARARGS | METH_KEYWORDS, "set_observer",
                PyDoc_STR("set_observer(callback)\n\n"
                "Sets the callback(ch:int, err:str, data:object) function to enable runtime error control and warning notifications")),
    
    // voe_call_report.h
    My_METHOD_DECL(WebRtcVoiceEngine, WriteReportToFile, METH_VARARGS, "write_report_to_file",
                PyDoc_STR("write_report_to_file(filename)\n\n"
                "Creates a text file in ASCII format, which contains a summary of all the statistics that can be obtained by the call report sub-API")),
    
    // voe_dtmf.h
    My_METHOD_DECL(WebRtcVoiceEngine, PlayDtmfTone, METH_VARARGS | METH_KEYWORDS, "play_dtmf_tone",
                PyDoc_STR("play_dtmf_tone(event_code, length_ms=200, attenuation_db=10)\n\n"
                "Plays a DTMF feedback tone for single digit event_code locally, e.g., event_code='0' to play tone for digit 0.")),
    My_METHOD_DECL(WebRtcVoiceEngine, StartPlayingDtmfTone, METH_VARARGS | METH_KEYWORDS, "start_playing_dtmf_tone",
                PyDoc_STR("start_playing_dtmf_tone(event_code, attenuation_db=10)\n\n"
                "Starts playing out a DTMF feedback tone for single digit event_code locally. The tone will be played out until the corresponding stop function is called")),
    My_METHOD_DECL(WebRtcVoiceEngine, StopPlayingDtmfTone, METH_NOARGS, "stop_playing_dtmf_tone",
                PyDoc_STR("stop_playing_dtmf_tone()\n\n"
                "Stops playing out a DTMF feedback tone locally")),
    
    // voe_external_media.h
    My_METHOD_DECL(WebRtcVoiceEngine, ExternalRecordingInsertData, METH_VARARGS | METH_KEYWORDS, "external_recording_insert_data",
                PyDoc_STR("external_recording_insert_data(speech_data_10ms, sampling_frequency, current_delay_ms)\n\n"
                "This function accepts externally recorded audio. During transmission, this method should be called at as regular an interval as possible with frames of corresponding size")),
    My_METHOD_DECL(WebRtcVoiceEngine, ExternalPlayoutGetData, METH_VARARGS | METH_KEYWORDS, "external_playout_get_data",
                PyDoc_STR("external_playout_get_data(sampling_freqency, current_delay_ms) -> speech_data_10ms"
                "This function gets audio for an external playout sink. During transmission, this function should be called every ~10 ms to obtain a new 10 ms frame of audio. The length of the block will be 160, 320, 440 or 480 samples (for 16, 32, 44 or 48 kHz sampling rates respectively)")),
    
    
    {NULL,	NULL},
};


static PyGetSetDef WebRtcVoiceEngine_getseters[] = {
    // voe_base.h
    My_PROPERTY_READONLY(WebRtcVoiceEngine, Version, "version",
                PyDoc_STR("(read-only) The version information for VoiceEngine and its components")),
    My_PROPERTY_WRITEONLY(WebRtcVoiceEngine, TraceFile, "trace_filename",
                PyDoc_STR("(write-only) The name of the trace file and enables non-encrypted trace messages")),
    My_PROPERTY_READONLY(WebRtcVoiceEngine, LastError, "last_error",
                PyDoc_STR("(read-only) The last VoiceEngine error code as a string")),
    My_PROPERTY_READONLY(WebRtcVoiceEngine, MaxNumOfChannels, "max_channels",
                PyDoc_STR("(read-only) The maximum number of channels that can be created")),
        
    // voe_audio_processing.h
    My_PROPERTY_READWRITE(WebRtcVoiceEngine, NsStatus, "ns_status",
                PyDoc_STR("The noise suppression mode to reduce the noise in the microphone signal")),
    My_PROPERTY_READWRITE(WebRtcVoiceEngine, AgcStatus, "agc_status",
                PyDoc_STR("The automatic gain control mode to adjust the microphone signal to an appropriate level")),
    My_PROPERTY_READWRITE(WebRtcVoiceEngine, AgcConfig, "agc_config",
                PyDoc_STR("The automatic gain control configuration should be used where the working environment is well known")),
    My_PROPERTY_READWRITE(WebRtcVoiceEngine, EcStatus, "ec_status",
                PyDoc_STR("The echo control mode mitigates acoustic echo where a user can hear their own speech repeated back due to an acoustic coupling between the speaker and the microphone at the remote end")),
    My_PROPERTY_READWRITE(WebRtcVoiceEngine, AecmMode, "aecm_mode",
                PyDoc_STR("The settings for the echo cancellation designed for mobile devices")),
    My_PROPERTY_READWRITE(WebRtcVoiceEngine, MetricsStatus, "metrics_status",
                PyDoc_STR("The ability to retrieve instantaneous speech, noise and echo metrics during an active call")),
    My_PROPERTY_READONLY(WebRtcVoiceEngine, SpeechMetrics, "speech_metrics",
                PyDoc_STR("(read-only) The instantaneous speech level metrics for the transmitted and received signals")),
    My_PROPERTY_READONLY(WebRtcVoiceEngine, NoiseMetrics, "noise_metrics",
                PyDoc_STR("(read-only) The instantaneous noise level metrics for the transmitted and received signals")),
    My_PROPERTY_READONLY(WebRtcVoiceEngine, EchoMetrics, "echo_metrics",
                PyDoc_STR("(read-only) The instantaneous echo level metrics for the near-end and far-end signals")),
    My_PROPERTY_WRITEONLY(WebRtcVoiceEngine, DebugRecording, "debug_recording_filename",
                PyDoc_STR("(write-only) The file name for recording of Audio Processing (AP) debugging information that can later be used for off-line analysis of the AP performance")),
    My_PROPERTY_READWRITE(WebRtcVoiceEngine, TypingDetectionStatus, "typing_detection_status",
                PyDoc_STR("The detection of disturbing keyboard typing causes an error notification as a callback upon detection")),
    
    // voe_codec.h
    My_PROPERTY_READONLY(WebRtcVoiceEngine, NumOfCodecs, "num_codecs",
                PyDoc_STR("(read-only) The number of supported codecs")),
    My_PROPERTY_READONLY(WebRtcVoiceEngine, Codecs, "codecs",
                PyDoc_STR("(read-only) The list of supported codecs information")),
    
    // voe_call_report.h
    My_PROPERTY_READONLY(WebRtcVoiceEngine, SpeechAndNoiseSummary, "speech_and_noise_summary",
                PyDoc_STR("(read-only) The minimum, maximum and average levels for long-term speech and noise metrics")),
    My_PROPERTY_READONLY(WebRtcVoiceEngine, EchoMetricSummary, "echo_metrics_summary",
                PyDoc_STR("(read-only) The minimum, maximum and average levels for long-term echo metrics")),
    
    // voe_dtmf.h
    My_PROPERTY_READWRITE(WebRtcVoiceEngine, DtmfFeedbackStatus, "dtmf_feedback_status",
                PyDoc_STR("The DTMF feedback state: when a DTMF tone is sent, the same tone is played out on the speaker")),
    
    // voe_external_media.h
    My_PROPERTY_WRITEONLY(WebRtcVoiceEngine, ExternalRecordingStatus, "external_recording_status",
                PyDoc_STR("(write-only) The state of external recording")),
    My_PROPERTY_WRITEONLY(WebRtcVoiceEngine, ExternalPlayoutStatus, "external_playout_status",
                PyDoc_STR("(write-only) The state of external playout")),

    
//    My_PROPERTY_READWRITE(WebRtcVoiceEngine, , "",
//                PyDoc_STR("")),
    
    {NULL}  /* Sentinel */
};

static PyTypeObject WebRtcVoiceEngineType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "webrtc.VoiceEngine",      /*tp_name*/
    sizeof(WebRtcVoiceEngine),            /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)WebRtcVoiceEngine_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT, /*tp_flags*/
    "VoiceEngine object",      /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    WebRtcVoiceEngine_methods, /* tp_methods */
    0,                         /* tp_members */
    WebRtcVoiceEngine_getseters,/* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc) WebRtcVoiceEngine_init,    /* tp_init */
    0,                         /* tp_alloc */
    0,                         /* tp_new */
};

int my_voice_engine_preinit() {
    WebRtcVoiceEngineType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&WebRtcVoiceEngineType) < 0)
        return -1;
    return 0;
}

void my_voice_engine_addobjects(PyObject* m) {
    Py_INCREF(&WebRtcVoiceEngineType);
    PyModule_AddObject(m, "VoiceEngine", (PyObject *)&WebRtcVoiceEngineType);
}
