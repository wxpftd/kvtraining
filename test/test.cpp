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
private:
	friend class boost::serialization::access;
	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)	
	{
		ar & age_;	
	}
	int age_;
};
int main()  
{  
	return 0;
}       
