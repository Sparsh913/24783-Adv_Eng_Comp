#include <string>
#include <iostream>
#include <stdlib.h>

int main(void)
{
	while(true!=std::cin.eof())
	{
		std::string s;
		std::getline(std::cin,s);
		std::cout << s << "\n";

		if(34<=s.size())
		{
			std::string newFn;
			newFn=s.c_str()+33;
			for(auto &c : newFn)
			{
				if('-'==c)
				{
					c='_';
				}
			}
			std::cout << newFn <<"\n";

			rename(s.c_str(),newFn.c_str());
		}
	}
	return 0;
}
