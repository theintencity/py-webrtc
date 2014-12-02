#include <Python.h>

#include "common_types.h"

#include "my_macro.h"
#include "my_error.h"
#include "my_common_types.h"
#include "my_voice_engine.h"
#include "my_channel.h"
#include "my_voe_encryption.h"

using namespace webrtc;


class EncryptionCallback: public Encryption {
public:
    EncryptionCallback(PyObject* encrypt_o, PyObject* decrypt_o, PyObject* encrypt_rtcp_o, PyObject* decrypt_rtcp_o, PyObject* userdata);
    ~EncryptionCallback();
    void encrypt(int channel_no, unsigned char* in_data, unsigned char* out_data, int bytes_in, int* bytes_out);
    void decrypt(int channel_no, unsigned char* in_data, unsigned char* out_data, int bytes_in, int* bytes_out);
    void encrypt_rtcp(int channel_no, unsigned char* in_data, unsigned char* out_data, int bytes_in, int* bytes_out);
    void decrypt_rtcp(int channel_no, unsigned char* in_data, unsigned char* out_data, int bytes_in, int* bytes_out);
    
private:
    PyObject *encrypt_o;
    PyObject *decrypt_o;
    PyObject *encrypt_rtcp_o;
    PyObject *decrypt_rtcp_o;
    PyObject *userdata;
};

EncryptionCallback::EncryptionCallback(PyObject* encrypt_o, PyObject* decrypt_o, PyObject* encrypt_rtcp_o, PyObject* decrypt_rtcp_o, PyObject* userdata)
{
    Py_XINCREF(encrypt_o);
    this->encrypt_o = encrypt_o;
    Py_XINCREF(decrypt_o);
    this->decrypt_o = decrypt_o;
    Py_XINCREF(encrypt_rtcp_o);
    this->encrypt_rtcp_o = encrypt_rtcp_o;
    Py_XINCREF(decrypt_rtcp_o);
    this->decrypt_rtcp_o = decrypt_rtcp_o;
    Py_XINCREF(userdata);
    this->userdata = userdata;
}

EncryptionCallback::~EncryptionCallback() {
    Py_XDECREF(this->encrypt_o);
    Py_XDECREF(this->decrypt_o);
    Py_XDECREF(this->encrypt_rtcp_o);
    Py_XDECREF(this->decrypt_rtcp_o);
    Py_XDECREF(this->userdata);
}

#define DEFINE_ENCRYPTION_METHOD(name)          \
void EncryptionCallback::name(int channel, unsigned char* in_data, unsigned char* out_data, int bytes_in, int* bytes_out)   \
{                                                                                                                           \
    if (this->name##_o != NULL) {                                                                                           \
        PyObject* input = PyString_FromStringAndSize((const char*) in_data, bytes_in);                                                    \
        PyObject* arglist = Py_BuildValue("(OiO)", this->userdata, channel, input);                                         \
        PyObject* output = PyObject_CallObject(this->name##_o, arglist);                                                    \
        Py_DECREF(arglist);                                                                                                 \
        if (PyString_Check(output)) {                                                                                       \
            *bytes_out = PyString_Size(output);                                                                             \
            memcpy(out_data, PyString_AsString(output), *bytes_out);                                                        \
        }                                                                                                                   \
    }                                                                                                                       \
}

DEFINE_ENCRYPTION_METHOD(encrypt)
DEFINE_ENCRYPTION_METHOD(decrypt)
DEFINE_ENCRYPTION_METHOD(encrypt_rtcp)
DEFINE_ENCRYPTION_METHOD(decrypt_rtcp)



My_METHOD_ARGS_KWARGS(WebRtcChannel, RegisterExternalEncryption)
{
    My_NULL_IF_NULL(self->voe->base, VoEBase)
    My_NULL_IF_NULL(self->voe->encryption, VoEEncryption)
    My_NULL_IF_NEG2(self->ch)
    
    PyObject* encrypt_o = NULL;
    PyObject* decrypt_o = NULL;
    PyObject* encrypt_rtcp_o = NULL;
    PyObject* decrypt_rtcp_o = NULL;
    PyObject* userdata = Py_None;
    static char *kwlist[] = {"encrypt", "decrypt", "encrypt_rtcp", "decrypt_rtcp", "userdata", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OOOO|O", kwlist, &encrypt_o, &decrypt_o, &encrypt_rtcp_o, &decrypt_rtcp_o, &userdata)) {
        return NULL;
    }

    if (encrypt_o != Py_None && !PyCallable_Check(encrypt_o)) {
        PyErr_SetString(WebRtcError, "encrypt must be callable or None");
        return NULL;
    }
    if (decrypt_o != Py_None && !PyCallable_Check(decrypt_o)) {
        PyErr_SetString(WebRtcError, "decrypt must be callable or None");
        return NULL;
    }
    if (encrypt_rtcp_o != Py_None && !PyCallable_Check(encrypt_rtcp_o)) {
        PyErr_SetString(WebRtcError, "encrypt_rtcp must be callable or None");
        return NULL;
    }
    if (decrypt_rtcp_o != Py_None && !PyCallable_Check(decrypt_rtcp_o)) {
        PyErr_SetString(WebRtcError, "decrypt_rtcp must be callable or None");
        return NULL;
    }
    
    if (self->encryption_callback != NULL) {
        delete self->encryption_callback;
        self->encryption_callback = NULL;
    }
    
    if (encrypt_o != Py_None && decrypt_o != Py_None && encrypt_rtcp_o != Py_None && decrypt_rtcp_o != Py_None) {
        self->encryption_callback = new EncryptionCallback(encrypt_o, decrypt_o, encrypt_rtcp_o, decrypt_rtcp_o, userdata);
        self->voe->encryption->RegisterExternalEncryption(self->ch, *self->encryption_callback);
    }
    else {
        self->voe->encryption->DeRegisterExternalEncryption(self->ch);
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

