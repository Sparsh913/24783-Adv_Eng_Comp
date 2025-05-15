#include <string>
#include <iostream>
#include "yshash.h"



template <>
struct ysHash<std::string>
{
	std::size_t operator()(const std::string &key) const
	{
		static unsigned primeNum[5]=
		{
			3,5,7,11,13
		};
		int k=0;
		std::size_t sum=0;
		for(auto c : key)
		{
			sum+=(c*primeNum[k%5]);
			++k;
		}

		// Another example
		// for(auto c : key)
		// {
		// 	size_t x=c;
		// 	x<<=(k%32);
		// 	sum^=x;
		// 	++k;
		// }

		return sum;
	}
};

bool Check(ysHashTable<std::string,int> &set,std::string from,int to)
{
	auto found=set.find(from);
	if(set.end()==found)
	{
		return false;
	}
	if(found->second!=to)
	{
		return false;
	}
	if(found->first!=from)
	{
		return false;
	}
	return true;
}

int main(void)
{
	ysHashTable<std::string,int> set;

std::cout << "0\n";

	set.insert("Carnegie",0);
	set.insert("Mellon",1);
	set.insert("University",2);
	set.insert("Mechanical",3);
	set.insert("Engineering",4);
	set.insert("you",5);
	set.insert("can",6);
	set.insert("keep",7);
	set.insert("each",8);
	set.insert("row",9);
	set.insert("sorted",10);
	set.insert("by",11);
	set.insert("the",12);
	set.insert("hash",13);
	set.insert("code",14);
	set.insert("You",15);
	set.insert("will",16);
	set.insert("get",17);
	set.insert("faster",18);
	set.insert("look",19);
	set.insert("up",20);
	set.insert("but",21);
	set.insert("slower",22);
	set.insert("insertion",23);
	set.insert("deletion",24);

	for(auto &e : set)
	{
		std::cout << e.first << " -> " << e.second << "\n";
	}

	std::cout << "----\n";

	if(true!=Check(set,"Carnegie",0) ||
	   true!=Check(set,"Mellon",1)||
	   true!=Check(set,"University",2)||
	   true!=Check(set,"Mechanical",3))
	{
		return 1;
	}

	auto found=set.find("Mechanical");
	std::cout << found->first << " -> " << found->second << "\n";

	return 0;
}
