#include <iostream>
#include <string>

int main(void)
{
	std::string str="018470183071";
	for(auto iter=str.begin(); iter!=str.end(); ++iter)
	{
		std::cout << *iter << "\n";
	}
	for(auto c : str)
	{
		std::cout << c << "\n";
	}
	return 0;
}

