#a.out: threadMain.cpp threadpool.h threadpool.cpp
#	g++ -Wall -std=c++11 -o a.out threadMain.cpp threadpool.h threadpool.cpp -lpthread

a.out: main.cpp processpool.h processpool.cpp 
	g++ -Wall -std=c++11 -o a.out main.cpp processpool.h processpool.cpp -lboost_serialization -lpthread

clean: 
	rm a.out
