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

int main(void)
{
	ysHashSet<std::string> set;

	set.insert("Carnegie");
	set.insert("Mellon");
	set.insert("University");
	set.insert("Mechanical");
	set.insert("Engineering");
	set.insert("you");
	set.insert("can");
	set.insert("keep");
	set.insert("each");
	set.insert("row");
	set.insert("sorted");
	set.insert("by");
	set.insert("the");
	set.insert("hash");
	set.insert("code");
	set.insert("You");
	set.insert("will");
	set.insert("get");
	set.insert("faster");
	set.insert("look");
	set.insert("up");
	set.insert("but");
	set.insert("slower");
	set.insert("insertion");
	set.insert("deletion");

	std::cout << (set.is_included("Mellon")) << "\n";
	std::cout << (set.is_included("Mechanical")) << "\n";
	std::cout << (set.is_included("lacinahceM")) << "\n";
	std::cout << (set.is_included("Pitt")) << "\n";
	std::cout << (set.is_included("")) << "\n";

	ysHash <std::string> func;
	std::cout << func("Mechanical") << "\n";
	std::cout << func("lacinahceM") << "\n";

	std::cout << "----\n";
	for(auto iter=set.begin(); iter!=set.end(); ++iter)
	{
		std::cout << *iter << "\n";
	}

	set.resize(17);

	std::cout << "----\n";
	for(auto &x : set)
	{
		std::cout << x << "\n";
	}

	std::cout << "----\n";
	set.Print();

	return 0;
}
