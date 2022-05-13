#include "Vector.h"

namespace FieaGameEngine
{
	template <typename T>
	Vector<T>::Iterator::Iterator(Vector& owner, size_t index) :
		_owner(&owner), currentIndex(index)
	{
	}

	template <typename T>
	inline bool Vector<T>::Iterator::operator==(const Iterator& other) const
	{
		return (_owner == other._owner && currentIndex == other.currentIndex);
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator!=(const Iterator& other) const
	{
		return !(operator==(other));
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Error, Iterator is currently not associated with a Vector!");
		}

		if (currentIndex < _owner->_size)
		{
			++currentIndex;
		}

		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator* temp = this;
		operator++();
		return *temp;
	}

	template<typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Error, Iterator currently not associated with a Vector!");
		}

		if (currentIndex > 0)
		{
			--currentIndex;
		}

		return *this;
	}

	template<typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
	{
		Iterator* temp = this;
		operator--();
		return *temp;
	}

	template<typename T>
	typename T& Vector<T>::Iterator::operator*() const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Error, cannot dereference an Iterator that is not associated with a Vector!");
		}

		if (currentIndex >= _owner->_size)
		{
			throw std::runtime_error("Error, cannot dereference end of Vector!");
		}
		
		return _owner->_table[currentIndex];
	}

	template<typename T>
	Vector<T>::ConstIterator::ConstIterator(const Vector& owner, size_t index) :
		_owner(&owner), currentIndex(index)
	{

	}

	template<typename T>
	Vector<T>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner(other._owner), currentIndex(other.currentIndex)
	{

	}

	template <typename T>
	inline bool Vector<T>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return (_owner == other._owner && currentIndex == other.currentIndex);
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return !(operator==(other));
	}

	template <typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Error, Iterator is currently not associated with a Vector!");
		}

		if (currentIndex < _owner->_size)
		{
			++currentIndex;
		}

		return *this;
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator* temp = this;
		operator++();
		return *temp;
	}

	template<typename T>
	typename const T& Vector<T>::ConstIterator::operator*() const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Error, cannot dereference an Iterator that is not associated with a Vector!");
		}

		if (currentIndex >= _owner->_size)
		{
			throw std::runtime_error("Error, cannot dereference end of Vector!");
		}

		return _owner->_table[currentIndex];
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator(*this, 0);
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator{ *this, _size };
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::begin() const
	{
		return ConstIterator{ *this };
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::end() const
	{
		return ConstIterator{ *this, _size };
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator{ *this };
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator{ *this, _size }; 
	}

	template<typename T>
	typename Vector<T>::Iterator Vector<T>::Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor)
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
	typename Vector<T>::ConstIterator Vector<T>::Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor) const
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
	inline Vector<T>::Vector(std::size_t capacity) :
		_capacity(capacity), _table(capacity > 0 ? reinterpret_cast<T*>(malloc(sizeof(T) * capacity)) : nullptr)
	{
	}

	template<typename T>
	Vector<T>::~Vector()
	{
		Clear();
		free(_table);
	}

	template<typename T>
	Vector<T>::Vector(std::initializer_list<T> list)
	{
		T* temp = reinterpret_cast<T*>(malloc(sizeof(T) * list.size()));
		assert(temp != nullptr);
		_table = temp;
		_capacity = list.size();
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template<typename T>
	Vector<T>::Vector(const Vector& other) :
		_capacity(other._capacity)
	{
		T* temp = reinterpret_cast<T*>(realloc(_table, sizeof(T) * _capacity));
		assert(temp != nullptr);
		_table = temp;

		for (const auto& value : other)
		{
			PushBack(value);
		}
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(const Vector& other)
	{
		if (this != &other)
		{
			Clear();

			_capacity = other._capacity;
			T* temp = static_cast<T*>(realloc(_table, sizeof(T) * _capacity));
			_table = temp;

			for (size_t i = 0; i < other._size; ++i)
			{
				PushBack(other.At(i));
			}
		}

		return *this;
	}

	template<typename T>
	Vector<T>::Vector(Vector&& rhs) noexcept :
		_size(rhs._size), _capacity(rhs._capacity), _table(rhs._table) 
	{
		rhs._size = 0;
		rhs._capacity = 0;
		rhs._table = nullptr;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clear();
			free(_table);
			_size = rhs._size;
			_capacity = rhs._capacity;
			_table = rhs._table;

			rhs._size = 0;
			rhs._capacity = 0;
			rhs._table = nullptr;
		}

		return *this;
	}

	template<typename T>
	inline T& Vector<T>::operator[](size_t index)
	{
		if (index >= _size || _size == 0)
		{
			throw runtime_error("Error, index out of bounds, element not valid!");
		}
		
		return _table[index];
	}
	
	template<typename T>
	inline const T& Vector<T>::operator[](size_t index) const
	{
		if (index >= _size || _size == 0)
		{
			throw runtime_error("Error, index out of bounds, element not valid!");
		}

		return _table[index];
	}
	
	template<typename T>
	inline T& Vector<T>::At(size_t index)
	{
		return operator[](index);
	}
	
	template<typename T>
	inline const T& Vector<T>::At(size_t index) const
	{
		return operator[](index);
	}

	template<typename T>
	inline void Vector<T>::PushBack(const T& data)
	{
		if (_size == _capacity)
		{
			// Grow
			const std::size_t VECTOR_GROWTH_FACTOR = 2;
			_capacity *= VECTOR_GROWTH_FACTOR;
			T* temp = static_cast<T*>(realloc(_table, sizeof(T) * _capacity));

			if (temp == nullptr)
			{
				throw runtime_error("Error, out of usable memory!");
			}

			_table = temp;
		}

		new(_table + _size) T(data);
		++_size;
	}

	template<typename T>
	inline void Vector<T>::PopBack()
	{
		if (_size == 0)
		{
			return;
		}

		_table[_size - 1].~T();
		--_size;
	}

	template<typename T>
	inline T& Vector<T>::Front()
	{
		if (_size == 0)
		{
			throw runtime_error("Error, cannot access Front of an empty Vector.");
		}

		return _table[0];
	}

	template<typename T>
	inline const T& Vector<T>::Front() const
	{
		if (_size == 0)
		{
			throw runtime_error("Error, cannot Front of an empty Vector.");
		}

		return _table[0];
	}

	template<typename T>
	inline T& Vector<T>::Back()
	{
		if (_size == 0)
		{
			throw runtime_error("Error, cannot access the Back of an empty Vector");
		}

		return _table[_size - 1];
	}

	template<typename T>
	inline const T& Vector<T>::Back() const
	{
		if (_size == 0)
		{
			throw runtime_error("Error, cannot access Back of an empty Vector.");
		}

		return _table[_size - 1];
	}

	template<typename T>
	inline void Vector<T>::Reserve(size_t newCapacity)
	{
		if (newCapacity < _capacity)
		{
			throw runtime_error("Error, cannot reserve less than the current capacity.");
		}

		T* temp = static_cast<T*>(realloc(_table, sizeof(T) * newCapacity));

		if (temp == nullptr)
		{	
			throw runtime_error("Error, not enough memory available!");
		}
		_table = temp;
		_capacity = newCapacity;
	}

	template<typename T>
	void Vector<T>::Resize(size_t newSize)
	{
		if (newSize < _size)
		{
			for (size_t i = newSize; i < _size; ++i)
			{
				_table[i].~T();
			}

			_capacity = newSize;
		}

		else
		{
			Reserve(newSize);

			for (size_t i = _size; i < newSize; ++i)
			{
				new(_table + i)T();
			}
		}

		_size = newSize;
	}

	template<typename T>
	inline void Vector<T>::Clear()
	{
		while (_size != 0)
		{
			_table[_size - 1].~T();
			--_size;
		}
	}

	template<typename T>
	void Vector<T>::Remove(const size_t& index)
	{
		if (index >= _size)
		{
			throw runtime_error("Error, index is invalid, not in range!");
		}

		_table[index].~T();
		auto size = (_size - index - 1) * sizeof(T);
		memmove(&_table[index], &_table[index + 1], size);
		--_size;
	}

	template<typename T>
	inline void Vector<T>::Remove(const Iterator& it)
	{
		if (it._owner != this)
		{
			throw runtime_error("Error, this iterator does not belong to this Vector!");
		}

		if (it == end())
		{
			throw runtime_error("Error, Iterator is at end() of Vector, cannot remove!");
		}

		if (it.currentIndex == (_size - 1))
		{
			PopBack();
		}

		else
		{
			Remove(it.currentIndex);
		}
	}

	template<typename T>
	void Vector<T>::Remove(const Iterator& first, const Iterator& last)
	{
		if ((first._owner != last._owner) || (first._owner != this))
		{
			throw std::runtime_error("Error, Iterators do not belong to the same container!");
		}

		if (last.currentIndex < first.currentIndex)
		{
			throw std::runtime_error("Error, iterators in invalid order!");
		}

		const auto elementsToRemove = last.currentIndex - first.currentIndex;
		if (elementsToRemove > 0)
		{
			for (size_t i = first.currentIndex; i < last.currentIndex; ++i)
			{
				_table[i].~T();
			}

			const auto elementsToMove = _size - last.currentIndex;
			if (elementsToMove > 0)
			{
				const auto size = elementsToMove * sizeof(T);
#if defined(_WIN64) || defined(_WIN32)
				memmove_s(&_table[first.currentIndex], size, &_table[last.currentIndex], size);
#else
				memmove(&_table[first.currentIndex], &_table[last.currentIndex], size);
#endif
			}

			_size -= elementsToRemove;
		}
	}

	template<typename T>
	inline size_t Vector<T>::Size() const
	{
		return _size;
	}

	template<typename T>
	inline size_t Vector<T>::Capacity() const
	{
		return _capacity;
	}
	
	template<typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return _size == 0;
	}
}