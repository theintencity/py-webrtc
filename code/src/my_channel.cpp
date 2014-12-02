/*
 * Copyright (c) 2011, Kundan Singh. All Rights Reserved.
 * Copyright (c) 2014, Intencity Cloud Technologies <theintencity@gmail.com>.
 */

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
#include "my_channel.h"

#include "my_voe_base.h"
#include "my_voe_codec.h"
#include "my_voe_ap.h"
#include "my_voe_call_report.h"
#include "my_voe_dtmf.h"
#include "my_voe_encryption.h"
#include "my_voe_external_media.h"


using namespace webrtc;


static void
WebRtcChannel_dealloc(WebRtcChannel* self)
{
    if (self->ch >= 0 && self->voe != NULL) {
        self->voe->base->DeleteChannel(self->ch);
        Py_DECREF(self->voe);
    }
    
    self->ob_type->tp_free((PyObject*) self);
}

static int
WebRtcChannel_init(WebRtcChannel *self, PyObject *args, PyObject *kwds)
{
    WebRtcVoiceEngine *voe = NULL;
    if (!PyArg_ParseTuple(args, "O", &voe)) {
        return NULL;
    }
    
    int ch = voe->base->CreateChannel();
    My_ERROR_IF_NEG(ch, voe->base)
    
    Py_INCREF(voe);
    self->voe = voe;
    self->ch = ch;

    return 0;
}

My_PROPERTY_GETTER(WebRtcChannel, ID)
{
    My_NULL_IF_NEG2(self->ch)
    
    if (self->ch < 0) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    
    return Py_BuildValue("i", self->ch);
}

static PyMethodDef WebRtcChannel_methods[] = {
    
    // voe_base.h
    
    My_METHOD_DECL(WebRtcChannel, SetLocalReceiver, METH_VARARGS | METH_KEYWORDS, "set_local_receiver",
                PyDoc_STR("Sets the local receiver port and address for a specified channel number")),
    My_METHOD_DECL(WebRtcChannel, SetSendDestination, METH_VARARGS | METH_KEYWORDS, "set_send_destination",
                PyDoc_STR("Sets the destination port and address for a specified channel number")),
    My_METHOD_DECL(WebRtcChannel, StartReceive, METH_NOARGS, "start_receive",
                PyDoc_STR("Prepares and initiates the VoiceEngine for reception of incoming RTP/RTCP packets on the specified channel")),
    My_METHOD_DECL(WebRtcChannel, StopReceive, METH_NOARGS, "stop_receive",
                PyDoc_STR("Stops receiving incoming RTP/RTCP packets on the specified channel")),
    My_METHOD_DECL(WebRtcChannel, StartPlayout, METH_NOARGS, "start_playout",
                PyDoc_STR("Starts forwarding the packets to the mixer/soundcard for a specified channel")),
    My_METHOD_DECL(WebRtcChannel, StopPlayout, METH_NOARGS, "stop_playout",
                PyDoc_STR("Stops forwarding the packets to the mixer/soundcard for a specified channel")),
    My_METHOD_DECL(WebRtcChannel, StartSend, METH_NOARGS, "start_send",
                PyDoc_STR("Starts sending packets to an already specified IP address and port number for a specified channel")),
    My_METHOD_DECL(WebRtcChannel, StopSend, METH_NOARGS, "stop_send",
                PyDoc_STR("Stops sending packets from a specified channel")),
    
    // voe_audio_processing.h
    
    My_METHOD_DECL(WebRtcChannel, SetRxVadObserver, METH_VARARGS | METH_KEYWORDS, "set_rx_vad_observer",
                PyDoc_STR("The voice activity detection observer callback(userdata:object, channel:int, vadDecision:int)) to enable rx VAD notification")),
    
    // voe_call_report.h

    My_METHOD_DECL(WebRtcChannel, ResetCallReportStatistics, METH_VARARGS, "reset_call_report_statistics",
                PyDoc_STR("Performs a combined reset of all components involved in generating the call report for a specified channel")),
    
    // voe_dtmf.h

    My_METHOD_DECL(WebRtcChannel, SendTelephoneEvent, METH_VARARGS | METH_KEYWORDS, "send_telephone_event",
                PyDoc_STR("Sends telephone events either in-band or out-of-band")),
    My_METHOD_DECL(WebRtcChannel, RegisterTelephoneEventDetection, METH_VARARGS | METH_KEYWORDS, "set_telephone_event_observer",
                PyDoc_STR("Installs or uninstalls a callback and activates/deactivates detection of telephone events")),
    
    // voe_encryption.h
    
    My_METHOD_DECL(WebRtcChannel, RegisterExternalEncryption, METH_VARARGS | METH_KEYWORDS, "set_external_encryption",
                PyDoc_STR("Set encryption functions (encrypt, decrypt, encrypt_rtcp, decrypt_rtcp) to enable external encryption")),
    //My_METHOD_DECL(WebRtcChannel, EnableSRTPSend, "enable_srtp_send", METH_VARARGS | METH_KEYWORDS,
    //            PyDoc_STR("Not supported")),
    //My_METHOD_DECL(WebRtcChannel, EnableSRTPReceive, "enable_srtp_recv", METH_VARARGS | METH_KEYWORDS,
    //            PyDoc_STR("Not supported")),
    

    My_METHOD_DECL(WebRtcChannel, RegisterExternalMediaProcessing, METH_VARARGS | METH_KEYWORDS, "set_external_media_processing",
                PyDoc_STR("set_external_media_processing(type, callback, userdata)\n\n"
                "Installs an external media process and activates external media for this channel and type")),

//    My_METHOD_DECL(WebRtcChannel, , METH_VARARGS, "",
//                PyDoc_STR("")),
    {NULL,	NULL},
};

