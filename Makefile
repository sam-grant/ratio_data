Target  = RatioMethod_Sam.exe
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs)
all:$(Target)

RatioMethod_Sam.exe: RatioMethod_Sam.cc
	g++ -o $@ RatioMethod_Sam.cc $(Objects) $(ROOTFLAGS) $(ROOTLIBS)
clean:
	rm *exe
