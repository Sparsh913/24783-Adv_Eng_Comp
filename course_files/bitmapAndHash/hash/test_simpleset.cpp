#include <iostream>
#include "simpleset.h"



int main(void)
{
	std::cout << "Start Test." << std::endl;

	SimpleSet<int> set;
	for(int i=0; i<10; ++i)
	{
		set.insert(i);
	}

	for(int i=0; i<10; ++i)
	{
		std::cout << set.is_included(i);
	}
	std::cout << "\n";

	for(int i=0; i<10; i+=2)
	{
		set.erase(i);
	}

	for(int i=0; i<10; ++i)
	{
		std::cout << set.is_included(i);
	}
	std::cout << "\n";


	set.resize(29);
	for(int i=0; i<10; ++i)
	{
		std::cout << set.is_included(i);
	}
	std::cout << "\n";


	set.resize(8);
	for(int i=0; i<10; ++i)
	{
		std::cout << set.is_included(i);
	}
	std::cout << "\n";


	std::cout << "End Test." << std::endl;

	return 0;
}
