a.out: test.cpp threadpool.h threadpool.cpp
	g++ -Wall -std=c++11 -o a.out test.cpp threadpool.h threadpool.cpp -lpthread

clean: 
	rm a.out
