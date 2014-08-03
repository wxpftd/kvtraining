#include <iostream>  
#include <sstream>  
#include <string>  
#include <boost/archive/text_iarchive.hpp>  
#include <boost/archive/text_oarchive.hpp>  
#include <vector>  
#include <boost/serialization/vector.hpp>  
#include <fstream>  
using namespace std;  
using namespace boost::serialization;  
using namespace boost::archive;  

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

stringstream ss;

void save()
{
	boost::archive::text_oarchive oa(ss);
	People p(10);
	oa << p;
}

void load()
{
	boost::archive::text_iarchive ia(ss);
	People p;
	ia >> p;
	cout << p.getAge() << endl; 
}
int main()  
{  
	save();
	load();
	return 0;
}       
