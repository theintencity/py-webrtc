#  
#  Copyright (c) 2011, Kundan Singh. All Rights Reserved.
#  Copyright (c) 2014, Intencity Cloud Technologies <theintencity@gmail.com>.
#  

import time, sys, os, traceback

try:
    import webrtc
except:
    traceback.print_exc()
    sys.exit(-1)

# run using
# export VERSIONER_PYTHON_PREFER_32_BIT=yes
# arch -i386 /usr/bin/python2.6 test.py

def main():
    '''
    Create the voice engine, and check its version.
    
    >>> voe = webrtc.VoiceEngine()
    >>> print voe.version.split('\\n')[0]
    VoiceEngine 4.1.0
    
    List of all the methods and attributes of a channel.
    
    print '\\n'.join([x for x in sorted(dir(voe)) if not x.startswith('_')])
    
    Get The maximum number of channels supported in the voice engine.
    
    >>> print voe.max_channels
    32
    
    Set and reset the trace file name if needed.
    
    >>> voe.trace_filename = 'webrtc.log'
    >>> voe.trace_filename = None
    
    Set or reset the error callback handler on the voice engine.
    
    >>> def voe_callback(ch_id, error, data):
    ...    print "voe_callback(%r, %r, %r)"%(ch_id, error, data)
    >>> voe.set_observer(voe_callback, voe)
    >>> voe.set_observer(None)
    
    Create a new voice channel and print its ID.
    
    >>> ch = webrtc.Channel(voe)
    >>> print ch.id
    0
    
    List of all the methods and attributes of a channel.
    
    print '\\n'.join([x for x in sorted(dir(ch)) if not x.startswith('_')])
    
    Manipulate the local receiver and send destination properties.
    
    >>> print ch.local_receiver, ch.send_destination
    ('', 0, 0) ('', 0, 0, 0)
    >>> ch.set_local_receiver(10000)
    >>> ch.set_send_destination(port=10000, ip="127.0.0.1")
    >>> print ch.local_receiver, ch.send_destination
    ('0.0.0.0', 10000, 10001) ('127.0.0.1', 10000, 10001, 10000)
    >>> ch.set_local_receiver(ip='127.0.0.1', port=10000, rtcp_port=20000)
    >>> ch.set_send_destination(ip='127.0.0.1', port=30000, rtcp_port=30003, source_port=30004)
    >>> print ch.local_receiver, ch.send_destination
    ('127.0.0.1', 10000, 20000) ('127.0.0.1', 30000, 30003, 30004)
    
    Manipulate the other channel properties.
    
    >>> print ch.on_hold_status
    None
    >>> ch.on_hold_status = "play-only"  # values: "send-play", "send-only", "play-only", None
    >>> print ch.on_hold_status
    play-only
    >>> ch.on_hold_status = None
    >>> print ch.on_hold_status
    None
    
    >>> print ch.neteq_playout_mode
    default
    >>> ch.neteq_playout_mode = "streaming"   # values: "default", "streaming", "fax"
    >>> print ch.neteq_playout_mode
    streaming
    >>> try: ch.neteq_playout_mode = None     # cannot assign to None
    ... except: print 'exception'
    exception
    >>> ch.neteq_playout_mode = "default"
    
    >>> print ch.neteq_background_mode
    on
    >>> ch.neteq_background_mode = "fade"     # value: "on", "fade", "off"
    >>> print ch.neteq_background_mode
    fade
    >>> ch.neteq_background_mode = "on"
    
    Start and stop various streams for playout, send and receive.
    
    >>> ch.start_playout()
    >>> ch.start_receive()
    >>> ch.start_send()
    >>> ch.stop_send()
    >>> ch.stop_receive()
    >>> ch.stop_playout()
    
    The audio processing properties are critical in tuning the audio quality.
    Some of these properties take None to disable, while others don't.
    Some of these properties have abstract values such as "unchanged" and "default" which when set
    will pick up the preconfigured value for that meta value. Hence reading the property after setting
    it to "unchanged" or "default" will give a different value then what was set.
    
    >>> print voe.ns_status, voe.agc_status, voe.ec_status, voe.aecm_mode
    None adaptive-analog None speakerphone
    >>> voe.ns_status = "default"             # values: "unchanged", "default", "conference",  "low", "moderate", "high", "very-high", None
    >>> voe.agc_status = "default"            # values: "unchanged", "default", "adaptive-analog", "adaptive-digital", "fixed-digital", None
    >>> voe.ec_status = "default"             # values: "unchanged", "default", "conference", "aec", "aecm", None
    >>> voe.aecm_mode = "loud-speakerphone"   # values: "quiet-earpiece", "earpiece", "loud-earpiece", "speakerphone", "loud-speakerphone"
    >>> print voe.ns_status, voe.agc_status, voe.ec_status, voe.aecm_mode
    moderate adaptive-analog aec loud-speakerphone
    >>> print voe.agc_config
    (3, 9, True)
    >>> voe.agc_config = (3, 9, True)        # (targetLeveldBOv:int, digitalCompressionGaindB:int, limiterEnable:bool)
    
    >>> print ch.rx_ns_status, ch.rx_agc_status, ch.rx_agc_config
    None None (3, 9, True)
    >>> ch.rx_ns_status = "default"           # same as voe.ns_status but for received signal
    >>> ch.rx_agc_status = "default"          # same as voe.agc_status but for received signal
    >>> ch.rx_agc_config = (3, 9, True)       # same as voe.agc_config but for received signal
    >>> print ch.rx_ns_status, ch.rx_agc_status, ch.rx_agc_config
    moderate adaptive-digital (3, 9, True)

    Set or reset the rx_vad_observer for receive signals voice activity detection
    
    >>> def vad_callback(ch_id, vad_decision, data): print ch_id, vad_decision, data
    >>> ch.set_rx_vad_observer(vad_callback, ch)
    >>> ch.set_rx_vad_observer(None)
    >>> ch.voice_activity
    False
    
    Some metrics related properties such as metrics_status, speech_metrics, noise_metrics and echo_metrics
    give error when setting metrics_status.
    
    Start and stop bebug recording for audio processing data for future analysis.
    
    >>> voe.debug_recording_filename = "recording.log"
    >>> voe.debug_recording_filename = None   # to stop recording
    >>> try: voe.debug_recording_filename
    ... except AttributeError: print 'not readable'
    not readable
    
    The typing detection status
    
    >>> print voe.typing_detection_status
    False
    >>> voe.typing_detection_status = True
    >>> print voe.typing_detection_status
    True
    
    Get the codecs count and list.
    >>> print voe.num_codecs
    14
    >>> print voe.codecs
    [{'name': 'ISAC', 'channels': 1, 'rate': 32000, 'frequency': 16000, 'type': 103, 'size': 480}, {'name': 'ISAC', 'channels': 1, 'rate': 56000, 'frequency': 32000, 'type': 104, 'size': 960}, {'name': 'L16', 'channels': 1, 'rate': 128000, 'frequency': 8000, 'type': 105, 'size': 80}, {'name': 'L16', 'channels': 1, 'rate': 256000, 'frequency': 16000, 'type': 107, 'size': 160}, {'name': 'L16', 'channels': 1, 'rate': 512000, 'frequency': 32000, 'type': 108, 'size': 320}, {'name': 'PCMU', 'channels': 1, 'rate': 64000, 'frequency': 8000, 'type': 0, 'size': 160}, {'name': 'PCMA', 'channels': 1, 'rate': 64000, 'frequency': 8000, 'type': 8, 'size': 160}, {'name': 'iLBC', 'channels': 1, 'rate': 13300, 'frequency': 8000, 'type': 102, 'size': 240}, {'name': 'G722', 'channels': 1, 'rate': 64000, 'frequency': 16000, 'type': 9, 'size': 320}, {'name': 'CN', 'channels': 1, 'rate': 0, 'frequency': 8000, 'type': 13, 'size': 240}, {'name': 'CN', 'channels': 1, 'rate': 0, 'frequency': 16000, 'type': 98, 'size': 480}, {'name': 'CN', 'channels': 1, 'rate': 0, 'frequency': 32000, 'type': 99, 'size': 960}, {'name': 'telephone-event', 'channels': 1, 'rate': 0, 'frequency': 8000, 'type': 106, 'size': 240}, {'name': 'red', 'channels': 1, 'rate': 0, 'frequency': 8000, 'type': 127, 'size': 0}]

    >>> print ch.send_codec
    {'name': 'PCMU', 'channels': 1, 'rate': 64000, 'frequency': 8000, 'type': 0, 'size': 160}
    
    >>> ch.send_codec = {'name': 'ISAC', 'channels': 1, 'rate': 32000, 'frequency': 16000, 'type': 103, 'size': 480}
    >>> print ch.send_codec
    {'name': 'ISAC', 'channels': 1, 'rate': 32000, 'frequency': 16000, 'type': 103, 'size': 480}
    
    >>> print ch.recv_codec                   # will be None if not receiving
    None
    
    TODO What are the allowed values for these?
    ch.isac_init_target_rate = 6400
    ch.isac_max_rate = 6400
    ch.isac_max_payload_size = 480
    ch.recv_payload_type
    
    >>> ch.send_cn_payload_type = (96, 16000)
    
    Manipulate voice activity detection related properties.
    
    >>> print ch.vad_status
    None
    >>> ch.vad_status = True
    >>> print ch.vad_status
    ('conventional', False)
    >>> ch.vad_status = ('high', True)
    >>> print ch.vad_status
    ('high', True)
    >>> ch.vad_status = None
    >>> print ch.vad_status
    None
    
    External media process.
    
    Close the channel by removing all references.
    >>> del ch
    '''
    


