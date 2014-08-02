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

int main(int argc, char* argv[])
{
	int i;
	People *p_map;
	char temp;
	p_map = (People*)mmap(NULL, sizeof(People)*10, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	if (fork() == 0)
	{
		sleep(2);	
		for (i=0;i<5;i++)
			printf("Child read: the %d people's age is %d\n", i+1, (*(p_map+i)).age);
		(*p_map).age = 100;
		munmap(p_map, sizeof(People)*10);
		exit(0);
	}
	temp = 'a';
	for (i=0; i<5; i++)
	{
		temp += 1;
		memcpy((*(p_map + i)).name, &temp, 1);
		(*(p_map + i)).age = 20 + i;
	}
	printf("Parent read: the first people's age is %d\n", (*p_map).age);
	printf("Umap\n");
	munmap(p_map, sizeof(People)*10);
	printf("Unmap ok.\n");
	return 0;
}
