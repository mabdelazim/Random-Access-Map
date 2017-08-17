#ifndef __RANDOM_ACCESS_HEADER__
#define __RANDOM_ACCESS_HEADER__
#include <algorithm>
#include <stdexcept>

template<typename Key_t, typename Data_t>

#define LEFT 0
#define RIGHT 1

class RandomAccessMap
{

public:
	typedef std::pair<Key_t, Data_t> Value_t;
	class Iterator;
protected:

	typedef char Balance_t;


public:

	RandomAccessMap() 
	: _root(NULL)
	{
	}

	template<typename InputIterator>
	RandomAccessMap(InputIterator first, InputIterator last)
	 :_root(NULL)
	{
		for (InputIterator it = first; it != last; ++it)
		{
			insert(it->first, it->second);
		}
	}

	Iterator insert(const Key_t& key, const Data_t& data)
	{
		bool isChanged = false;
		_root = insert(_root, key, data, isChanged);
		_root->setRoot();
		return find(_root, key);
	}

	void remove(const Key_t& key)
	{
		if (_root)
		{
			_root = removeNode(_root, key);
			if (_root)
			{
				_root->setRoot();
			}
		}
	}

	bool isValid()
	{
		Key_t temp1, temp2;
		return isValid(_root, temp1, temp2);
	}

	bool has_key(const Key_t& key)
	{
		return find(_root, key) != NULL;
	}
	
	Iterator find(const Key_t& key)
	{
	  Node* n = find(_root, key);
	  return Iterator(n);
        }
    
	void clear()
	{
		freeNode(_root);
		_root = NULL;
	}

	Key_t key(std::size_t index)
	{
		Node* n = get(_root, index);
		if (n)
		{
			return n->key();
		}
		else
		{
			throw std::invalid_argument("Invalided Index");
		}
	}

	Data_t& data(std::size_t index)
	{
		Node* n = get(_root, index);
		if (n)
		{
			return n->data();
		}
		else
		{
			throw std::invalid_argument("Invalided Index");
		}
	}

	const Data_t& data(std::size_t index) const
	{
		Node* n = get(_root, index);
		if (n)
		{
			return n->data();
		}
		else
		{
			throw std::invalid_argument("Invalided Index");
		}
	}

	Data_t& operator [](const Key_t& key)
	{
		Node* n = find(_root, key);
		if (!n)
		{
			insert(key, Data_t());
			n = find(_root, key);
		}

		return n->data();
	}

	std::size_t size() const
	{
		if (_root)
		{
			return _root->count();
		}
		else
		{
			return 0;
		}
	}

	~RandomAccessMap()
	{
		freeNode(_root);
	}


	Iterator begin()
	{
		Node* n = _root;
		if (n != NULL)
		{
			while (n->left())
			{
				n = n->left();
			}
		}

		return Iterator(n);
	}

	Iterator end()
	{
		return Iterator();
	}

protected:

	class Node;

public:
	class Iterator
	{
	public:

		Iterator() :_n(NULL)
		{

		}

	protected:
		Iterator(Node* n)
			:_n(n)
		{

		}

	public:
		Iterator& operator ++()
		{
			if (_n->child(RIGHT))
			{
				_n = _n->right();
				while (_n->left())
				{
					_n = _n->left();
				}

			}
			else
			{

				while (_n->parent() && (_n->parent()->right() == _n))
				{
					_n = _n->parent();
				}
				_n = _n->parent();
			}
			return *this;
		}

		Iterator& operator ++(int)
		{
			++(*this);
			return *this;
		}

		const Value_t& operator *() const
		{
			return _n->value();
		}

		const Value_t* operator ->()  const
		{
			return &_n->value();
		}

		bool operator == (const Iterator& other) const
		{
			return _n == other._n;
		}

		bool operator !=(const Iterator& other) const
		{
			return  _n != other._n;
		}

	protected:

		Node* _n;
		friend Iterator RandomAccessMap::begin();
		friend Iterator RandomAccessMap::insert(const Key_t& key, const Data_t& data);
		friend Iterator RandomAccessMap::find(const Key_t& key);
	};

protected:

	class Node
	{
	public:
		

		Node(const Key_t key, const Data_t& data)
			:_value(key, data), _balance(0), _count(1), _parent(NULL)
		{
			_childs[0] = NULL;
			_childs[1] = NULL;
		}

