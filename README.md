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

