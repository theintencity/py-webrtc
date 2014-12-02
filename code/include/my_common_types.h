/*
 * Copyright (c) 2011, Kundan Singh. All Rights Reserved.
 * Copyright (c) 2014, Intencity Cloud Technologies <theintencity@gmail.com>.
 */

#ifndef MY_COMMON_TYPES_H
#define MY_COMMON_TYPES_H 1

extern const char* Error2String(int err);
extern const char* attr2string(int attr, const int attrs[], const char* strings[]);
extern int string2attr(const char* str, const int attrs[], const char* strings[]);

#define My_ATTR2STRING(value, type)                     \
    attr2string(value, type##_Enum, type##_Str)

#define My_STRING2ATTR(value, type)                     \
    string2attr(value, type##_Enum, type##_Str)

#define My_PROTO_CONVERT(attr)                          \
    extern const int attr##_Enum[];                     \
    extern const char* attr##_Str[];

My_PROTO_CONVERT(OnHoldModes)
My_PROTO_CONVERT(NetEqModes)
My_PROTO_CONVERT(NetEqBgnModes)
My_PROTO_CONVERT(NsModes)
My_PROTO_CONVERT(AgcModes)
My_PROTO_CONVERT(AgcConfig)
My_PROTO_CONVERT(EcModes)
My_PROTO_CONVERT(AecmModes)
My_PROTO_CONVERT(VadModes)
My_PROTO_CONVERT(AmrMode)
My_PROTO_CONVERT(TelephoneEventDetectionMethods)
My_PROTO_CONVERT(ProcessingTypes)

#endif
