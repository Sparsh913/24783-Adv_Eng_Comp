#include <iostream>
#include <unordered_set>
#include <stdlib.h>
#include <time.h>


int main(void)
{
	unsigned int i=0,r[40];
	std::unordered_set <unsigned int> used;

	srand(time(nullptr));
	while(i<40)
	{
		auto x=rand()%40;
		if(used.find(x)==used.end())
		{
			r[i]=x;
			++i;
			used.insert(x);
		}
	}

	for(auto x : r)
	{
		std::cout << x << "\n";
	}
	return 0;
}
