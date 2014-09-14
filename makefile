#a.out: threadMain.cpp threadpool.h threadpool.cpp
#	g++ -Wall -std=c++11 -o a.out threadMain.cpp threadpool.h threadpool.cpp -lpthread

#a.out: main.cpp processpool.h processpool.cpp charqueue.h
#	g++ -Wall -std=c++11 -o a.out main.cpp processpool.h processpool.cpp charqueue.h charqueue.cpp -lboost_serialization -lpthread

install: a.out 

#a.out: serverMain.cpp socket.h socket.cpp
#	g++ -Wall -std=c++11 -o a.out serverMain.cpp socket.h socket.cpp

#b.out: clientMain.cpp socket.h socket.cpp
#	g++ -Wall -std=c++11 -o b.out clientMain.cpp socket.h socket.cpp


a.out: main.cpp echoser.h echoser.cpp socket.h socket.cpp threadpool.h threadpool.cpp
	g++ -g -Wall -std=c++11 -o a.out main.cpp echoser.h echoser.cpp socket.h socket.cpp threadpool.h threadpool.cpp -lpthread




clean: 
	rm a.out
