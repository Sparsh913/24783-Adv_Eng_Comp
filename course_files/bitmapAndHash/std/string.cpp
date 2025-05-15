#include <string>
#include <unordered_set>
#include <iostream>

int main(void)
{
	std::unordered_set<std::string> set;
	set.insert("Carnegie");
	set.insert("Mellon");
	set.insert("University");
	set.insert("Mechanical");
	set.insert("Engineering");

	for(auto s : set)
	{
		std::cout << s << "\n";
	}

	std::cout << (set.end()!=set.find("Mellon")) << "\n";
	std::cout << (set.end()!=set.find("Pitt")) << "\n";
	return 0;
}
