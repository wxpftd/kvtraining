#include <sys/mman.h>
#include <iostream>  
#include <sstream>  
#include <string>  
#include <boost/archive/text_iarchive.hpp>  
#include <boost/archive/text_oarchive.hpp>  
#include <boost/serialization/vector.hpp>  
#include <pthread.h>
#include <fstream>  
#include <fcntl.h>
#include <unistd.h>
using namespace std;  
using namespace boost::serialization;  
using namespace boost::archive;  
pthread_mutex_t *p_mutex;

void init_mutex(void)
{
	int ret;
	p_mutex = (pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_WRITE|PROT_READ, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	if (MAP_FAILED == p_mutex)
	{
		cerr << "mmap init failed." << endl;	
		exit(-1);
	}

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	ret = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	if (0 != ret)
	{
		cerr << "pthread_mutexattr_setpshared failed." << endl;	
		exit(-1);
	}
	pthread_mutex_init(p_mutex, &attr);
}

class People
{
public:
	People()
	{
	}
	People(int age)
	{
		age_ = age;	
	}
	
	int getAge()
	{
		return age_;	
	}
private:
	friend class boost::serialization::access;
	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)	
	{
		ar & age_;	
	}
	int age_;
};


void save(string &buffer)
{
	stringstream ss(buffer);
	boost::archive::text_oarchive oa(ss);
	People p(10);
	oa << p;
	buffer = ss.str();
}

void load(string &buffer)
{
	stringstream ss(buffer); 
	boost::archive::text_iarchive ia(ss);
	People p;
	ia >> p;
	cout << p.getAge() << endl; 
}

int main()  
{  
	//string buffer;
	//save(buffer);
	//load(buffer);
	init_mutex();    
    int ret;        
    char str1[]="this is child process\n";    
    char str2[]="this is father process\n";    
    int fd=open("tmp", O_RDWR|O_CREAT|O_TRUNC, 0666);    
    if( -1==fd )    
    {    
        perror("open");    
        exit(1);    
    }    
    pid_t pid;    
    pid=fork();    
    if( pid<0 )    
    {    
        perror("fork");    
        exit(1);    
    }    
    else if( 0==pid )    
    {    
        ret=pthread_mutex_lock(p_mutex);    
        if( ret!=0 )    
        {    
            perror("child pthread_mutex_lock");    
        }    
        sleep(10);//测试是否能够阻止父进程的写入    
        write(fd, str1, sizeof(str1));    
        ret=pthread_mutex_unlock(p_mutex);      
        if( ret!=0 )    
        {    
            perror("child pthread_mutex_unlock");    
        }       
    }    
    else    
    {    
        sleep(2);//保证子进程先执行     
        ret=pthread_mutex_lock(p_mutex);    
        if( ret!=0 )    
        {    
            perror("father pthread_mutex_lock");    
        }    
        write(fd, str2, sizeof(str2));    
        ret=pthread_mutex_unlock(p_mutex);      
        if( ret!=0 )    
        {    
            perror("father pthread_mutex_unlock");    
        }                   
    }    
    //wait(NULL);    
    munmap(p_mutex, sizeof(pthread_mutex_t)); 
	return 0;
}       
