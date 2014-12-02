#ifndef MY_MACRO_H
#define MY_MACRO_H 1

#define My_PROPERTY_READONLY(type, attr, name, desc)    \
    {name, (getter)type##_Get##attr, NULL, desc, NULL}

#define My_PROPERTY_WRITEONLY(type, attr, name, desc)   \
    {name, NULL, (setter)type##_Set##attr, desc, NULL}

#define My_PROPERTY_READWRITE(type, attr, name, desc)   \
    {name, (getter)type##_Get##attr, (setter)type##_Set##attr, desc, NULL}

#define My_PROTO_WRITEONLY(type, attr)                  \
    int type##_Set##attr(type *self, PyObject *value, PyObject *kwargs);

#define My_PROTO_READONLY(type, attr)                   \
    PyObject * type##_Get##attr(type *self, void *closure);

#define My_PROTO_READWRITE(type, attr)                  \
    PyObject * type##_Get##attr(type *self, void *closure); \
    int type##_Set##attr(type *self, PyObject *value, PyObject *kwargs);

#define My_METHOD_DECL(type, attr, flags, name, desc)   \
    {name, (PyCFunction)type##_##attr, flags, PyDoc_STR(desc)}

#define My_PROPERTY_SETTER(type, attr)                  \
    int type##_Set##attr(type *self, PyObject *value, PyObject *kwargs)

#define My_PROPERTY_GETTER(type, attr)                  \
    PyObject * type##_Get##attr(type *self, void *closure)

#define My_METHOD_ARGS_KWARGS(type, name)               \
    PyObject * type##_##name(type *self, PyObject *args, PyObject *kwargs)

#define My_METHOD_ARGS(type, name)                      \
    PyObject * type##_##name(type *self, PyObject *args)

#define My_METHOD(type, name)                           \
    PyObject * type##_##name(type *self, PyObject *unused)


#define My_ERROR_IF_NULL(value, type)                   \
    if (value == NULL) {                                \
        PyErr_SetString(WebRtcError, "Internal Error: invalid " #value " (" #type ")"); \
        return -1;                                      \
    }

#define My_NULL_IF_NULL(value, type)                    \
    if (value == NULL) {                                \
        PyErr_SetString(WebRtcError, "Internal Error: invalid " #value " (" #type ")"); \
        return NULL;                                    \
    }


#define My_ERROR_IF_NEG(value, base)                    \
    if (value < 0) {                                      \
        PyErr_Format(WebRtcError, "%d %s", base->LastError(), Error2String(base->LastError())); \
        return -1;                                      \
    }

#define My_NULL_IF_NEG(value, base)                     \
    if (value < 0) {                                      \
        PyErr_Format(WebRtcError, "%d %s", base->LastError(), Error2String(base->LastError())); \
        return NULL;                                    \
    }


#define My_ERROR_IF_NEG2(value)                    \
    if (value < 0) {                                      \
        PyErr_Format(WebRtcError, "Invalid " #value " (%d)", value); \
        return -1;                                      \
    }

#define My_NULL_IF_NEG2(value)                     \
    if (value < 0) {                                      \
        PyErr_Format(WebRtcError, "Invalid " #value " (%d)", value); \
        return NULL;                                    \
    }

#define My_NULL_IF_NEG3(value, valuestr)                    \
    if (value< 0) {                                      \
        PyErr_Format(WebRtcError, "Invalid " #value " (\"%s\")", valuestr); \
        return NULL;                                      \
    }

#define My_ERROR_IF_NEG3(value, valuestr)                    \
    if (value< 0) {                                      \
        PyErr_Format(WebRtcError, "Invalid " #value " (\"%s\")", valuestr); \
        return -1;                                      \
    }


#define My_STRING_OR_NONE(value, valuestr)              \
    char *valuestr = NULL;                              \
    if (value != Py_None) {                             \
        if (!PyString_Check(value)) {                   \
            PyErr_SetString(WebRtcError, "must be a string"); \
            return -1;                                  \
        }                                               \
        valuestr = PyString_AsString(value);            \
        if (valuestr != NULL && strlen(valuestr) == 0) {\
            valuestr = NULL;                            \
        }                                               \
    }

#define My_STRING(value, valuestr)                      \
    char *valuestr = NULL;                              \
    if (!PyString_Check(value)) {                       \
        PyErr_SetString(WebRtcError, "must be a string"); \
        return -1;                                      \
    }                                                   \
    valuestr = PyString_AsString(value);                \

#define My_INT(value, valueint)                         \
    if (!PyInt_Check(value)) {                          \
        PyErr_SetString(WebRtcError, "must be a integer"); \
        return -1;                                      \
    }                                                   \
    int valueint = PyInt_AsLong(value);                 \

#endif /* My_MACRO_H */
