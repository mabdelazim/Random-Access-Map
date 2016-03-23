#include "IndexedMap.h"
#include <cassert>
#include <cstdlib>
#include <sstream>
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


/*

void Test_IndexMapIterator()
{
	cout << " Testing Itertor\n";
	IndexedMap my_map;
	//my_map.insert(10);
	//my_map.insert(7);
	//my_map.insert(12);
	for (std::size_t i = 0; i < 100; i++)
	{
		my_map.insert(rand() % 10000, "MSM");
	}
	for (auto it : my_map)
	{
		cout << it.first << ", " << it.second << std::endl;
	}
	//my_map.printAll();

}

void Test_IndexMapSubscript()
{
	cout << " Testing Itertor\n";
	IndexedMap my_map;
	//my_map.insert(10);
	//my_map.insert(7);
	//my_map.insert(12);
	for (std::size_t i = 0; i < 100; i++)
	{
		std::stringstream ss;
		int v = rand() % 10000;
		ss << "[MSM " << v << " SS]";
		my_map.insert(v, ss.str());
	}
	for (auto it : my_map)
	{
		cout << it.first << ", " << my_map[it.first] << std::endl;
	}
	//my_map.printAll();

}

void Test_IndexSetIterator()
{
	cout << " Testing Set Itertor\n";
	IndexedSet my_set;
	//my_map.insert(10);
	//my_map.insert(7);
	//my_map.insert(12);
	for (std::size_t i = 0; i < 100; i++)
	{
		my_set.insert(rand() % 10000);
	}

	for (auto it : my_set)
	{
		cout << it << std::endl;
	}
	//my_map.printAll();

}



void Test_IndexSet()
{
	IndexedSet my_set;
	//my_map.insert(10);
	//my_map.insert(7);
	//my_map.insert(12);
	for (std::size_t i = 0; i < 10; i++)
	{
		my_set.insert(rand() % 10000);
	}
	for (std::size_t i = 0; i != my_set.size(); ++i)
	{
		cout << my_set[i] << std::endl;
	}
	//my_map.printAll();

}
#include <map>
*/
void main()
{
	Test_IndexedMapInsertionAndOperator();
	Test_IndexedMapIndex();
	Test_IndexedMapRemove();
	Test_IndexedMapRemoveRand();
	/*Test_IndexMap();
	cout << "Test Set\n";
	Test_IndexSet();
	Test_IndexMapIterator();
	Test_IndexSetIterator();
	Test_IndexMapSubscript();
	cout << "Finisned\n";
	std::map<int, int> m;
	m[0] = 1;
	m.begin()->first;
	while (true);*/
}