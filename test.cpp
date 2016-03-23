#include "IndexedMap.h"
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <vector>
void Test_IndexedMapInsertionAndOperator()
{
	IndexedMap <std::string, char> my_map;
	my_map.insert("A", 'A');
	my_map.insert("C", 'C');
	my_map.insert("B", 'B');


	assert(my_map.size() == 3);
	
	assert(my_map["A"] == 'A');
	assert(my_map["B"] == 'B');
	assert(my_map["C"] == 'C');

}

void Test_IndexedMapIndex()
{
	IndexedMap <std::string, char> my_map;
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

void Test_IndexedMapRemove()
{
	IndexedMap <std::string, char> my_map;
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

void Test_IndexedMapRemoveRand()
{
	IndexedMap <std::string, std::size_t> my_map;
	
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
	typedef IndexedMap<std::string, char> Map_t;
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

void main()
{
	Test_IndexedMapInsertionAndOperator();
	Test_IndexedMapIndex();
	Test_IndexedMapRemove();
	Test_IndexedMapRemoveRand();
	Test_IndexCopyContainer();

}