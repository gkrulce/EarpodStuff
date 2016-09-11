FFT_INCLUDE = $(HOME_DIR)/fftLib/local/include/
FFT_LIBRARY = $(HOME_DIR)/fftLib/local/lib/

CC = g++
CFLAGS = -Wall -std=c++11
LFLAGS = -lfftw3 -lm 
SOURCES = ConversionScript.cpp Frontend.cpp FrontendTest.cpp EarpodModel.cpp EarpodModelTest.cpp
OBJECTS=$(SOURCES:.cpp=.o)
INCLUDE = -I$(FFT_INCLUDE)
L_INCLUDE = -L$(FFT_LIBRARY)

all: $(SOURCES) FrontendTest ConversionScript EarpodModelTest

EarpodModelTest: $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDE) EarpodModelTest.o Frontend.o EarpodModel.o $(L_INCLUDE) $(LFLAGS) -o EarpodModelTest

FrontendTest: $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDE) FrontendTest.o Frontend.o $(L_INCLUDE) $(LFLAGS) -o FrontendTest

ConversionScript: $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDE) ConversionScript.o Frontend.o $(L_INCLUDE) $(LFLAGS) -o ConversionScript

.cpp.o:
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -f *.o ConversionScript FrontendTest

