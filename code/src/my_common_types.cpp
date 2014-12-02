#include <string.h>

#include "common_types.h"

using namespace webrtc;

#include "my_common_types.h"


const int   OnHoldModes_Enum[]  = {kHoldSendAndPlay, kHoldSendOnly, kHoldPlayOnly, -1};
const char* OnHoldModes_Str[]   = {"send-play", "send-only", "play-only"};
const int   NetEqModes_Enum[]   = {kNetEqDefault, kNetEqStreaming, kNetEqFax, -1};
const char* NetEqModes_Str[]    = {"default", "streaming", "fax"};
const int   NetEqBgnModes_Enum[]= {kBgnOn, kBgnFade, kBgnOff, -1};
const char* NetEqBgnModes_Str[] = {"on", "fade", "off"};
const int   NsModes_Enum[]      = {kNsUnchanged, kNsDefault, kNsConference, kNsLowSuppression, kNsModerateSuppression, kNsHighSuppression, kNsVeryHighSuppression, -1};
const char* NsModes_Str[]       = {"unchanged", "default", "conference",  "low", "moderate", "high", "very-high"};
const int   AgcModes_Enum[]     = {kAgcUnchanged, kAgcDefault, kAgcAdaptiveAnalog, kAgcAdaptiveDigital, kAgcFixedDigital, -1};
const char* AgcModes_Str[]      = {"unchanged", "default", "adaptive-analog", "adaptive-digital", "fixed-digital"};
const int   EcModes_Enum[]      = {kEcUnchanged, kEcDefault, kEcConference, kEcAec, kEcAecm, -1};
const char* EcModes_Str[]       = {"unchanged", "default", "conference", "aec", "aecm"};
const int   AecmModes_Enum[]    = {kAecmQuietEarpieceOrHeadset, kAecmEarpiece, kAecmLoudEarpiece, kAecmSpeakerphone, kAecmLoudSpeakerphone, -1};
const char* AecmModes_Str[]     = {"quiet-earpiece", "earpiece", "loud-earpiece", "speakerphone", "loud-speakerphone"};
const int   VadModes_Enum[]      = {kVadConventional, kVadAggressiveLow, kVadAggressiveMid, kVadAggressiveHigh, -1};
const char* VadModes_Str[]       = {"conventional", "low", "mid", "high"};
const int   AmrMode_Enum[]      = {kRfc3267BwEfficient, kRfc3267OctetAligned, kRfc3267FileStorage, -1};
const char* AmrMode_Str[]       = {"bw-efficient", "octet-aligned", "file-storage"};
const int   TelephoneEventDetectionMethods_Enum[] = {kInBand, kOutOfBand, kInAndOutOfBand, -1};
const char* TelephoneEventDetectionMethods_Str[] = {"in-band", "out-of-band", "in-and-out-of-band"};
const int   ProcessingTypes_Enum[] = {kPlaybackPerChannel, kPlaybackAllChannelsMixed, kRecordingPerChannel, kRecordingAllChannelsMixed, -1};
const char* ProcessingTypes_Str[] = {"playback-per-channel", "playback-all-mixed", "recording-per-channel", "recording-all-mixed"};


const char* attr2string(int attr, const int attrs[], const char* strings[])
{
    for (int i=0; attrs[i] >= 0; ++i) {
        if (attr == attrs[i])
            return strings[i];
    }
    return NULL;
}

int string2attr(const char* str, const int attrs[], const char* strings[])
{
    for (int i=0; attrs[i] >= 0; ++i) {
        if (strcmp(str, strings[i]) == 0)
            return attrs[i];
    }
    return -1;
}

