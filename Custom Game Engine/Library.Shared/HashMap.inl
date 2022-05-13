#include "HashMap.h"

namespace FieaGameEngine
{
	template<typename TKey, typename TValue>
	HashMap<TKey, TValue>::Iterator::Iterator(size_t index, HashMap& owner) :
		currentIndex(index), _owner(&owner)
	{

	}

	template<typename TKey, typename TValue>
	HashMap<TKey, TValue>::Iterator::Iterator(size_t index, HashMap& owner, ChainIterator chainIterator) :
		currentIndex(index), _owner(&owner), _chainIterator(chainIterator)
	{

	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::Iterator::operator!=(const Iterator& other) const
	{
		/*if (_owner == other._owner)
		{
			if (currentIndex == other.currentIndex)
			{
				if (currentIndex < _owner->_hashMap.Size())
				{
					return _chainIterator != other._chainIterator;
				}
				return false;
			}
		}

		return true;*/

		return (_owner != other._owner || currentIndex != other.currentIndex || _chainIterator != other._chainIterator);
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::Iterator& HashMap<TKey, TValue>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw runtime_error("Error, Iterator currently not associated with a HashMap!");
		}

		if (currentIndex < _owner->_hashMap.Size())
		{
			++_chainIterator;
			if (_chainIterator == _owner->_hashMap.At(currentIndex).end())
			{
				++currentIndex;
				while (currentIndex < _owner->_hashMap.Size())
				{
					if (!(_owner->_hashMap.At(currentIndex).IsEmpty()))
					{
						_chainIterator = _owner->_hashMap.At(currentIndex).begin();
						return *this;
					}
					
					++currentIndex;
				}
			}
		}

		if (currentIndex >= _owner->_hashMap.Size())
		{
			_chainIterator = _owner->_hashMap[currentIndex - 1].end();
		}
		return *this;
	}

	template<typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::Iterator::operator++(int)
	{
		Iterator* temp = this;
		operator++();
		return *temp;
	}

	template<typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::PairType& HashMap<TKey, TValue>::Iterator::operator*() const
	{
		if (_owner == nullptr)
		{
			throw runtime_error("Error, cannot Dereference an Iterator not associated with a HashMap!");
		}

		if (currentIndex >= _owner->_hashMap.Size())
		{
			throw runtime_error("Error, cannot Dereference end of Vector!");
		}

		return *_chainIterator;
	}

	template<typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::PairType* HashMap<TKey, TValue>::Iterator::operator->() const
	{
		if (_owner == nullptr)
		{
			throw runtime_error("Error, cannot Dereference an Iterator not associated with a HashMap!");
		}

		if (currentIndex >= _owner->_hashMap.Size())
		{
			throw runtime_error("Error, cannot Dereference end of Vector!");
		}

		return &(*_chainIterator);
	}

	template<typename TKey, typename TValue>
	HashMap<TKey, TValue>::ConstIterator::ConstIterator(size_t index, const HashMap& owner) :
		currentIndex(index), _owner(&owner)
	{
	}

	template<typename TKey, typename TValue>
	HashMap<TKey, TValue>::ConstIterator::ConstIterator(size_t index, const HashMap& owner, ChainConstIterator chainIterator) :
		currentIndex(index), _owner(&owner), _chainIterator(chainIterator)
	{

	}

	template<typename TKey, typename TValue>
	HashMap<TKey, TValue>::ConstIterator::ConstIterator(const Iterator& other) :
		currentIndex(other.currentIndex), _owner(other._owner), _chainIterator(other._chainIterator)
	{

	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		/*if (_owner == other._owner)
		{
			if (currentIndex == other.currentIndex)
			{
				if (currentIndex < _owner->_hashMap.Size())
				{
					return _chainIterator != other._chainIterator;
				}
				return false;
			}
		}

		return true;*/
		return (_owner != other._owner || currentIndex != other.currentIndex || _chainIterator != other._chainIterator);

	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::ConstIterator& HashMap<TKey, TValue>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw runtime_error("Error, ConstIterator currently not associated with a HashMap!");
		}

		if (currentIndex < _owner->_hashMap.Size())
		{
			++_chainIterator;
			if (_chainIterator == _owner->_hashMap.At(currentIndex).end())
			{
				++currentIndex;
				while (currentIndex < _owner->_hashMap.Size())
				{
					if (!(_owner->_hashMap.At(currentIndex).IsEmpty()))
					{
						_chainIterator = _owner->_hashMap.At(currentIndex).begin();
						return *this;
					}

					++currentIndex;
				}
			}
		}

