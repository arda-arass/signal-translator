PROG = signal-translator
SRCS = main.cpp dsp.cpp

CLEANFILES = $(PROG)

INCLUDE = -I${HOME}/anaconda3/envs/py2/include/python2.7
INCLUDE += -I$(shell pwd)/../fftw3/usr/include
LDPATHS = -L${HOME}/anaconda3/envs/py2/lib
LDPATHS += -L$(shell pwd)/../fftw3/usr/lib
CFLAGS = $(INCLUDE) $(LDPATHS) -std=c++11 -DWITHOUT_NUMPY
LDFLAGS = -lpython2.7 -lfftw3

all: $(PROG)

$(PROG): $(SRCS)
	$(CXX) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o $(CLEANFILES) 
