#a.out: threadMain.cpp threadpool.h threadpool.cpp
#	g++ -Wall -std=c++11 -o a.out threadMain.cpp threadpool.h threadpool.cpp -lpthread

#a.out: main.cpp processpool.h processpool.cpp charqueue.h
#	g++ -Wall -std=c++11 -o a.out main.cpp processpool.h processpool.cpp charqueue.h charqueue.cpp -lboost_serialization -lpthread

install: a.out b.out

a.out: main.cpp socket.h socket.cpp
	g++ -Wall -std=c++11 -o a.out main.cpp socket.h socket.cpp

b.out: clientMain.cpp socket.h socket.cpp
	g++ -Wall -std=c++11 -o b.out clientMain.cpp socket.h socket.cpp



clean: 
	rm a.out
