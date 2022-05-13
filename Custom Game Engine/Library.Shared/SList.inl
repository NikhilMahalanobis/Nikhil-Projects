#include "SList.h"
namespace FieaGameEngine
{
	template<typename T>
	SList<T>::Iterator::Iterator(const SList& owner, Node* node) :
		_owner(&owner), _node(node)
	{
	}

	template<typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container!");
		}

		if (_node != nullptr)
		{
			_node = _node->Next;
		}

		return *this;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();
		return temp;
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& other) const
	{
		return other._owner != _owner || other._node != _node;
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename T>
	T& SList<T>::Iterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Cannot de-reference a null Node pointer!");
		}

		return _node->data;
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::begin()
	{
		return Iterator{ *this, _front };
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::end()
	{
		return Iterator{ *this };
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor)
	{
		Iterator it;
		for (it = begin(); it != end(); ++it)
		{
			if (equalityFunctor(*it, value))
			{
				break;
			}
		}

		return it;
	}

	template<typename T>
	SList<T>::ConstIterator::ConstIterator(const SList& owner, Node* node) :
		_owner(&owner), _node(node)
	{
	}

	template<typename T>
	SList<T>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner(other._owner), _node(other._node)
	{
	}

	template<typename T>
	typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container!");
		}

		if (_node != nullptr)
		{
			_node = _node->Next;
		}

		return *this;
	}

	template<typename T>
	typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		operator++();
		return temp;
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return other._owner != _owner || other._node != _node;
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename T>
	inline const T& SList<T>::ConstIterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Cannot de-reference a null Node pointer!");
		}

		return _node->data;
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::begin() const
	{
		return ConstIterator{ *this, _front };
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return ConstIterator{ *this, _front };
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::end() const
	{
		return ConstIterator{ *this }; 
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return ConstIterator{ *this };
	}

	template<typename T>
	typename SList<T>::ConstIterator SList<T>::Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor) const
	{
		ConstIterator it;
		for (it = begin(); it != end(); ++it)
		{
			if (equalityFunctor(*it, value))
			{
				break;
			}
		}

		return it;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::InsertAfter(const T& data, Iterator place, std::function<bool(const T&, const T&)> equalityFunctor)
	{
		if (place._owner != this)
		{
			throw std::runtime_error("The given Iterator does not belong to the given list.");
		}

		if (_front == _back || place == end())
		{
			PushBack(data);
			return Iterator{ *this, _back };
		}

		else
		{
			Node* insert = new Node(data, place._node->Next);
			place._node->Next = insert;
			++_size;
			return Iterator{ *this, insert };
		}
	}

	template<typename T>
	bool SList<T>::Remove(const T& data, std::function<bool(const T&, const T&)> equalityFunctor)
	{

		return Remove(Find(data, equalityFunctor));
	}

	template<typename T>
	inline bool SList<T>::Remove(Iterator position)
	{
		if (position._owner != this)
		{
			throw std::runtime_error("Iterator not with this container");
		}

		bool result = false;
		if (position != end())
		{
			if (position._node == _back)
			{
				PopBack();
			}

			else
			{
				Node* next = position._node->Next;
				position._node->data.~T();
				new(&(position._node->data))T(std::move(next->data));
				position._node->Next = next->Next;
				delete next;

				if (position._node->Next == nullptr)
				{
					_back = position._node;
				}

				--_size;
			}
			result = true;
		}

		return result;
	}

	template<typename T>
	SList<T>::Node::Node(const T& data, Node* Next) :
		data(data), Next(Next)
	{
	}

	template <typename T>
	SList<T>::SList() :
		_size(0), _front(nullptr), _back(nullptr)
	{}

	template <typename T>
	SList<T>::SList(std::initializer_list<T> list)
	{
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template <typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	template<typename T>
	SList<T>::SList(const SList& other)
	{
		Node* currentNode = other._front;
		while (currentNode != nullptr)
		{
			PushBack(currentNode->data);
			currentNode = currentNode->Next;
		}
	}

	template<typename T>
	SList<T>& SList<T>::operator=(const SList& other)
	{
		if (this != &other)
		{
			Clear();

			Node* currentNode = other._front;
			while (currentNode != nullptr)
			{
				PushBack(currentNode->data);
				currentNode = currentNode->Next;
			}
		}

		return *this;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushFront(const T& data)
	{
		_front = new Node(data, _front);

		if (_size == 0)
		{
			_back = _front;
		}

		++_size;
		return begin();
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& data)
	{
		Node* temp = new Node(data);

		if (_size == 0)
		{
			_front = temp;
		}
		else
		{
			_back->Next = temp;
		}
		_back = temp;
		_size++;
		return Iterator{ *this, _back };
	}

	template<typename T>
	void SList<T>::PopFront()
	{
		Node* temp;

		if (_front == nullptr)
		{
			throw std::runtime_error("Cannot delete member from an empty list.");
		}

		temp = _front;
		_front = _front->Next;

		--_size;
		delete temp;

		if (_size == 0)
		{
			_back = nullptr;
		}
	}

	template<typename T>
	void SList<T>::PopBack()
	{
		Node* current = _front;
		Node* prev = _front;

		if (_size == 0)
		{
			throw std::runtime_error("Cannot delete member from an empty list.");
		}

		if (_front == _back)
		{
			current = _back;
			delete current;
			_front = nullptr;
			_back = nullptr;
			--_size;
			return;
		}

		while (current->Next != nullptr)
		{
			prev = current;
			current = current->Next;
		}
		_back = prev;
		delete current;
		_back->Next = nullptr;
		--_size;

	}

	template<typename T>
	inline void SList<T>::Clear()
	{
		Node* prev = _front;
		Node* current;

		if (_size == 0)
		{
			return;
		}

		while (prev != nullptr)
		{
			current = prev->Next;
			delete prev;
			prev = current;
		}

		_front = nullptr;
		_back = nullptr;
		_size = 0;
	}

	template <typename T>
	T& SList<T>::Front()
	{
		if (_front == nullptr)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return _front->data;
	}

	template <typename T>
	const T& SList<T>::Front() const
	{
		if (_front == nullptr)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return _front->data;
	}

	template <typename T>
	T& SList<T>::Back()
	{
		if (_back == nullptr)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}
		return _back->data;
	}

	template <typename T>
	const T& SList<T>::Back() const
	{
		if (_back == nullptr)
		{
			throw std::runtime_error("Cannot access an empty list");
		}
		return _back->data;
	}

	template<typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return _size == 0;
	}

	template <typename T>
	inline std::size_t SList<T>::Size() const
	{
		return _size;
	}

}