		void swap(Node& other)
		{
			std::swap(_value, other._value);
			std::swap(_balance, other._balance);
			std::swap(_count, other._count);
			if ((this == other._parent) || (&other == this->_parent))
			{
				Node* parentNode = this == other._parent ? this : &other;
				Node* childNode = this == other._parent ? &other : this;
				int dir = childNode->dir();

				int otherDir = dir == LEFT ? RIGHT : LEFT;

				if (parentNode->_parent)
				{
					parentNode->_parent->_childs[parentNode->dir()] = childNode;
				}
				childNode->_parent = parentNode->_parent;


				parentNode->_childs[dir] = childNode->_childs[dir];
				if (childNode->_childs[dir])
				{
					childNode->_childs[dir]->_parent = parentNode;
				}


				childNode->_childs[dir] = parentNode;
				parentNode->_parent = childNode;


				if (childNode->_childs[otherDir])
				{
					childNode->_childs[otherDir]->_parent = parentNode;
				}
				if (parentNode->_childs[otherDir])
				{
					parentNode->_childs[otherDir]->_parent = childNode;
				}


				std::swap(parentNode->_childs[otherDir], childNode->_childs[otherDir]);
				
			}
			else
			{

				if (other._childs[LEFT])
				{
					other._childs[LEFT]-> _parent = this;
				}
				if (other._childs[RIGHT])
				{
					other._childs[RIGHT]->_parent = this;
				}
				if (this->_childs[LEFT])
				{
					this->_childs[LEFT]->_parent = &other;
				}
				if (this->_childs[RIGHT])
				{
					this->_childs[RIGHT]->_parent = &other;
				}
				std::swap(this->_childs[LEFT], other._childs[LEFT]);
				std::swap(this->_childs[RIGHT], other._childs[RIGHT]);
				
				if (this->_parent)
				{	
					this->_parent->_childs[this->dir()] = &other;
				}
				if (other._parent)
				{
					other._parent->_childs[other.dir()] = this;
				}
			
				std::swap(this->_parent, other._parent);
		 }
		}

		void setLeft(Node* n)
		{
			setChild(n, LEFT);
		}

		void setRight(Node* n)
		{
			setChild(n, RIGHT);
		}

		Node* left()
		{
			return _childs[LEFT];
		}

		Node* right()
		{
			return _childs[RIGHT];
		}

		void setChild(Node* n, int dir)
		{
			_childs[dir] = n;
			if (n)
			{
				n->_parent = this;
			}

			updateCount();

		}

		void setData(const Data_t& data)
		{
			_value.second = data;
		}

		Node* child(int dir) const
		{
			return _childs[dir];
		}

		Node* parent() const
		{
			return _parent;
		}

		Balance_t& balance()
		{
			return _balance;
		}

		const Balance_t & balance() const
		{
			return _balance;
		}

		std::size_t count() const
		{
			return _count;
		}

		const Data_t& data() const
		{
			return _value.second;
		}


		Data_t& data()
		{
			return _value.second;
		}

		const Key_t& key() const
		{
			return _value.first;
		}

		void setRoot()
		{
			_parent = NULL;
		}

		Value_t& value()
		{
			return _value;
		}

		const Value_t& value() const
		{
			return _value;
		}

	protected:

		void updateCount()
		{
			_count = 1;
			if (_childs[LEFT])
			{
				_count += _childs[LEFT]->_count;

			}
			if (_childs[RIGHT])
			{
				_count += _childs[RIGHT]->_count;

			}
		}

	protected:
		int dir() const
		{
			if (_parent)
			{
				return  _parent->_childs[LEFT] == this ? LEFT : RIGHT;
			}
			else
			{
				return -1;
			}
		}


	protected:

		Value_t _value;
		Balance_t _balance;
		std::size_t _count;
		Node* _childs[2];
		Node* _parent;
	};

	static Node* insert(Node*n, const Key_t& key, const Data_t& data, bool& isChanged)
	{
		isChanged = false;
		if (n == NULL)
		{
			Node* newNode = new Node(key, data);
			isChanged = true;
			return newNode;
		}
		else if (key > n->key())
		{
			bool newIsChanged;
			n->setRight(insert(n->right(), key, data, newIsChanged));
			if (newIsChanged && n->balance() == 1)
			{
				isChanged = false;
				return rightBalance(n);
			}
			else if (newIsChanged)
			{
				n->balance()++;
				isChanged = n->balance() > 0;
				return n;
			}
			else
			{
				return n;
			}

		}
		else if (key < n->key())
		{
			bool newIsChange;
			n->setLeft(insert(n->left(), key, data, newIsChange));
			if (newIsChange && n->balance() == -1)
			{
				isChanged = false;
				return leftBalance(n);
			}
			else if (newIsChange)
			{
				n->balance()--;
				isChanged = n->balance() < 0;
				return n;
			}
			else
			{
				return n;
			}
		}
		else
		{
			n->setData(data);
			isChanged = false;
			return n;
		}
	}

	static Node* leftBalance(Node *n)
	{
		Node* left = n->left();
		Node* rightLeft = left->right();

		if (left->balance() == -1)
		{
			n->setLeft(rightLeft);
			left->setRight(n);
			n->balance() = 0;
			left->balance() = 0;
			return left;
		}

		else if (left->balance() == 0)
		{
			n->setLeft(rightLeft);
			left->setRight(n);
			n->balance() = -1;
			left->balance() = 1;
			return left;
		}
		else{
			if (rightLeft->balance() == -1)
			{
				n->balance() = 1;
				left->balance() = 0;
				rightLeft->balance() = 0;
			}
			else if (rightLeft->balance() == 0)
			{
				n->balance() = 0;
				left->balance() = 0;
				rightLeft->balance() = 0;
			}
			else
			{
				n->balance() = 0;
				left->balance() = -1;
				rightLeft->balance() = 0;
			}

			n->setLeft(rightLeft->right());
			left->setRight(rightLeft->left());
			rightLeft->setRight(n);
			rightLeft->setLeft(left);
			return rightLeft;
		}
	}

