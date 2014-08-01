#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <sys/wait.h>
#include <cstring>

using namespace std;

class People
{
public:
	char name[4];
	int age;
};

int main(int argc, char *argv[])
{
	int fd, i;
	People *p_map;
	fd = open(argv[1], O_CREAT|O_RDWR, 00777);
	p_map = (People*)mmap(NULL, sizeof(People)*10, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	for (i=0;i<10;i++)
	{
		printf("Name: %s,age %d;\n", (*(p_map+i)).name, (*(p_map+i)).age);	
		//cout << "Name: " << (*(p_map+i)).name << "age: " << (*(p_map+i)).age << endl;
	}
	munmap(p_map, sizeof(People)*10);
}

