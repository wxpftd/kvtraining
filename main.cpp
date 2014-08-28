#include <sys/mman.h>
#include <iostream>
#include <unistd.h>
#include "processpool.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <string>
#include <sstream>


using namespace std;
using namespace mmtraining;
using namespace boost::archive;
using namespace boost::serialization;

class MyRun : public Runnable
{
public:
	int *count;
	int fd;
	MyRun() 
	{
		//fd = open("/tmp/test_shm", O_CREAT|O_RDWR|O_TRUNC, 00777);
		//lseek(fd, sizeof(int)-1, SEEK_SET);
		//write(fd, "", 1);
		count = (int*)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
		if (MAP_FAILED == count)
		{
			cerr << "mmap init failed." << endl;  
			exit(-1);
		}	
		(*count) = 0;
	}

	int Run()
	{
		(*count)++;
		cout << (*count) << endl; 
		munmap(count, sizeof(int));
		exit(0);
		return 0;
	}
	
private:

	friend class boost::serialization::access;
	template <typename Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & count;
	}
		
};

class MyProcess : public Process
{
public:
	int DoRun()
	{
		printf("MyProcess is running.\n");	
		exit(0);
	}
};

class MyTask : public Task 
{
public:
	MyTask()
	{
		name_ = "First\n";		
	}

	MyTask(const string name)
	{
		name_ = name;	
	}
	
	int DoTask()
	{
		printf("pid is %d:",getpid());
		cout << name_ << endl;
		return 0;
	}

	int ToBuffer(string &buffer)
	{
		buffer = name_;
		//stringstream ss(buffer);	
		//boost::archive::text_oarchive oa(ss);
		//oa << (*this);
		return 0;
		return -1;	
	}

	int FromBuffer(string &buffer)
	{
		this->name_ = buffer;
		//stringstream ss(buffer);
		//boost::archive::text_iarchive ia(ss);
		//ia >> (*this);
		return 0;
		return -1;		
	}

private:
	//friend class boost::serialization::access;
	//template<typename Archive>
	//void serialize(Archive &ar, const unsigned int version)
	//{
	//	ar & name_;	
	//}
	string name_;
};

int main()
{
	MyTask *myTask = new MyTask("Main Functon.");
	TaskProcessPool *tpp = new TaskProcessPool(*myTask);
	tpp->init();
	tpp->Start(200);
	for (int i=0; i<100000; i++)
		tpp->AddTask(*myTask);
	sleep(5);
	cout << "pp.KillAll():" <<  tpp->KillAll() << endl;
	cout << "pp.WaitAll():" <<  tpp->WaitAll() << endl;
	tpp->destroy();	
	delete myTask;
	delete tpp;
	return 0;
}