static PyGetSetDef WebRtcChannel_getseters[] = {
    
    // voe_base.h
    
    My_PROPERTY_READONLY(WebRtcChannel, ID, "id",
                PyDoc_STR("(read-only) The underlying channel id for this channel")),
    My_PROPERTY_READONLY(WebRtcChannel, LocalReceiver, "local_receiver",
                PyDoc_STR("(read-only) Get the local receiver port and address for a specified channel number")),
    My_PROPERTY_READONLY(WebRtcChannel, SendDestination, "send_destination",
                PyDoc_STR("(read-only) The destination port and address for a specified channel number")),
    My_PROPERTY_READWRITE(WebRtcChannel, OnHoldStatus, "on_hold_status",
                PyDoc_STR("The current playout and transmission status for this channel")),
    My_PROPERTY_READWRITE(WebRtcChannel, NetEQPlayoutMode, "neteq_playout_mode",
                PyDoc_STR("The NetEQ playout mode for this channel")),
    My_PROPERTY_READWRITE(WebRtcChannel, NetEQBGNMode, "neteq_background_mode",
                PyDoc_STR("The NetEQ background noise mode for this channel")),

    // voe_audio_processing.h
    
    My_PROPERTY_READWRITE(WebRtcChannel, RxNsStatus, "rx_ns_status",
                PyDoc_STR("The noise suppression mode of receiving signal intended for advanced use only")),
    My_PROPERTY_READWRITE(WebRtcChannel, RxAgcStatus, "rx_agc_status",
                PyDoc_STR("The automatic gain control mode to adjust the received signal intended for advanced use only")),
    My_PROPERTY_READWRITE(WebRtcChannel, RxAgcConfig, "rx_agc_config",
                PyDoc_STR("The automatic gain control configuration of receiving signal intended for advanced use only")),
    My_PROPERTY_READONLY(WebRtcChannel, VoiceActivityIndicator, "voice_activity",
                PyDoc_STR("(read-only) The voice activity is True if audio frames contain speech else False. Always True if VAD is disabled")),
    
    // voe_codec.h
    
    My_PROPERTY_READWRITE(WebRtcChannel, SendCodec, "send_codec",
                PyDoc_STR("The codec parameters for the sending codec on this channel")),
    My_PROPERTY_READONLY(WebRtcChannel, RecCodec, "recv_codec",
                PyDoc_STR("(read-only) The currently received codec for this channel")),
    My_PROPERTY_WRITEONLY(WebRtcChannel, ISACInitTargetRate, "isac_init_target_rate",
                PyDoc_STR("(write-only) The initial values of target rate and frame size for iSAC for this channel. This API is only valid if iSAC is setup to run in channel-adaptive mode")),
    My_PROPERTY_WRITEONLY(WebRtcChannel, ISACMaxRate, "isac_max_rate",
                PyDoc_STR("(write-only) The maximum allowed iSAC rate which the codec may not exceed for a single packet for the specified channel. The maximum rate is defined as payload size per frame size in bits per second.")),
    My_PROPERTY_WRITEONLY(WebRtcChannel, ISACMaxPayloadSize, "isac_max_payload_size",
                PyDoc_STR("(write-only) The maximum allowed iSAC payload size for this channel. The maximum value is set independently of the frame size, i.e. 30 ms and 60 ms packets have the same limit")),
    My_PROPERTY_READWRITE(WebRtcChannel, RecPayloadType, "recv_payload_type",
                PyDoc_STR("The dynamic payload type number for a particular codec or disables (ignores)) a codec for receiving. For instance, when receiving an invite from a SIP-based client, this function can be used to change the dynamic payload type number to match that in the INVITE SDP-message.")),
    My_PROPERTY_WRITEONLY(WebRtcChannel, SendCNPayloadType, "send_cn_payload_type",
                PyDoc_STR("(write-only) The payload type for the sending of SID-frames with background noise estimation during silence periods detected by the VAD")),
    My_PROPERTY_READWRITE(WebRtcChannel, VADStatus, "vad_status",
                PyDoc_STR("the VAD/DTX (silence suppression)) status for this channel")),
    //My_PROPERTY_WRITEONLY(WebRtcChannel, AMREncFormat, "amr_end_format", PyDoc_STR("Not supported")),
    //My_PROPERTY_WRITEONLY(WebRtcChannel, AMRDecFormat, "amr_dec_format", PyDoc_STR("Not supported")),
    //My_PROPERTY_WRITEONLY(WebRtcChannel, AMRWbEncFormat, "amrwb_enc_format", PyDoc_STR("Not supported")),
    //My_PROPERTY_WRITEONLY(WebRtcChannel, AMRWbDecFormat, "amrwb_dec_format", PyDoc_STR("Not supported")),
    
    // voe_call_report.h
    
    My_PROPERTY_READONLY(WebRtcChannel, RoundTripTimeSummary, "round_trip_time_summary",
                PyDoc_STR("(read-only) The minimum, maximum and average levels for Round Trip Time (RTT)) measurements")),
    My_PROPERTY_READONLY(WebRtcChannel, DeadOrAliveSummary, "dead_or_alive_summary",
                PyDoc_STR("(read-only) The total amount of dead and alive connection detections during a VoIP session")),


    // voe_dtmf.h
    
    My_PROPERTY_READWRITE(WebRtcChannel, SendTelephoneEventPayloadType, "send_telephone_event_payload_type",
                PyDoc_STR("the currently set dynamic payload type for telephone events")),
    My_PROPERTY_READWRITE(WebRtcChannel, DtmfPlayoutStatus, "dtmf_playout_status",
                PyDoc_STR("Gets the DTMF playout status")),
    My_PROPERTY_READONLY(WebRtcChannel, TelephoneEventDetectionStatus, "telephone_event_detection_status",
                PyDoc_STR("(read-only) Gets the current telephone-event detection status for a specified channel")),
    
//    My_PROPERTY_READWRITE(WebRtcChannel, , "",
//                PyDoc_STR("")),
    {NULL}  /* Sentinel */
};


static PyTypeObject WebRtcChannelType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "webrtc.Channel",      /*tp_name*/
    sizeof(WebRtcChannel),            /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)WebRtcChannel_dealloc, /*tp_dealloc*/
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
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "Channel object",          /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    WebRtcChannel_methods,     /* tp_methods */
    0,                         /* tp_members */
    WebRtcChannel_getseters,   /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)WebRtcChannel_init,        /* tp_init */
    0,                         /* tp_alloc */
    0,                         /* tp_new */
};

int my_channel_preinit() {
    WebRtcChannelType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&WebRtcChannelType) < 0)
        return -1;
    return 0;
}

void my_channel_addobjects(PyObject* m) {
    Py_INCREF(&WebRtcChannelType);
    PyModule_AddObject(m, "Channel", (PyObject *)&WebRtcChannelType);
}