#print audio.Device(type="input")

#print audio.get_codecs()
#
## audio capture in speex/8000
#audio.connect(audio.input("l16/16000"),
#              audio.pipe("l16/16000", "l16/8000"),
#              audio.pipe("l16/8000", "speex/8000"),
#              audio.port("speex/8000", on_data))
#
## audio playback in pcmu/16000
#p2 = audio.port("pcmu/16000")
#audio.connect(p2,
#              audio.pipe("pcmu/16000", "l16/16000"),
#              audio.pipe("l16/16000", "l16/44100"),
#              audio.output("l16/44100", device="default"))
#
#while True:
#    p2.put('some audio data')
#    
## audio mixer from multiple sources
#s = audio.mixer("l16/8000")
#p1 = audio.port("pcmu/8000")
#p2 = audio.port("speex/16000")
#p3 = audio.port("gsm/8000")
#audio.connect(p1, audio.pipe("pcmu/8000", "l16/8000"), s)
#audio.connect(p2, audio.pipe("speex/16000", "l16/16000"), audio.pipe("l16/16000", "l16/8000"), s)
#audio.connect(p3, audio.pipe("gsm/8000", "l16/8000"), s)
#audio.connect(s, audio.subtract(p1), audio.pipe("l16/8000", "pcmu/8000"), p1)
#audio.connect(s, audio.subtract(p2), audio.pipe("l16/8000", "l16/16000"), audio.pipe("l16/16000", "speex/16000"), p2)
#audio.connect(s, audio.subtract(p3), audio.pipe("l16/8000", "gsm/8000"), p3)
#
#sys.exit()
#
#print 'current-api=', rtaudio.get_current_api()
#print 'device_count=', rtaudio.get_device_count()
#for i in range(rtaudio.get_device_count()):
#    print 'device', i, rtaudio.get_device_info(i)
#print 'default-output=', rtaudio.get_default_output_device()
#print 'default-input=', rtaudio.get_default_input_device()
#
##sys.exit()
#
#queue = []
#
##def callback(in_buf, frames_count, streamTime, status, userdata):
#def callback(input, buffer_frames, stream_time, status, userdata):
#    # print 'callback', len(input), buffer_frames, stream_time, status, userdata
#    queue.append(input)
#    if len(queue) > 100:
#        input = queue.pop(0)
#        return input + input
#    else:
#        return ""
#    
## start
#try:
#    print 'open_stream'
#    rtaudio.open_stream(output_device=rtaudio.get_default_output_device(), output_channels=2,
#                        input_device=rtaudio.get_default_input_device(), input_channels=1,
#                        format='s16', callback=callback, flags=1)
#    print 'start_stream'
#    rtaudio.start_stream()
#except rtaudio.error, e:
#    print e
#    
#try:
#    while True:
#        time.sleep(10)
#except KeyboardInterrupt:
#    if rtaudio.is_stream_running:
#        print 'stop_stream'
#        rtaudio.stop_stream()
#    if rtaudio.is_stream_open:
#        print 'close_stream'
#        rtaudio.close_stream()
#            
    
if __name__ == "__main__":
    import doctest
    doctest.testmod()
    
