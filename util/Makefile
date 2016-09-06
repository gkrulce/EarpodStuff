test: frontend.o FrontendTest.cpp
	g++ -Wall -std=c++11 frontend.o FrontendTest.cpp -o test

frontend.o: frontend.cpp frontend.hpp
	g++ -c -std=c++11 frontend.cpp

clean:
	rm *.o test

