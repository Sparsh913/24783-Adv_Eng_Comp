#include <string>
#include <set>
#include <iostream>

int main(void)
{
	std::set <std::string> set;
	set.insert("Carnegie");
	set.insert("Mellon");
	set.insert("University");
	set.insert("Mechanical");
	set.insert("Engineering");
	set.insert("Department");

	for(auto s : set)
	{
		std::cout << s << "\n";
	}

	std::cout << (set.find("Mellon")!=set.end()) << "\n";
	std::cout << (set.find("XYZ")!=set.end()) << "\n";

	return 0;
}
