CC = g++
CFLAGS = -Wall -std=c++11
SOURCES = ConversionScript.cpp Frontend.cpp FrontendTest.cpp EarpodModel.cpp EarpodModelTest.cpp
OBJECTS=$(SOURCES:.cpp=.o) kiss_fft.o

all: $(SOURCES) FrontendTest ConversionScript EarpodModelTest

EarpodModelTest: $(OBJECTS)
	$(CC) $(CFLAGS) EarpodModelTest.o Frontend.o EarpodModel.o kiss_fft.o -o EarpodModelTest

FrontendTest: $(OBJECTS)
	$(CC) $(CFLAGS) FrontendTest.o Frontend.o kiss_fft.o -o FrontendTest

ConversionScript: $(OBJECTS)
	$(CC) $(CFLAGS) ConversionScript.o Frontend.o kiss_fft.o -o ConversionScript

kiss_fft.o:
	$(MAKE) asm -C kiss_fft130/
	$(CC) -c $(CFLAGS) kiss_fft130/kiss_fft.s -o kiss_fft.o

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o ConversionScript FrontendTest

