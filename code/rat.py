# 
#  Copyright (c) 2011, Kundan Singh. All Rights Reserved.
#  Copyright (c) 2014, Intencity Cloud Technologies <theintencity@gmail.com>.
# 


import time, sys, os, traceback
try:
    from webrtc import VoiceEngine, Channel
except:
    traceback.print_exc()

def audio_tool(ip, dest_port, receive_port, source_port, codec_name, rate, channels):
    try:
        #print '%s/%d/%d/%d %s/%d/%d'%(ip, dest_port, receive_port, source_port, codec_name, rate, channels)
    
        voe = VoiceEngine()
        voe.ns_status = "default"
        voe.agc_status = "default"
        voe.ec_status = "default"
        # voe.typing_detection_status = True
        
        ch = Channel(voe)
        if ip.startswith('224.'):
            ch.set_local_receiver(port=receive_port, multicast=ip)
        else:
            ch.set_local_receiver(port=receive_port)
        ch.set_send_destination(port=dest_port, ip=ip, source_port=source_port)
        # ch.vad_status = True
        
        
        codecs = voe.codecs
        found = False
        for codec in codecs:
            if codec['name'].lower() == codec_name.lower() and codec['channels'] == channels and codec['frequency'] == rate:
                ch.send_codec = codec
                found = True
                break
        if not found:
            raise ValueError, 'not found %s/%d/%d'%(codec_name, rate, channels)
            
        ch.start_playout()
        ch.start_receive()
        ch.start_send()
        
        while True:
            time.sleep(1)
            
    except KeyboardInterrupt:
        ch.stop_send()
        ch.stop_receive()
        ch.stop_playout()
        
        del ch
    except:
        traceback.print_exc()
    
if __name__ == '__main__':
    try:
        if len(sys.argv) < 3: raise ValueError, 'need at least 2 arguments'
        ip, dest_port, receive_port, source_port = sys.argv[-2].split("/")
        codec_name, rate, channels = sys.argv[-1].split("/")
    except:
        print 'usage: python %s ip/dest-port/receive-port/source-port codec-name/rate/channels'%(sys.argv[0],)
        sys.exit(-1)
        
    dest_port, receive_port, source_port, rate, channels = map(int, [dest_port, receive_port, source_port, rate, channels])
    
    audio_tool(ip, dest_port, receive_port, source_port, codec_name, rate, channels)
        
    
    
