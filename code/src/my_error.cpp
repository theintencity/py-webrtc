/*
 * Copyright (c) 2011, Kundan Singh. All Rights Reserved.
 * Copyright (c) 2014, Intencity Cloud Technologies <theintencity@gmail.com>.
 */

#include "Python.h"

#include "stdio.h"
#include "string.h"
#include "voe_errors.h"
#include "common_types.h"

using namespace webrtc;

PyObject *WebRtcError;

// generated using this command:
// for x in `cat ../trunk//voice_engine/main/interface/voe_errors.h | grep "\#define VE_" |cut -f2 -d" "`
//    do echo "case $x: return \"$x\";"
// done

const char* Error2String(int err)
{
    switch (err) {
        case VE_PORT_NOT_DEFINED: return "VE_PORT_NOT_DEFINED";
        case VE_CHANNEL_NOT_VALID: return "VE_CHANNEL_NOT_VALID";
        case VE_FUNC_NOT_SUPPORTED: return "VE_FUNC_NOT_SUPPORTED";
        case VE_INVALID_LISTNR: return "VE_INVALID_LISTNR";
        case VE_INVALID_ARGUMENT: return "VE_INVALID_ARGUMENT";
        case VE_INVALID_PORT_NMBR: return "VE_INVALID_PORT_NMBR";
        case VE_INVALID_PLNAME: return "VE_INVALID_PLNAME";
        case VE_INVALID_PLFREQ: return "VE_INVALID_PLFREQ";
        case VE_INVALID_PLTYPE: return "VE_INVALID_PLTYPE";
        case VE_INVALID_PACSIZE: return "VE_INVALID_PACSIZE";
        case VE_NOT_SUPPORTED: return "VE_NOT_SUPPORTED";
        case VE_ALREADY_LISTENING: return "VE_ALREADY_LISTENING";
        case VE_CHANNEL_NOT_CREATED: return "VE_CHANNEL_NOT_CREATED";
        case VE_MAX_ACTIVE_CHANNELS_REACHED: return "VE_MAX_ACTIVE_CHANNELS_REACHED";
        case VE_REC_CANNOT_PREPARE_HEADER: return "VE_REC_CANNOT_PREPARE_HEADER";
        case VE_REC_CANNOT_ADD_BUFFER: return "VE_REC_CANNOT_ADD_BUFFER";
        case VE_PLAY_CANNOT_PREPARE_HEADER: return "VE_PLAY_CANNOT_PREPARE_HEADER";
        case VE_ALREADY_SENDING: return "VE_ALREADY_SENDING";
        case VE_INVALID_IP_ADDRESS: return "VE_INVALID_IP_ADDRESS";
        case VE_ALREADY_PLAYING: return "VE_ALREADY_PLAYING";
        case VE_NOT_ALL_VERSION_INFO: return "VE_NOT_ALL_VERSION_INFO";
        case VE_DTMF_OUTOF_RANGE: return "VE_DTMF_OUTOF_RANGE";
        case VE_INVALID_CHANNELS: return "VE_INVALID_CHANNELS";
        case VE_SET_PLTYPE_FAILED: return "VE_SET_PLTYPE_FAILED";
        case VE_ENCRYPT_NOT_INITED: return "VE_ENCRYPT_NOT_INITED";
        case VE_NOT_INITED: return "VE_NOT_INITED";
        case VE_NOT_SENDING: return "VE_NOT_SENDING";
        case VE_EXT_TRANSPORT_NOT_SUPPORTED: return "VE_EXT_TRANSPORT_NOT_SUPPORTED";
        case VE_EXTERNAL_TRANSPORT_ENABLED: return "VE_EXTERNAL_TRANSPORT_ENABLED";
        case VE_STOP_RECORDING_FAILED: return "VE_STOP_RECORDING_FAILED";
        case VE_INVALID_RATE: return "VE_INVALID_RATE";
        case VE_INVALID_PACKET: return "VE_INVALID_PACKET";
        case VE_NO_GQOS: return "VE_NO_GQOS";
        case VE_INVALID_TIMESTAMP: return "VE_INVALID_TIMESTAMP";
        case VE_RECEIVE_PACKET_TIMEOUT: return "VE_RECEIVE_PACKET_TIMEOUT";
        case VE_STILL_PLAYING_PREV_DTMF: return "VE_STILL_PLAYING_PREV_DTMF";
        case VE_INIT_FAILED_WRONG_EXPIRY: return "VE_INIT_FAILED_WRONG_EXPIRY";
        case VE_SENDING: return "VE_SENDING";
        case VE_ENABLE_IPV6_FAILED: return "VE_ENABLE_IPV6_FAILED";
        case VE_FUNC_NO_STEREO: return "VE_FUNC_NO_STEREO";
        case VE_FW_TRAVERSAL_ALREADY_INITIALIZED: return "VE_FW_TRAVERSAL_ALREADY_INITIALIZED";
        case VE_PACKET_RECEIPT_RESTARTED: return "VE_PACKET_RECEIPT_RESTARTED";
        case VE_NOT_ALL_INFO: return "VE_NOT_ALL_INFO";
        case VE_CANNOT_SET_SEND_CODEC: return "VE_CANNOT_SET_SEND_CODEC";
        case VE_CODEC_ERROR: return "VE_CODEC_ERROR";
        case VE_NETEQ_ERROR: return "VE_NETEQ_ERROR";
        case VE_RTCP_ERROR: return "VE_RTCP_ERROR";
        case VE_INVALID_OPERATION: return "VE_INVALID_OPERATION";
        case VE_CPU_INFO_ERROR: return "VE_CPU_INFO_ERROR";
        case VE_SOUNDCARD_ERROR: return "VE_SOUNDCARD_ERROR";
        case VE_SPEECH_LEVEL_ERROR: return "VE_SPEECH_LEVEL_ERROR";
        case VE_SEND_ERROR: return "VE_SEND_ERROR";
        case VE_CANNOT_REMOVE_CONF_CHANNEL: return "VE_CANNOT_REMOVE_CONF_CHANNEL";
        case VE_PLTYPE_ERROR: return "VE_PLTYPE_ERROR";
        case VE_SET_FEC_FAILED: return "VE_SET_FEC_FAILED";
        case VE_CANNOT_GET_PLAY_DATA: return "VE_CANNOT_GET_PLAY_DATA";
        case VE_APM_ERROR: return "VE_APM_ERROR";
        case VE_RUNTIME_PLAY_WARNING: return "VE_RUNTIME_PLAY_WARNING";
        case VE_RUNTIME_REC_WARNING: return "VE_RUNTIME_REC_WARNING";
        case VE_NOT_PLAYING: return "VE_NOT_PLAYING";
        case VE_SOCKETS_NOT_INITED: return "VE_SOCKETS_NOT_INITED";
        case VE_CANNOT_GET_SOCKET_INFO: return "VE_CANNOT_GET_SOCKET_INFO";
        case VE_INVALID_MULTICAST_ADDRESS: return "VE_INVALID_MULTICAST_ADDRESS";
        case VE_DESTINATION_NOT_INITED: return "VE_DESTINATION_NOT_INITED";
        case VE_RECEIVE_SOCKETS_CONFLICT: return "VE_RECEIVE_SOCKETS_CONFLICT";
        case VE_SEND_SOCKETS_CONFLICT: return "VE_SEND_SOCKETS_CONFLICT";
        case VE_TYPING_NOISE_WARNING: return "VE_TYPING_NOISE_WARNING";
        case VE_SATURATION_WARNING: return "VE_SATURATION_WARNING";
        case VE_NOISE_WARNING: return "VE_NOISE_WARNING";
        case VE_CANNOT_GET_SEND_CODEC: return "VE_CANNOT_GET_SEND_CODEC";
        case VE_CANNOT_GET_REC_CODEC: return "VE_CANNOT_GET_REC_CODEC";
        case VE_ALREADY_INITED: return "VE_ALREADY_INITED";
        case VE_RTCP_SOCKET_ERROR: return "VE_RTCP_SOCKET_ERROR";
        case VE_MIC_VOL_ERROR: return "VE_MIC_VOL_ERROR";
        case VE_SPEAKER_VOL_ERROR: return "VE_SPEAKER_VOL_ERROR";
        case VE_CANNOT_ACCESS_MIC_VOL: return "VE_CANNOT_ACCESS_MIC_VOL";
        case VE_CANNOT_ACCESS_SPEAKER_VOL: return "VE_CANNOT_ACCESS_SPEAKER_VOL";
        case VE_GET_MIC_VOL_ERROR: return "VE_GET_MIC_VOL_ERROR";
        case VE_GET_SPEAKER_VOL_ERROR: return "VE_GET_SPEAKER_VOL_ERROR";
        case VE_THREAD_RTCP_ERROR: return "VE_THREAD_RTCP_ERROR";
        case VE_CANNOT_INIT_APM: return "VE_CANNOT_INIT_APM";
        case VE_SEND_SOCKET_TOS_ERROR: return "VE_SEND_SOCKET_TOS_ERROR";
        case VE_CANNOT_RETRIEVE_DEVICE_NAME: return "VE_CANNOT_RETRIEVE_DEVICE_NAME";
        case VE_SRTP_ERROR: return "VE_SRTP_ERROR";
        case VE_INTERFACE_NOT_FOUND: return "VE_INTERFACE_NOT_FOUND";
        case VE_TOS_GQOS_CONFLICT: return "VE_TOS_GQOS_CONFLICT";
        case VE_CANNOT_ADD_CONF_CHANNEL: return "VE_CANNOT_ADD_CONF_CHANNEL";
        case VE_BUFFER_TOO_SMALL: return "VE_BUFFER_TOO_SMALL";
        case VE_CANNOT_EXECUTE_SETTING: return "VE_CANNOT_EXECUTE_SETTING";
        case VE_CANNOT_RETRIEVE_SETTING: return "VE_CANNOT_RETRIEVE_SETTING";
        case VE_RTP_KEEPALIVE_FAILED: return "VE_RTP_KEEPALIVE_FAILED";
        case VE_SEND_DTMF_FAILED: return "VE_SEND_DTMF_FAILED";
        case VE_CANNOT_RETRIEVE_CNAME: return "VE_CANNOT_RETRIEVE_CNAME";
        case VE_DECRYPTION_FAILED: return "VE_DECRYPTION_FAILED";
        case VE_ENCRYPTION_FAILED: return "VE_ENCRYPTION_FAILED";
        case VE_CANNOT_RETRIEVE_RTP_STAT: return "VE_CANNOT_RETRIEVE_RTP_STAT";
        case VE_GQOS_ERROR: return "VE_GQOS_ERROR";
        case VE_BINDING_SOCKET_TO_LOCAL_ADDRESS_FAILED: return "VE_BINDING_SOCKET_TO_LOCAL_ADDRESS_FAILED";
        case VE_TOS_INVALID: return "VE_TOS_INVALID";
        case VE_TOS_ERROR: return "VE_TOS_ERROR";
        case VE_CANNOT_RETRIEVE_VALUE: return "VE_CANNOT_RETRIEVE_VALUE";
        case VE_PLAY_UNDEFINED_SC_ERR: return "VE_PLAY_UNDEFINED_SC_ERR";
        case VE_REC_CANNOT_OPEN_SC: return "VE_REC_CANNOT_OPEN_SC";
        case VE_SOCKET_ERROR: return "VE_SOCKET_ERROR";
        case VE_MMSYSERR_INVALHANDLE: return "VE_MMSYSERR_INVALHANDLE";
        case VE_MMSYSERR_NODRIVER: return "VE_MMSYSERR_NODRIVER";
        case VE_MMSYSERR_NOMEM: return "VE_MMSYSERR_NOMEM";
        case VE_WAVERR_UNPREPARED: return "VE_WAVERR_UNPREPARED";
        case VE_WAVERR_STILLPLAYING: return "VE_WAVERR_STILLPLAYING";
        case VE_UNDEFINED_SC_ERR: return "VE_UNDEFINED_SC_ERR";
        case VE_UNDEFINED_SC_REC_ERR: return "VE_UNDEFINED_SC_REC_ERR";
        case VE_THREAD_ERROR: return "VE_THREAD_ERROR";
        case VE_CANNOT_START_RECORDING: return "VE_CANNOT_START_RECORDING";
        case VE_PLAY_CANNOT_OPEN_SC: return "VE_PLAY_CANNOT_OPEN_SC";
        case VE_NO_WINSOCK_2: return "VE_NO_WINSOCK_2";
        case VE_SEND_SOCKET_ERROR: return "VE_SEND_SOCKET_ERROR";
        case VE_BAD_FILE: return "VE_BAD_FILE";
        case VE_EXPIRED_COPY: return "VE_EXPIRED_COPY";
        case VE_NOT_AUTHORISED: return "VE_NOT_AUTHORISED";
        case VE_RUNTIME_PLAY_ERROR: return "VE_RUNTIME_PLAY_ERROR";
        case VE_RUNTIME_REC_ERROR: return "VE_RUNTIME_REC_ERROR";
        case VE_BAD_ARGUMENT: return "VE_BAD_ARGUMENT";
        case VE_LINUX_API_ONLY: return "VE_LINUX_API_ONLY";
        case VE_REC_DEVICE_REMOVED: return "VE_REC_DEVICE_REMOVED";
        case VE_NO_MEMORY: return "VE_NO_MEMORY";
        case VE_BAD_HANDLE: return "VE_BAD_HANDLE";
        case VE_RTP_RTCP_MODULE_ERROR: return "VE_RTP_RTCP_MODULE_ERROR";
        case VE_AUDIO_CODING_MODULE_ERROR: return "VE_AUDIO_CODING_MODULE_ERROR";
        case VE_AUDIO_DEVICE_MODULE_ERROR: return "VE_AUDIO_DEVICE_MODULE_ERROR";
        case VE_CANNOT_START_PLAYOUT: return "VE_CANNOT_START_PLAYOUT";
        case VE_CANNOT_STOP_RECORDING: return "VE_CANNOT_STOP_RECORDING";
        case VE_CANNOT_STOP_PLAYOUT: return "VE_CANNOT_STOP_PLAYOUT";
        case VE_CANNOT_INIT_CHANNEL: return "VE_CANNOT_INIT_CHANNEL";
        case VE_RECV_SOCKET_ERROR: return "VE_RECV_SOCKET_ERROR";
        case VE_SOCKET_TRANSPORT_MODULE_ERROR: return "VE_SOCKET_TRANSPORT_MODULE_ERROR";
        case VE_AUDIO_CONF_MIX_MODULE_ERROR: return "VE_AUDIO_CONF_MIX_MODULE_ERROR";
        case VE_IGNORED_FUNCTION: return "VE_IGNORED_FUNCTION";
        default: return "<unknown>";
    }
}

void my_error_addobjects(PyObject* m) {
    WebRtcError = PyErr_NewException((char*) "webrtc.error", NULL, NULL);
    Py_INCREF(WebRtcError);
    PyModule_AddObject(m, "error", WebRtcError);
}
