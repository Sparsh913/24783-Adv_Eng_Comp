#ifndef SIMPLESET_IS_INCLUDED
#define SIMPLESET_IS_INCLUDED
/* { */


#include <algorithm>
#include <vector>

template <class IntClass>
class SimpleSet
{
public:
	enum
	{
		MINIMUM_HASH_SIZE=7
	};
private:
	std::vector <std::vector <IntClass> > table;
public:
	SimpleSet()
	{
		// table[?]
		table.resize(MINIMUM_HASH_SIZE);
	}
	~SimpleSet()
	{
	}
	void clear(void)
	{
		table.resize(MINIMUM_HASH_SIZE);
		for(auto &row : table)
		{
			row.clear();
		}
	}
	void insert(const IntClass &incoming)
	{
		if(true!=is_included(incoming))
		{
			int r=incoming%table.size();
			if(r<0)
			{
				r=-r;
			}
			table[r].push_back(incoming);
		}
	}
	void erase(const IntClass &incoming)
	{
		int r=incoming%table.size();
		if(r<0)
		{
			r=-r;
		}

		for(auto &x : table[r])
		{
			if(x==incoming)
			{
				std::swap(x,table[r].back());
				table[r].pop_back();
				return;
			}
		}
	}
	bool is_included(const IntClass &incoming)
	{
		int r=incoming%table.size();
		if(r<0)
		{
			r=-r;
		}
		for(auto tst : table[r])
		{
			if(tst==incoming)
			{
				return true;
			}
		}
		return false;
	}

	void resize(int newSize)
	{
		newSize=std::max<int>(MINIMUM_HASH_SIZE,newSize);

		/* Simple method
		std::vector <std::vector <IntClass> > newTable;
		newTable.resize(newSize);

		for(auto &row : table)
		{
			for(auto elem : row)
			{
				auto newRow=elem%newSize;
				if(newRow<0)
				{
					newRow=-newRow;
				}
				newTable[newRow].push_back(elem);
			}
		}

		std::swap(newTable,table); */

		if(table.size()<newSize)
		{
			table.resize(newSize);
		}


		for(int r=0; r<table.size(); ++r)
		{
			for(int c=0; c<table[r].size(); )
			{
				auto elem=table[r][c];
				auto newRow=elem%newSize;
				if(newRow!=r)
				{
					auto toMove=table[r][c];
					std::swap(table[r][c],table[r].back());
					table[r].pop_back();
					table[newRow].push_back(toMove);
				}
				else
				{
					c++;
				}
			}
		}


		table.resize(newSize);
	}


	// How can we implement find() and end()?
};



/* } */
#endif
