#include <iostream>  
#include "charqueue.h"
#include <cstring>
using namespace std;  

int main()  
{  
	char buffer[] = "Hello World.";
	CharQueue charqueue;
	charqueue.push(buffer);
	charqueue.pop(buffer);
	cout << buffer << endl;
	
	return 0;
}       
