#include <Python.h>
#include <structmember.h>

#include "my_error.h"
#include "my_voice_engine.h"
#include "my_channel.h"

using namespace webrtc;

extern "C" {
    PyMODINIT_FUNC initwebrtc(void);
}

static PyMethodDef WebRtcMethods[] = {
        
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initwebrtc(void)
{
    PyObject *m;
    
    PyEval_InitThreads();

    if (my_voice_engine_preinit() < 0)
        return;

    if (my_channel_preinit() < 0)
        return;
    
    m = Py_InitModule("webrtc", WebRtcMethods);
    if (m == NULL)
        return;

    my_error_addobjects(m);
    my_voice_engine_addobjects(m);
    my_channel_addobjects(m);
}

