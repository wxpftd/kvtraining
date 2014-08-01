install: a.out b.out

a.out : test.cpp 
	g++ -Wall -std=c++11 -o a.out test.cpp

b.out: testRelated.cpp
	g++ -Wall -std=c++11 -o b.out testRelated.cpp 

clean: 
	rm a.out b.out
