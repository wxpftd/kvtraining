install: a.out 

a.out : test.cpp charqueue.h charqueue.cpp
	g++ -g -Wall -std=c++11 -o a.out test.cpp charqueue.h charqueue.cpp -lboost_serialization -lpthread

b.out: testRelated.cpp
	g++ -Wall -std=c++11 -o b.out testRelated.cpp 

clean: 
	rm a.out