	static Node* rightBalance(Node* n)
	{
		Node* right = n->right();
		Node * leftRight = right->left();

		if (right->balance() == 1)
		{

			n->setRight(leftRight);
			right->setLeft(n);
			// single rotation
			n->balance() = 0;
			right->balance() = 0;

			return right;
		}
		else if (right->balance() == 0)
		{
			n->setRight(leftRight);
			right->setLeft(n);

			n->balance() = 1;
			right->balance() = -1;
			return right;
		}
		else
		{
			if (leftRight->balance() == 1)
			{
				n->balance() = -1;
				right->balance() = 0;
				leftRight->balance() = 0;
			}
			else if (leftRight->balance() == 0)
			{
				n->balance() = 0;
				right->balance() = 0;
				leftRight->balance() = 0;
			}
			else
			{
				n->balance() = 0;
				right->balance() = 1;
				leftRight->balance() = 0;
			}

			n->setRight(leftRight->left());
			right->setLeft(leftRight->right());
			leftRight->setLeft(n);
			leftRight->setRight(right);
			return leftRight;
		}
	}

	static void freeNode(Node* n)
	{
		if (n->left())
		{
			freeNode(n->left());
		}
		if (n->right())
		{
			freeNode(n->right());
		}
		delete n;
	}

	static Node* removeNode(Node* n, const Key_t& key)
	{
		Node* result = n;
		if (key < n->key() && n->left())
		{
			Balance_t oldBalance = n->left()->balance();
			n->setLeft(removeNode(n->left(), key));
			if (!n->left() || (n->left()->balance() == 0 && abs(oldBalance) == 1))
			{
				n->balance()++;
				if (n->balance() == 2)
				{
					result = rightBalance(n);
				}
			}
		}

		else if (key > n->key() && n->right())
		{
			Balance_t oldBalance = n->right()->balance();
			n->setRight(removeNode(n->right(), key));
			if (!n->right() || (n->right()->balance() == 0 && abs(oldBalance) == 1))
			{
				n->balance()--;
				if (n->balance() == -2)
				{
					result = leftBalance(n);
				}
			}
		}

		else if (n->key() == key)
		{
			if (n->left() == NULL && n->right() == NULL)
			{
				result = NULL;

				delete n;
			}
			else if (n->left() == NULL)
			{
				result = n->right();
				delete n;
			}
			else if (n->right() == NULL)
			{
				result = n->left();
				delete n;
			}
			else
			{
				Node * nextNode = n->right();
				while (nextNode->left() != NULL)
				{
					nextNode = nextNode->left();
				}
				n->swap(*nextNode);
				std::swap(n->value(), nextNode->value());
				result = nextNode;
				Balance_t oldBalance = nextNode->right()->balance();
				nextNode->setRight(removeNode(nextNode->right(), n->key()));
				if (!nextNode->right() || (nextNode->right()->balance() == 0 && abs(oldBalance) == 1))
				{
					nextNode->balance()--;
					if (nextNode->balance() == -2)
					{
						result = leftBalance(nextNode);
					}
				}

			}
		}
		return result;
	}

	static bool isValid(Node *n, Key_t& maxValue, Key_t& minValue)
	{
		if (n->balance() < -1 || n->balance() > 1)
		{
			return false;
		}

		if (n->left())
		{
			Key_t leftMax;
			if (!isValid(n->left(), leftMax, minValue))
			{
				return false;
			}
			if (leftMax >= n->key())
			{
				return false;
			}
		}
		else
		{
			minValue = n->key();
		}

		if (n->right())
		{
			Key_t rightMin;
			if (!isValid(n->right(), maxValue, rightMin))
			{
				return false;
			}
			if (rightMin <= n->key())
			{
				return false;
			}
		}
		else
		{
			maxValue = n->key();
		}
		return true;

	}

	static  Node* get(Node* n, std::size_t index)
	{
		if (n == NULL)
		{
			return NULL;
		}
		std::size_t leftCount = n->left() ? n->left()->count() : 0;

		if (index < leftCount)
		{
			return get(n->left(), index);
		}
		else if (index == leftCount)
		{
			return n;
		}
		else
		{
			return get(n->right(), index - leftCount - 1);
		}

	}
	static Node* find(Node * n, const Key_t& key)
	{
		if (!n)
		{
			return n;
		}
		if (n->key() == key)
		{
			return n;
		}
		if (key < n->key())
		{
			return find(n->left(), key);
		}
		if (key > n->key())
		{
			return find(n->right(), key);
		}
		return NULL;

	}
	Node* _root;
};
#endif
