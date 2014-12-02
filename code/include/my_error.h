/*
 * Copyright (c) 2011, Kundan Singh. All Rights Reserved.
 * Copyright (c) 2014, Intencity Cloud Technologies <theintencity@gmail.com>.
 */

#ifndef MY_ERROR_H
#define MY_ERROR_H 1

#include <Python.h>

extern PyObject *WebRtcError;
extern const char* Error2String(int err);
extern void my_error_addobjects(PyObject* m);

#endif

