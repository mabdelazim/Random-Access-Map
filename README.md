# RandomAccessMap
C++ template class for random access map
This likes the std::map but you can access items random by indexe with syntax my_map.key(i) and my_map.data(i)
This is useful to build system like caching where recent used data is highest index 



# Usage
We need just the file "RandomAccessMap.h" to use the map in your project.
Example :
RandomAccessMap<std::string, int> my_map;
my_map.insert("A" , 0);
std::cout<<" First index is key: "<<may_map.key(0) << " data: "<<my_map.data(0);
my_map.remove("A");


# Performance
All operations work in O(log(n)) as n is the number of items in the map

# Implementation
It is an AVL standarad tree, plus adding the count of childs in each nodes.

#Testing
file test.cpp contains some test cases.


# RecentList

RecentList is one of usage example of RandomAccessMap. Which can be used to repesent a cache system.

It is a template class provides last in first out system. It is useful for caching system
When new item is added if it doesn't exist it pushed to the beging of the list
If it exists, it is moved to begin of the list.
This class can used to present a cache system.

Example 
 RecentList<std::string> my_list;
 my_list.add("A");
 my_list.add("B");
 my_list.add("A");
 std::cout<< my_list[0]; // show "A"
