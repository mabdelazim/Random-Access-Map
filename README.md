# IndexedMap
C++ template class for indexed map.
This likes the std::map but you can access items by index to llike my_map[0] and my_map[1]
This is useful to build system like caching where recent used data is highest index 

# Usage
We need just the file "IndexedMap.h" to use the map in your project.
Example :
IndexedMap<std::string, int> my_map;
my_map.insert("A" , 0);
std::cout<<" First index is key: "<<may_map.key(0) << " data: "<<my_map.data(0);
my_map.remove("A");


# Performance
All operations work in O(log(n)) as n is the number of items in the map

# Implementation
It is an AVL standarad tree, plus addin the cound of childs in each nodes.

#Testing
file test.cpp contains some test cases.

