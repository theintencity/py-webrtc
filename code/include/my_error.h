#ifndef MY_ERROR_H
#define MY_ERROR_H 1

#include <Python.h>

extern PyObject *WebRtcError;
extern const char* Error2String(int err);
extern void my_error_addobjects(PyObject* m);

#endif