		if (currentIndex >= _owner->_hashMap.Size())
		{
			_chainIterator = _owner->_hashMap[currentIndex - 1].end();
		}
		return *this;
	}

	template<typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::ConstIterator::operator++(int)
	{
		ConstIterator* temp = this;
		operator++();
		return *temp;
	}

	template<typename TKey, typename TValue>
	typename const HashMap<TKey, TValue>::PairType& HashMap<TKey, TValue>::ConstIterator::operator*() const
	{
		if (_owner == nullptr)
		{
			throw runtime_error("Error, cannot Dereference an Iterator not associated with a HashMap!");
		}

		if (currentIndex >= _owner->_hashMap.Size())
		{
			throw runtime_error("Error, cannot Dereference end of Vector!");
		}

		return *_chainIterator;
	}

	template<typename TKey, typename TValue>
	typename const HashMap<TKey, TValue>::PairType* HashMap<TKey, TValue>::ConstIterator::operator->() const
	{
		if (_owner == nullptr)
		{
			throw runtime_error("Error, cannot Dereference an Iterator not associated with a HashMap!");
		}

		if (currentIndex >= _owner->_hashMap.Size())
		{
			throw runtime_error("Error, cannot Dereference end of Vector!");
		}

		return &(*_chainIterator);
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::begin()
	{
		if (_size != 0)
		{
			for (size_t i = 0; i < _hashMap.Size(); ++i)
			{
				if (_hashMap.At(i).Size() > 0)
				{
					return Iterator(i, *this, _hashMap.At(i).begin());
				}	
			}
		}

		return end();
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::end()
	{
		return Iterator(_hashMap.Size(), *this , _hashMap[_hashMap.Size() - 1].end());
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::begin() const
	{
		if (_size != 0)
		{
			for (size_t i = 0; i < _hashMap.Size(); ++i)
			{
				if (_hashMap.At(i).Size() > 0)
				{
					return ConstIterator(i, *this, _hashMap.At(i).begin());
				}
			}
		}

		return end();
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::end() const
	{
		return ConstIterator(_hashMap.Size(), *this, _hashMap[_hashMap.Size() - 1].end());

	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::cbegin() const
	{
		if (_size != 0)
		{
			for (size_t i = 0; i < _hashMap.Size(); ++i)
			{
				if (_hashMap.At(i).Size() > 0)
				{
					return ConstIterator(i, *this, _hashMap.At(i).begin());
				}

			}
		}

		return cend();
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::cend() const
	{
		return ConstIterator(_hashMap.Size(), *this, _hashMap[_hashMap.Size() - 1].end());
	}

	template<typename TKey, typename TValue>
	HashMap<TKey, TValue>::HashMap(size_t hashMapSize, HashFunctor hashFunction, EqualityFunctor equalityFunction) :
		_hashFunction(hashFunction), _equalityFunction(equalityFunction), _hashMap(hashMapSize)
	{
		_hashMap.Resize(hashMapSize);
	}

	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::HashMap(std::initializer_list<PairType> list, HashFunctor hashFunction, EqualityFunctor equalityFunction) :
		_hashFunction(hashFunction), _equalityFunction(equalityFunction), _hashMap(list.size())
	{
		_hashMap.Resize(list.size());
		for (const auto& value : list)
		{
			Insert(value);
		}
	}

	template<typename TKey, typename TValue>
	TValue& HashMap<TKey, TValue>::operator[](const TKey& key)
	{
		std::pair insertPair = make_pair(key, TValue());
		return (*(Insert(insertPair).first)).second;
	}

	template<typename TKey, typename TValue>
	inline TValue& HashMap<TKey, TValue>::At(const TKey& key)
	{
		Iterator foundResult = Find(key);

		if (foundResult != end())
		{
			return (*foundResult).second;
		}

		throw runtime_error("Error, index out of bounds, key does not exist in HashMap!");
	}

	template<typename TKey, typename TValue>
	inline const TValue& HashMap<TKey, TValue>::At(const TKey& key) const
	{
		ConstIterator foundResult = Find(key);

		if (foundResult != end())
		{
			return (*(foundResult)).second;
		}

		throw runtime_error("Error, index out of bounds, key does not exist in HashMap!");
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::Find(const TKey& key)
	{
		size_t index;
		return Find(key, index);
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::Find(const TKey& key) const
	{
		size_t index;
		return Find(key, index);
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::Find(const TKey& key, size_t& index)
	{
		index = _hashFunction(key) % Capacity();
		ChainType& chain = _hashMap[index];
		for (auto chainIterator = chain.begin(); chainIterator != chain.end(); ++chainIterator)
		{
			if (_equalityFunction(key, (*chainIterator).first))
			{
				return Iterator(index, *this, chainIterator);
			}
		}

		return end();
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::Find(const TKey& key, size_t& index) const
	{
		index = _hashFunction(key) % Capacity();
		auto& chain = _hashMap[index];
		for (auto chainIterator = chain.begin(); chainIterator != chain.end(); ++chainIterator)
		{
			if (_equalityFunction(key, (*chainIterator).first))
			{
				return ConstIterator(index, *this, chainIterator);
			}
		}

		return end();
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::ContainsKey(const TKey& key) const
	{
		
		auto foundResult = Find(key);
		if (foundResult == end())
		{
			return false;
		}

		return true;
	}

	template<typename TKey, typename TValue>
	inline std::pair<typename HashMap<TKey, TValue>::Iterator, bool> HashMap<TKey, TValue>::Insert(const PairType& other)
	{
		bool didFind = false;
		size_t index;
		Iterator foundResult = Find(other.first, index);

		if (foundResult != end())
		{
			didFind = true;
			return make_pair(foundResult, didFind);
		}

		//size_t index = _hashFunction(other.first) % Capacity();
		ChainIterator newChainIterator = _hashMap[index].PushBack(other);
		++_size;
		return make_pair(Iterator(index, *this, newChainIterator), didFind);
	}

	template<typename TKey, typename TValue>
	inline void HashMap<TKey, TValue>::Remove(const TKey& key)
	{
		size_t index;
		Iterator foundResult = Find(key, index);

		if (foundResult != end())
		{
			_hashMap[index].Remove((*foundResult));
			--_size;
		}
	}

	template<typename TKey, typename TValue>
	inline void HashMap<TKey, TValue>::Clear()
	{
		for (auto& bucket : _hashMap)
		{
			bucket.Clear();
		}
		_size = 0;
	}


	template<typename TKey, typename TValue>
	inline size_t HashMap<TKey, TValue>::Size() const
	{
		return _size;
	}

	template<typename TKey, typename TValue>
	inline size_t HashMap<TKey, TValue>::Capacity() const
	{
		return _hashMap.Capacity();
	}
}