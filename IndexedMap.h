#ifndef __INDEX_MAP_HEADER__
#define __INDEX_MAP_HEADER__
#include <string>


template<typename Key_t, typename Data_t>
class IndexedMap
{

public:
	typedef std::pair<Key_t, Data_t> Value_t;
protected:

	typedef char Balance_t;
public:

	IndexedMap() 
	: _root(NULL)
	{
	}

	template<typename InputIterator>
	IndexedMap(InputIterator first, InputIterator last)
	 :_root(NULL)
	{
		for (InputIterator it = first; it != last; ++it)
		{
			insert(it->first, it->second);
		}
	}

	void insert(const Key_t& key, const Data_t& data)
	{
		bool isChanged = false;
		_root = insert(_root, key, data, isChanged);
		_root->setRoot();
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

	bool find(const Key_t& key)
	{
		return find(_root, key) != NULL;
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

	~IndexedMap()
	{
		freeNode(_root);
	}

	class Iterator;

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
			if (_n->right())
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
		friend Iterator IndexedMap::begin();
	};

protected:

	class Node
	{
	public:

		Node(const Key_t key, const Data_t& data)
			:_value(key, data), _balance(0), _count(1), _left(NULL), _right(NULL), _parent(NULL)
		{
		}
	/*	~Node()
		{
			Node* p = parent();
			while (p)
			{
				--p->_count;
				p = p->parent;
			}
		}
		*/
		void setLeft(Node* left)
		{
			_left = left;
			if (_left)
			{
				_left->_parent = this;
			}
			updateCount();
		}

		void setRight(Node* right)
		{
			_right = right;
			if (_right)
			{
				_right->_parent = this;
			}
			updateCount();
		}

		void setData(const Data_t& data)
		{
			_value.second = data;
		}

		void setKey(const Key_t& key)
		{
			_value.first = key;
		}


		Node* left() const
		{
			return _left;
		}

		Node* right() const
		{
			return _right;
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
			if (_left)
			{
				_count += _left->_count;

			}
			if (_right)
			{
				_count += _right->_count;

			}
		}

	protected:

		Value_t _value;
		Balance_t _balance;
		std::size_t _count;
		Node* _left;
		Node* _right;
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
				n->setData(nextNode->data());
				n->setKey(nextNode->key());
				Balance_t oldBalance = n->right()->balance();
				n->setRight(removeNode(n->right(), nextNode->key()));
				if (!n->right() || (n->right()->balance() == 0 && abs(oldBalance) == 1))
				{
					n->balance()--;
					if (n->balance() == -2)
					{
						result = leftBalance(n);
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