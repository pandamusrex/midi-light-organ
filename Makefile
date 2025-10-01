CXXFLAGS=-O3 -W -Wall -Wextra -Wno-unused-parameter -D_FILE_OFFSET_BITS=64

# Zeller RGB
RGB_LIB_DISTRIBUTION=../rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

# Image Magick
MAGICK_CXXFLAGS?=$(shell GraphicsMagick++-config --cppflags --cxxflags)
MAGICK_LDFLAGS?=$(shell GraphicsMagick++-config --ldflags --libs)
AV_CXXFLAGS=$(shell pkg-config --cflags  libavcodec libavformat libswscale libavutil)
AV_LDFLAGS=$(shell pkg-config --cflags --libs  libavcodec libavformat libswscale libavutil)

all: midi-light-organ

midi-light-organ: midi-light-organ.o midi-listener.o midi-mailbox.o note-renderer.o $(RGB_LIBRARY)
	g++ -o midi-light-organ midi-light-organ.o midi-listener.o midi-mailbox.o note-renderer.o $(RGB_LDFLAGS) $(MAGICK_LDFLAGS) $(LDFLAGS) -lasound

midi-light-organ.o: midi-light-organ.cpp
	g++ $(CXXFLAGS) -I$(RGB_INCDIR) $(MAGICK_CXXFLAGS) -c midi-light-organ.cpp

midi-listener.o: midi-listener.cpp
	g++ $(CXXFLAGS) -c midi-listener.cpp

midi-mailbox.o: midi-mailbox.cpp
	g++ $(CXXFLAGS) -c midi-mailbox.cpp

note-renderer.o: note-renderer.cpp
	g++ $(CXXFLAGS) -I$(RGB_INCDIR) -c note-renderer.cpp