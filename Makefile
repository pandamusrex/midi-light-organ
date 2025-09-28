RGB_LIB_DISTRIBUTION=../rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

all: midi-light-organ

midi-light-organ: midi-light-organ.o $(RGB_LIBRARY)
	g++ -o midi-light-organ midi-light-organ.o $(LDFLAGS)

midi-light-organ.o: midi-light-organ.cpp
	g++ -I$(RGB_INCDIR) -c midi-light-organ.cpp
