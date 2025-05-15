#ifndef YSHASH_IS_INCLUDED
#define YSHASH_IS_INCLUDED
/* { */

#include <algorithm>
#include <vector>
#include <iostream>

// Template for hash functions.
template <class KeyType>
struct ysHash
{
	std::size_t operator()(const KeyType &key) const;
};


template <class KeyType>
class ysHashBase
{
protected:
	enum
	{
		MINIMUM_HASH_SIZE=7
	};

	template <class Entry>
	bool is_included(const KeyType &key,const std::vector <std::vector <Entry> > &table)
	{
		ysHash<KeyType> func;
		auto code=func(key);
		auto row=code%table.size();
		for(auto &e : table[row])
		{
			if(code==e.code && key==e.first)
			{
				return true;
			}
		}
		return false;
	}
};


template <class KeyType>
class ysHashSet : public ysHashBase <KeyType>
{
private:
	class Entry
	{
	public:
		std::size_t code;
		KeyType first;
	};
	std::vector <std::vector <Entry> > table;

public:
	friend class iterator;
	class iterator
	{
	public:
		ysHashSet<KeyType> *owner;
		std::size_t row,column;
		bool operator==(const iterator &incoming) const
		{
			return row==incoming.row && column==incoming.column;
		}
		bool operator!=(const iterator &incoming) const
		{
			return row!=incoming.row || column!=incoming.column;
		}

        iterator operator++()  // Pre-incrementation  ++iter
        {
			auto next=owner->move_to_next(*this);
			row=next.row;
			column=next.column;
			return next;
		}
        iterator operator++(int) // Post-incrementation  iter++
        {
			auto current=*this;
			auto next=owner->move_to_next(*this);
			row=next.row;
			column=next.column;
			return current;
		}
        KeyType &operator*()    // De-referencing
        {
			return owner->table[row][column].first;
		}
        KeyType *operator->()   // De-referencing
        {
			return &owner->table[row][column].first;
		}
	};

	ysHashSet()
	{
		table.resize(MINIMUM_HASH_SIZE);
	}
	void insert(const KeyType &key)
	{
		if(true!=is_included(key)) // end()==find(key)
		{
			ysHash<KeyType> func;
			auto code=func(key);
			auto row=code%table.size();

			Entry e;
			e.code=code;
			e.first=key;
			table[row].push_back(e);
		}
	}
	bool is_included(const KeyType &key)
	{
		return ysHashBase <KeyType>::is_included<Entry>(key,table);
	}
	void erase(const KeyType &key);
	void erase(iterator iter);
	iterator find(const KeyType &key) const;

	iterator begin(void)
	{
		iterator i;
		i.owner=this;
		i.column=0;
		for(i.row=0; i.row<table.size(); ++i.row)
		{
			if(true!=table[i.row].empty())
			{
				return i;
			}
		}
		return end();
	}

	iterator move_to_next(iterator iter)
	{
		++iter.column;
		if(iter.column<table[iter.row].size())
		{
			return iter;
		}

		++iter.row;
		iter.column=0;
		while(iter.row<table.size())
		{
			if(0<table[iter.row].size())
			{
				return iter;
			}
			++iter.row;
		}

		return end();
	}

	// Standard Template Library very often uses end() as NULL.
	iterator end(void)
	{
		iterator i;
		i.owner=this;
		i.row=~0;
		i.column=~0;
		return i;
	}

	void resize(std::size_t nRows)
	{
		nRows=std::max<std::size_t>(nRows,MINIMUM_HASH_SIZE);

		std::vector <std::vector <Entry> > newTable;
		newTable.resize(nRows);

		for(auto &row : table)
		{
			for(auto &clm : row)
			{
				auto newRow=clm.code%nRows;
				Entry newEnt;
				newTable[newRow].push_back(newEnt);
				newTable[newRow].back().code=clm.code;
				std::swap(newTable[newRow].back().first,clm.first);
			}
		}

		std::swap(table,newTable);
	}

	// Memo to myself: autoResize should be implemented in the assignment.
	//                 Don't write during the lecture.
	void autoResize(void)
	{
		// Part of the next assignment.
	}
	// Hint:  If you increase size when the length exceeds table_size*4, and
	//        reduce size when the length is below table_size/4, 
	//        it could increase/decrease table size too often when
	//        keys are inserted and deleted frequently.
	//        There should be a buffer.


	void Print(void) const
	{
		for(auto &row : table)
		{
			for(auto &clm : row)
			{
				std::cout << clm.first << "|";
			}
			std::cout << "\n";
		}
	}
};


