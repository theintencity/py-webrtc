all : module

module: $(LIBRARIES) main.cpp Makefile
	rm -f build/lib.*/*.so
	ARCHFLAGS="-arch i386" python setup.py -v build
	cp build/lib.*/webrtc.so .
	
clean:
	rm -rf build
