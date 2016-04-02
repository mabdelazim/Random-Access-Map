#include "RecentList.h"
#include "Test.h"
#include <cassert>
#include <string>

void Test_RecentList_Add()
{
	RecentList<std::string> my_list;
	my_list.add("A");
	my_list.add("B");
	my_list.add("C");
	assert(my_list.size() == 3);
	assert(my_list[0] == "C");
	assert(my_list[1] == "B");
	assert(my_list[2] == "A");

}


void Test_RecentList_Limit()
{
	RecentList<std::string> my_list(2);
	my_list.add("A");
	my_list.add("B");
	my_list.add("C");
	assert(my_list.size() == 2);
	assert(my_list[0] == "C");
	assert(my_list[1] == "B");

}

void Test_RecentList_Remove()
{
	RecentList<std::string> my_list;
	my_list.add("A");
	my_list.add("B");
	my_list.add("C");
	my_list.add("D");
	

	// remove last one
	my_list.remove("D");
	assert(my_list.size() == 3);
	assert(my_list[0] == "C");
	assert(my_list[1] == "B");
	assert(my_list[2] == "A");

	// remove middle one
	my_list.remove("B");
	assert(my_list.size() == 2);
	assert(my_list[0] == "C");
	assert(my_list[1] == "A");

	// remove first one
	my_list.remove("A");
	assert(my_list.size() == 1);
	assert(my_list[0] == "C");

}

void Test_RecentList_Order()
{
	RecentList<std::string> my_list;
	my_list.add("A");
	my_list.add("B");
	my_list.add("C");
	my_list.add("D");

	my_list.add("B");
	assert(my_list.size() == 4);
	assert(my_list[0] == "B");
	assert(my_list[1] == "D");
	assert(my_list[2] == "C");
	assert(my_list[3] == "A");

}

void Test_RecentListClear()
{
	RecentList<std::string> my_list;
	my_list.add("A");
	my_list.add("B");
	my_list.add("C");
	my_list.add("D");
	my_list.clear();
	assert(my_list.size() == 0);

	my_list.add("X");
	assert(my_list.size() == 1);
	assert(my_list[0] == "X");




}

void Test_RecentList()
{
	Test_RecentList_Add();
	Test_RecentList_Limit();
	Test_RecentList_Remove();
	Test_RecentList_Order();
	Test_RecentListClear();
}