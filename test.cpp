#include "RandomAccessMap.h"
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <vector>
void Test_RandomAccessMapInsertionAndOperator()
{
	RandomAccessMap <std::string, char> my_map;
	my_map.insert("A", 'A');
	my_map.insert("C", 'C');
	my_map.insert("B", 'B');


	assert(my_map.size() == 3);
	
	assert(my_map["A"] == 'A');
	assert(my_map["B"] == 'B');
	assert(my_map["C"] == 'C');

}

void Test_RandomAccessMapIndex()
{
	RandomAccessMap <std::string, char> my_map;
	my_map.insert("A", 'A');
	my_map.insert("C", 'C');
	my_map.insert("B", 'B');

	assert(my_map.size() == 3);

	assert(my_map.key(0) == "A");
	assert(my_map.data(0) == 'A');
	assert(my_map.key(1) == "B");
	assert(my_map.data(1) == 'B');
	assert(my_map.key(2) == "C");
	assert(my_map.data(2) == 'C');
}

void Test_RandomAccessMapRemove()
{
	RandomAccessMap <std::string, char> my_map;
	my_map.insert("A", 'A');
	my_map.insert("C", 'C');
	my_map.insert("B", 'B');

	my_map.remove("B");
	assert(my_map.size() == 2);

	assert(my_map.key(0) == "A");
	assert(my_map.data(0) == 'A');
	assert(my_map.key(1) == "C");
	assert(my_map.data(1) == 'C');
}

void Test_RandomAccessMapRemoveRand()
{
	RandomAccessMap <std::string, std::size_t> my_map;
	
	my_map.insert("A", 'A');

	for (std::size_t i = 0; i < 1000; i++)
	{
		std::stringstream ss;
		ss << "B " << i;
		my_map.insert(ss.str(), i);
	}

	my_map.insert("C", 'C');

	for (std::size_t i = 0; i < 100; ++i)
	{
		std::size_t index = rand() % 100  +100;
		my_map.remove(my_map.key(index));
	}


	assert(my_map.key(0) == "A");
	assert(my_map.data(0) == 'A');

	assert(my_map.key(my_map.size() -1 )== "C");
	assert(my_map.data(my_map.size() - 1) == 'C');


	assert(my_map.size() == (1000 + 1 - 100 + 1));
}


void Test_IndexCopyContainer()
{
	typedef RandomAccessMap<std::string, char> Map_t;
	typedef std::vector<Map_t::Value_t> Vec_t;
	Vec_t vec;
	vec.push_back(Map_t::Value_t( "A", 'A'));
	vec.push_back(Map_t::Value_t("B", 'B'));
	vec.push_back(Map_t::Value_t("C", 'C'));

	Map_t my_map(vec.begin(), vec.end());
	assert(my_map.size() == 3);

	assert(my_map.key(0) == "A");
	assert(my_map.data(0) == 'A');
	assert(my_map.key(1) == "B");
	assert(my_map.data(1) == 'B');
	assert(my_map.key(2) == "C");
	assert(my_map.data(2) == 'C');

	
}

#include <unordered_map>
void Test_IteratorValidity()
{
	typedef RandomAccessMap<int, int> Map_t;
	Map_t my_map;
	std::unordered_map<int, Map_t::Iterator> iterators_list;
	for (std::size_t i = 0; i < 1000; i++)
	{
		int key = rand();
		if (!my_map.find(key))
		{
			Map_t::Iterator it = my_map.insert(key, key | 1);
			iterators_list[key] = it;
		}
	}

	for (std::size_t i = 0; i < 100; i++)
	{
		int index = rand() % my_map.size();
		int key = my_map.key(index);
		my_map.remove(key);
		iterators_list.erase(key);
	}

	for (std::unordered_map<int, Map_t::Iterator>::iterator it = iterators_list.begin(); it != iterators_list.end(); ++it)
	{
		assert(it->first == it->second->first);
		assert((it->first | 1) == it->second->second);

	}

}

void main()
{
	Test_RandomAccessMapInsertionAndOperator();
	Test_RandomAccessMapIndex();
	Test_RandomAccessMapRemove();
	Test_RandomAccessMapRemoveRand();
	Test_IndexCopyContainer();
	Test_IteratorValidity();
}