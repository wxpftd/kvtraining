#include <fcntl.h>
#include <iostream>
#include <sys/shm.h>
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
	int shm_id, i;
	key_t key;
	char *name= "/dev/shm/myshm2";
	People *p_map;
	key = ftok(name, 0);
	if (-1 == key)
	{
		printf("Ftok failed.\n");
		exit(-1);	
	}
	shm_id = shmget(key, 4096, IPC_CREAT);
	if (-1 == shm_id)
	{
		printf("Shmget failed.\n");	
		exit(-1);
	}
	p_map = (People*)shmat(shm_id, NULL, 0);
	for (i=0; i<10; i++)
	{
		printf("name: %s\n", (*(p_map+i)).name);
		printf("age: %d\n", (*(p_map+i)).age);
	}
	if (shmdt(p_map) == -1)
	{
		printf("Detach failed.\n");	
		exit(-1);
	}
	
	return 0;
}
