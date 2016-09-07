FFT_INCLUDE = $(HOME_DIR)/fftLib/local/include/
FFT_LIBRARY = $(HOME_DIR)/fftLib/local/lib/

test: frontend.o FrontendTest.cpp
	g++ -Wall -std=c++14 -I$(FFT_INCLUDE) FrontendTest.cpp frontend.o -L$(FFT_LIBRARY) -lfftw3 -lm -o test

frontend.o: frontend.cpp frontend.hpp
	g++ -c -std=c++14 -I $(FFT_INCLUDE) frontend.cpp

clean:
	rm *.o test

