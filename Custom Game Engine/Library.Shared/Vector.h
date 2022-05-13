  #pragma once
#include <cassert>
#include <stdexcept>
#include <initializer_list>
#include <functional>

using namespace std;

namespace FieaGameEngine
{
	template <typename T> class Vector final
	{
	public:

		class Iterator
		{
			friend Vector;
		
		public:
			
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::bidirectional_iterator_tag;

			/// <summary>
			/// Compiler supplied default constructor for Iterator
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Copy Constructor for Iterator
			/// </summary>
			/// <param name="other"> Takes a const reference to an Iterator </param>
			Iterator(const Iterator& other) = default;

			/// <summary>
			/// Copy Assignment Operator for Iterator
			/// </summary>
			/// <param name="other"> Takes a const reference to an Iterator </param>
			/// <returns> Returns a reference to an Iterator </returns>
			Iterator& operator=(const Iterator& other) = default;

			/// <summary>
			/// Equality Operator for Iterator
			/// </summary>
			/// <param name="other"> Takes const reference to an Iterator </param>
			/// <returns> Returns a bool </returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// Inequality Operator for Iterator
			/// </summary>
			/// <param name="other"> Takes a const reference to an Iterator</param>
			/// <returns> Returns a bool </returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Pre-increment operator for Iterator
			/// </summary>
			/// <returns> Returns a reference to an Iterator </returns>
			Iterator& operator++();

			/// <summary>
			/// Post-increment operator for Iterator
			/// </summary>
			/// <param name=""> Takes a bogus int </param>
			/// <returns> Returns a copy of an Iterator </returns>
			Iterator operator++(int);

			/// <summary>
			/// Pre-decrement operator for Iterator
			/// </summary>
			/// <returns> Returns a reference to an Iterator </returns>
			Iterator& operator--();

			/// <summary>
			/// Post-decrement operator for Iterator
			/// </summary>
			/// <param name=""> Takes a copy of a bogus int </param>
			/// <returns> Returns a copy of an Iterator </returns>
			Iterator operator--(int);
			/// <summary>
			/// De-reference operator for Iterator
			/// </summary>
			/// <returns> Returns a reference to a templated type T </returns>
			T& operator*() const;

		private:
			/// <summary>
			/// Overloaded default constructor for Iterator
			/// </summary>
			/// <param name="owner"> Takes a reference to a Vector </param>
			/// <param name="index"> Takes a copy of a size_t </param>
			Iterator(Vector& owner, size_t index = 0);

			Vector* _owner{ nullptr };
			size_t currentIndex{ 0 };
		};

		class ConstIterator
		{
			friend Vector;

		public:

			/// <summary>
			/// Compiler supplied default constructor for ConstIterator
			/// </summary>
			ConstIterator() = default;
			
			/// /// <summary>
			/// Overloaded default constructor for ConstIterator
			/// </summary>
			/// <param name="other"> Takes a const reference to an Iterator</param>
			ConstIterator(const Iterator& other);

			/// <summary>
			/// Copy Constructor for ConstIterator
			/// </summary>
			/// <param name="other"> Takes a const reference to a ConstIterator </param>
			ConstIterator(const ConstIterator& other) = default;


			/// <summary>
			/// Copy Assignment operator for ConstIterator
			/// </summary>
			/// <param name="other"> Takes a const reference to a ConstIterator </param>
			/// <returns> Returns a reference to a ConstIterator </returns>
			ConstIterator& operator=(const ConstIterator& other) = default;

			/// <summary>
			/// Equality operator for ConstIterator
			/// </summary>
			/// <param name="other"> Takes a const reference to a ConstIterator </param>
			/// <returns> Returns a copy of a bool </returns>
			bool operator==(const ConstIterator& other) const;

			/// <summary>
			/// Inequality operator for ConstIterator
			/// </summary>
			/// <param name="other"> Takes a const reference to a ConstIterator </param>
			/// <returns> Returns a copy of a bool </returns>
			bool operator!=(const ConstIterator& other) const;

			/// <summary>
			/// Pre-increment operator for ConstIterator 
			/// </summary>
			/// <returns> Returns a reference to a ConstIterator </returns>
			ConstIterator& operator++();

			/// <summary>
			/// Post-increment operator for ConstIterator
			/// </summary>
			/// <param name=""> Takes a bogus int </param>
			/// <returns> Returns a copy of a ConstInterator </returns>
			ConstIterator operator++(int);
			 
			/// <summary>
			/// De-reference operator for ConstIterator
			/// </summary>
			/// <returns> Returns a const reference to a templated type T </returns>
			const T& operator*() const;

		private:
			/// <summary>
			/// Overloaded default constructor for ConstIterator
			/// </summary>
			/// <param name="owner"> Takes a const reference to a Vector </param>
			/// <param name="index"> Takes a copy of a size_t </param>
			ConstIterator(const Vector& owner, size_t index = 0);


			const Vector* _owner{ nullptr };
			size_t currentIndex{ 0 };
		};

		/// <summary>
		/// Gets the Iterator at the beginning of the Vector
		/// </summary>
		/// <returns> Returns a copy of an Iterator </returns>
		Iterator begin();

		/// <summary>
		/// Gets the ConstIterator at the beginning of the const Vector
		/// </summary>
		/// <returns> Returns a copy of a ConstIterator </returns>
		ConstIterator begin() const;

		/// <summary>
		/// Gets the Iterator at the beginning of a Vector, but returns it as a ConstIterator
		/// </summary>
		/// <returns> Returns a copy of a ConstIterator </returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets the Iterator at the end of the Vector
		/// </summary>
		/// <returns> Returns a copy of an Iterator </returns>
		Iterator end();

		/// <summary>
		/// Gets the ConstIterator at the end of the const Vector
		/// </summary>
		/// <returns> Returns a copy of a ConstIterator </returns>
		ConstIterator end() const;

		/// <summary>
		/// Gets the Iterator at the end of the Vector, but returns it as a ConstIterator
		/// </summary>
		/// <returns> Returns a copy of a ConstIterator </returns>
		ConstIterator cend() const;

		/// <summary>
		/// Finds the given value in the Vector, using the given equalityFunctor
		/// </summary>
		/// <param name="value"> takes a const reference to a templated type T </param>
		/// <param name="equalityFunctor"> Takes a function that takes 2 const references to a templated type T, and returns a copy of a bool </param>
		/// <returns> Returns a copy of an Iterator </returns>
		Iterator Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor = std::equal_to<T>());

