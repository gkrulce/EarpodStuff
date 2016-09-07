FFT_INCLUDE = $(HOME_DIR)/fftLib/local/include/
FFT_LIBRARY = $(HOME_DIR)/fftLib/local/lib/

test: Frontend.o FrontendTest.cpp
	g++ -Wall -std=c++11 -I$(FFT_INCLUDE) FrontendTest.cpp Frontend.o -L$(FFT_LIBRARY) -lfftw3 -lm -o test

Frontend.o: Frontend.cpp Frontend.hpp
	g++ -c -std=c++11 -I $(FFT_INCLUDE) Frontend.cpp

clean:
	rm *.o test

