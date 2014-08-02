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
	int fd, i;
	People *p_map;
	char temp;

	fd = open(argv[1], O_CREAT|O_RDWR|O_TRUNC, 00777);
	lseek(fd, sizeof(People)*5-1, SEEK_SET);
	write(fd, "", 1);
	p_map = (People*)mmap(NULL, sizeof(People)*10, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);
	temp = 'a';
	for (i=0; i<10; i++)
	{
		temp += 1;
		memcpy((*(p_map + i)).name, &temp, 1);
		(*(p_map + i)).age = 20 + i;
	}
	printf("Initialize over.\n");
	sleep(10);
	munmap(p_map, sizeof(People)*10);
	printf("Unmap ok.\n");
	return 0;
}