template <class KeyType,class ValueType>
class ysHashTable : public ysHashBase <KeyType>
{
private:
	class Entry
	{
	public:
		std::size_t code;
		KeyType first;
		ValueType second;
	};
	std::vector <std::vector <Entry> > table;

public:
	friend class iterator;
	class iterator
	{
	public:
		ysHashTable<KeyType,ValueType> *owner;
		std::size_t row,column;
		bool operator==(const iterator &incoming) const
		{
			return row==incoming.row && column==incoming.column;
		}
		bool operator!=(const iterator &incoming) const
		{
			return row!=incoming.row || column!=incoming.column;
		}

        iterator operator++()  // Pre-incrementation  ++iter
        {
			auto next=owner->move_to_next(*this);
			row=next.row;
			column=next.column;
			return next;
		}
        iterator operator++(int) // Post-incrementation  iter++
        {
			auto current=*this;
			auto next=owner->move_to_next(*this);
			row=next.row;
			column=next.column;
			return current;
		}
        Entry &operator*()    // De-referencing
        {
			return owner->table[row][column];
		}
        Entry *operator->()   // De-referencing
        {
			return &owner->table[row][column];
		}
	};

	ysHashTable()
	{
		table.resize(MINIMUM_HASH_SIZE);
	}
	void insert(const KeyType &key,const ValueType &value)
	{
		ysHash<KeyType> func;
		auto code=func(key);
		auto row=code%table.size();
		for(auto &e : table[row])
		{
			if(code==e.code && key==e.first)
			{
				e.second=value;
				return;
			}
		}

		Entry e;
		table[row].push_back(e);
		table[row].back().code=code;
		table[row].back().first=key;
		table[row].back().second=value;
	}
	bool is_included(const KeyType &key)
	{
		return ysHashBase <KeyType>::is_included<Entry>(key,table);
	}
	void erase(const KeyType &key)
	{
		erase(find(key));
	}
	void erase(iterator iter)
	{
		if(end()!=iter)
		{
			std::swap(table[iter.row][iter.column],table[iter.row].back());
			table[iter.row].pop_back();
		}
	}

	iterator find(const KeyType &key) const
	{
		ysHash<KeyType> func;
		auto code=func(key);
		auto row=code%table.size();

		iterator i;
		i.owner=(ysHashTable <KeyType,ValueType> *)this;
		i.row=row;
		for(i.column=0; i.column<table[i.row].size(); ++i.column)
		{
			if(code==table[i.row][i.column].code &&
			   key==table[i.row][i.column].first)
			{
				return i;
			}
		}

		return end();
	}

	iterator begin(void)
	{
		iterator i;
		i.owner=this;
		i.column=0;
		for(i.row=0; i.row<table.size(); ++i.row)
		{
			if(true!=table[i.row].empty())
			{
				return i;
			}
		}
		return end();
	}

	iterator move_to_next(iterator iter)
	{
		++iter.column;
		if(iter.column<table[iter.row].size())
		{
			return iter;
		}

		++iter.row;
		iter.column=0;
		while(iter.row<table.size())
		{
			if(0<table[iter.row].size())
			{
				return iter;
			}
			++iter.row;
		}

		return end();
	}

	// Standard Template Library very often uses end() as NULL.
	iterator end(void) const
	{
		iterator i;
		i.owner=(ysHashTable <KeyType,ValueType> *)this;
		i.row=~0;
		i.column=~0;
		return i;
	}

	void resize(std::size_t nRows)
	{
		nRows=std::max<std::size_t>(nRows,MINIMUM_HASH_SIZE);

		std::vector <std::vector <Entry> > newTable;
		newTable.resize(nRows);

		for(auto &row : table)
		{
			for(auto &clm : row)
			{
				auto newRow=clm.code%nRows;
				Entry newEnt;
				newTable[newRow].push_back(newEnt);
				newTable[newRow].back().code=clm.code;
				std::swap(newTable[newRow].back().first,clm.first);
				std::swap(newTable[newRow].back().second,clm.second);
			}
		}

		std::swap(table,newTable);
	}

	// Memo to myself: autoResize should be implemented in the assignment.
	//                 Don't write during the lecture.
	void autoResize(void)
	{
		// Part of the next assignment.
	}
	// Hint:  If you increase size when the length exceeds table_size*4, and
	//        reduce size when the length is below table_size/4, 
	//        it could increase/decrease table size too often when
	//        keys are inserted and deleted frequently.
	//        There should be a buffer.


	void Print(void) const
	{
		for(auto &row : table)
		{
			for(auto &clm : row)
			{
				std::cout << clm.first << "|";
			}
			std::cout << "\n";
		}
	}
};



/* } */
#endif
