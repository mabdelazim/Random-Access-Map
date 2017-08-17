#ifndef __RECENT_LIST_HEADER__
#define __RECENT_LIST_HEADER__
#include "RandomAccessMap.h"
#include <unordered_map>
#include <stdexcept>
#include <limits>


template<typename Item_t>
class RecentList
{
public:
	RecentList(std::size_t limit = std::numeric_limits<std::size_t>::max())
		: _lastIndex(0), _limit(limit)
	{
	}

	void add(const Item_t& item)
	{
		Hash_t::iterator pos = _hash.find(item);
		if (pos != _hash.end())
		{
			_indexMap.remove(pos->second);
		}
		else if (size() == _limit)
		{
			Hash_t::iterator pos = _indexMap.begin()->second;
			_indexMap.remove(pos->second);
			_hash.erase(pos);
		}
		++_lastIndex;
		pos = _hash.insert(Hash_t::value_type(item, _lastIndex)).first;
		_indexMap[_lastIndex] = pos;
		

	}

	void remove(const Item_t& item)
	{
		Hash_t::iterator pos = _hash.find(item);
		if (pos != _hash.end())
		{
			_indexMap.remove(pos->second);
			_hash.erase(pos);
		}
	}

	bool has(const Item_t& item) const
	{
		return _hash.find(item) != _hash.end();
	}

	const Item_t& operator[](const std::size_t& index) const
	{
		if (index >= size())
		{
			throw std::invalid_argument("Invalided Index");
		}

		std::size_t map_index = size() - index -1;
		return _indexMap.data(map_index)->first;
	}

	
	std::size_t size() const
	{
		return _hash.size();
	}

	void clear()
	{
		_hash.clear();
		_indexMap.clear();
	}

protected:

	typedef  std::unordered_map<Item_t, std::size_t> Hash_t;
	Hash_t _hash;
	RandomAccessMap<std::size_t, typename Hash_t::iterator> _indexMap;
	std::size_t _lastIndex;
	std::size_t _limit;
    


};

#endif