		/// <summary>
		/// Finds the given value in the const Vector, using the given equalityFunctor
		/// </summary>
		/// <param name="value"> Takes a const reference to a templated type T </param>
		/// <param name="equalityFunctor">Takes a function that takes 2 const references to a templated type T, and returns a copy of a bool </param>
		/// <returns> Returns a copy of a ConstIterator </returns>
		ConstIterator Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor = std::equal_to<T>()) const;

		/// <summary>
		/// Default constructor that takes a capacity and allocates memory for the table
		/// </summary>
		/// <param name="capacity"> Takes a copy to a size_t </param>
		explicit Vector(std::size_t capacity = 10);

		/// <summary>
		/// Destructor for Vector, calls Clear and frees _table
		/// </summary>
		~Vector();

		/// <summary>
		/// Initializer list constructor for Vector
		/// </summary>
		/// <param name="list"> Takes a copy of an initializer list of templated type T </param>
		Vector(std::initializer_list<T> list);

		/// <summary>
		/// Copy Constructor for Vector
		/// </summary>
		/// <param name="other"> Takes a const reference to a Vector </param>
		Vector(const Vector& other);

		/// <summary>
		/// Copy Assignment Operator for Vector
		/// </summary>
		/// <param name="other"> Takes a const reference to a Vector </param>
		/// <returns> Returns a reference to a Vector </returns>
		Vector& operator=(const Vector& other);

		/// <summary>
		/// Move Constructor for Vector
		/// </summary>
		/// <param name="other"> Takes a rvalue reference to a Vector </param>
		Vector(Vector&& other) noexcept;

		/// <summary>
		/// Move Assignment Operator for Vector
		/// </summary>
		/// <param name="other"> Takes a rvalue reference to a Vector </param>
		/// <returns> Returns a reference to a Vector </returns>
		Vector& operator=(Vector&& other) noexcept;

		/// <summary>
		/// Indexing operator for Vector
		/// </summary>
		/// <param name="index"> Takes a copy of a size_t </param>
		/// <returns> Returns a reference to a templated type T </returns>
		T& operator[](size_t index);

		/// <summary>
		/// Indexing operator for const Vector
		/// </summary>
		/// <param name="index"> Takes a copy of a size_t </param>
		/// <returns> Returns a const reference to a templated type T </returns>
		const T& operator[](size_t index) const;

		/// <summary>
		/// At function for Vector that calls the index operator
		/// </summary>
		/// <param name="index"> Takes a copy of a size_t </param>
		/// <returns> Returns a reference to a templated type T </returns>
		T& At(size_t index);

		/// <summary>
		/// At function for const Vector that call the index operator
		/// </summary>
		/// <param name="index"> Takes a copy of a size_t </param>
		/// <returns> Returns a const reference to a templated type T </returns>
		const T& At(size_t index) const;

		/// <summary>
		/// Pushes the data to the back of the Vector
		/// </summary>
		/// <param name="data"> Takes a const reference to a templated type T </param>
		void PushBack(const T& data);

		/// <summary>
		/// Removes the element at the back of the Vector
		/// </summary>
		void PopBack();

		/// <summary>
		/// Returns the data at the front of the Vector
		/// </summary>
		/// <returns> Returns a reference to a templated type T </returns>
		T& Front();

		/// <summary>
		/// Returns the data at the front of the const Vector
		/// </summary>
		/// <returns> Returns a const reference to a templated type T </returns>
		const T& Front() const;

		/// <summary>
		/// Returns the data at the back of the Vector
		/// </summary>
		/// <returns> Returns a reference to a templated type T </returns>
		T& Back();

		/// <summary>
		/// Returns the data at the back of the const Vector
		/// </summary>
		/// <returns> Returns a const reference to a templated type T </returns>
		const T& Back() const;

		/// <summary>
		/// Re-allocates a new capacity for the Vector
		/// </summary>
		/// <param name="newCapacity"> Takes a copy of a size_t </param>
		void Reserve(size_t newCapacity);

		/// <summary>
		/// Re-sizes the vector, both to smaller and larger sizes
		/// </summary>
		/// <param name="newSize"> Takes a copy of a size_t </param>
		void Resize(size_t newSize);

		/// <summary>
		/// Clears all the elements from the Vector 
		/// </summary>
		void Clear();

		/// <summary>
		/// Overloaded Remove that removes the element at the given index
		/// </summary>
		/// <param name="index"> Takes a const reference to a size_t </param>
		void Remove(const size_t& index);

		/// <summary>
		/// Overloaded Remove that removes the element pointed at by the given Iterator
		/// </summary>
		/// <param name="it"> Takes a const reference to an Iterator </param>
		void Remove(const Iterator& it);

		/// <summary>
		/// Overloaded Remove that removes a range of elements based on Iterator position
		/// </summary>
		/// <param name="first"> Takes a const reference to an Iterator </param>
		/// <param name="last"> Takes a const reference to an Iterator </param>
		void Remove(const Iterator& first, const Iterator& last);

		/// <summary>
		/// Gives the current Size of the Vector
		/// </summary>
		/// <returns> Returns a copy of a size_t </returns>
		size_t Size() const;

		/// <summary>
		/// Gives the current Capacity of the Vector
		/// </summary>
		/// <returns> Returns a copy of a size_t </returns>
		size_t Capacity() const;

		/// <summary>
		/// Checks to see if size == 0
		/// </summary>
		/// <returns> Returns a copy of a bool </returns>
		bool IsEmpty() const;
	private:
		T* _table{ nullptr };
		size_t _capacity{ 0 };
		size_t _size{ 0 };
	};
}

#include "Vector.inl"