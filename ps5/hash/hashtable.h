#ifndef YSHASH_IS_INCLUDED
#define YSHASH_IS_INCLUDED
/* { */

// Difference from the code done in class:
//   Removed hash set.
//   Made const-correct.
//   Made un-copyable.

#include <algorithm>
#include <vector>
#include <iostream>

// Template for hash functions.
template <class KeyType>
struct ysHash
{
	std::size_t operator()(const KeyType &key) const;
};



template <class KeyType,class ValueType>
class ysHashTable
{
private:
	ysHashTable(const ysHashTable <KeyType,ValueType> &);
	ysHashTable <KeyType,ValueType> &operator=(const ysHashTable <KeyType,ValueType> &);

	enum
	{
		MINIMUM_HASH_SIZE=7
	};

	class Entry
	{
	public:
		std::size_t code;
		KeyType first;
		ValueType second;
	};
	std::vector <std::vector <Entry> > table;
	std::size_t numElements;

public:
	friend class iterator;
	friend class const_iterator;

	template <class OwnerType,class ColumnType>
	class iterator_base
	{
	public:
		OwnerType *owner;
		std::size_t row,column;
		bool operator==(const iterator_base<OwnerType,ColumnType> &incoming) const
		{
			return row==incoming.row && column==incoming.column;
		}
		bool operator!=(const iterator_base<OwnerType,ColumnType> &incoming) const
		{
			return row!=incoming.row || column!=incoming.column;
		}

        iterator_base <OwnerType,ColumnType> operator++()  // Pre-incrementation  ++iter
        {
			auto next=owner->move_to_next(*this);
			row=next.row;
			column=next.column;
			return next;
		}
        iterator_base operator++(int) // Post-incrementation  iter++
        {
			auto current=*this;
			auto next=owner->move_to_next(*this);
			row=next.row;
			column=next.column;
			return current;
		}
        ColumnType &operator*()    // De-referencing
        {
			return owner->table[row][column];
		}
        ColumnType *operator->()   // De-referencing
        {
			return &owner->table[row][column];
		}
	};
	typedef iterator_base<ysHashTable<KeyType,ValueType>,Entry> iterator;
	typedef iterator_base<const ysHashTable<KeyType,ValueType>,const Entry	> const_iterator;

	ysHashTable()
	{
		table.resize(MINIMUM_HASH_SIZE);
		numElements=0;
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

		++numElements;
		autoResize();
	}
	bool is_included(const KeyType &key)
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
			--numElements;
			autoResize();
		}
	}

	std::size_t GetNumElements() const
	{
		return numElements;
	}



	template <class iterator_class,class OwnerType>
	iterator_class find_base(const KeyType &key,OwnerType *thisPtr) const
	{
		ysHash<KeyType> func;
		auto code=func(key);
		auto row=code%table.size();

		iterator_class i;
		i.owner=thisPtr;
		i.row=row;
		for(i.column=0; i.column<table[i.row].size(); ++i.column)
		{
			if(code==table[i.row][i.column].code &&
			   key==table[i.row][i.column].first)
			{
				return i;
			}
		}

		return thisPtr->end();
	}
	iterator find(const KeyType &key)
	{
		return find_base<iterator,ysHashTable<KeyType,ValueType> >(key,this);
	}
	const_iterator find(const KeyType &key) const
	{
		return find_base<const_iterator,const ysHashTable<KeyType,ValueType> >(key,this);
	}


	template <class iterator_class,class OwnerClass>
	iterator_class begin_base(OwnerClass *thisPtr) const
	{
		iterator_class i;
		i.owner=thisPtr;
		i.column=0;
		for(i.row=0; i.row<table.size(); ++i.row)
		{
			if(true!=table[i.row].empty())
			{
				return i;
			}
		}
		return thisPtr->end();
	}
	iterator begin(void)
	{
		return begin_base<iterator,ysHashTable<KeyType,ValueType> >(this);
	}
	const_iterator begin(void) const
	{
		return begin_base<const_iterator,const ysHashTable<KeyType,ValueType> >(this);
	}


	template <class iterator_class,class OwnerClass>
	iterator_class move_to_next_base(iterator_class iter,OwnerClass *thisPtr) const
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

		return thisPtr->end();
	}
	iterator move_to_next(iterator iter)
	{
		return move_to_next_base<iterator,ysHashTable<KeyType,ValueType> >(iter,this);
	}
	const_iterator move_to_next(const_iterator iter) const
	{
		return move_to_next_base<const_iterator,const ysHashTable<KeyType,ValueType> >(iter,this);
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
	const_iterator end(void) const
	{
		const_iterator i;
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
		// 5.2
		// Complete AutoResize Function in the HashTable Class
		// AutoResize function in the HashTable class is left empty. Fill the function. Resize function is already filled,
		// and you can use the function.
		// AutoResize is called at the end of insert and erase. The idea is to keep the hash size comparable to the
		// number of elements in the table, which means you need to keep track of the number of elements in the table.
		// Add appropriate member variables and lines in other places in the class.

		double loadFactor=static_cast<double>(GetNumElements())/table.size();
		if(loadFactor>1.5)
		{
			resize(table.size()*2);
		}
		else if(loadFactor < 0.25 && table.size()>MINIMUM_HASH_SIZE)
		{
			resize(std::max(static_cast<std::size_t>(MINIMUM_HASH_SIZE), table.size()/2));
		}
	}
};



/* } */
#endif
