#pragma once
#include<cstdint>
#include<functional>
#include<initializer_list>
#include<stdexcept>

namespace FieaGameEngine
{
	template <typename T> class SList final
	{
	private:
		struct Node final
		{
			T data;
			Node* Next;

			Node(const T& data, Node* next = nullptr);
		};
	public:
		class Iterator
		{
			friend SList;
		public:
			/// <summary>
			/// Compiler supplied default constructor for Iterator
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Compares the equality of 2 Iterators based on their respective data
			/// </summary>
			/// <param name="other">Takes a const reference to an Iterator </param>
			/// <returns> Returns a bool </returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// Compares the inequality of 2 Iterators based on their respective data
			/// </summary>
			/// <param name="other"> Takes a const reference to an Iterator </param>
			/// <returns> Returns a bool </returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Pre-increments the Iterator to the next element in the List
			/// </summary>
			/// <returns> Returns a reference to an Iterator </returns>
			Iterator& operator++();

			/// <summary>
			/// Post-increments the Iterator to the next element in the list
			/// </summary>
			/// <param name=""> Takes a bogus int (not to be used) </param>
			/// <returns> Returns a copy of an Iterator </returns>
			Iterator operator++(int);

			/// <summary>
			/// Dereferences the Iterator 
			/// </summary>
			/// <returns> Returns a referene to a templated type T </returns>
			T& operator*() const;

		private:
			/// <summary>
			/// Iterator constructor that initializes the owner of the iterator and the current Node it's pointing to
			/// </summary>
			/// <param name="owner"> Takes a const reference to the owning SList </param>
			/// <param name="node"> Takes a Node pointer to the current node </param>
			Iterator(const SList& owner, Node* node = nullptr);

			Node* _node{ nullptr };
			const SList* _owner{nullptr};
		};

		class ConstIterator
		{
			friend SList;
		public:
			/// <summary>
			/// Compiler supplied default constructor for ConstIterator
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// One param constructor that "copies" from an Iterator object
			/// </summary>
			/// <param name="other"> Takes a const reference to an Iterator </param>
			ConstIterator(const Iterator& other);

			/// <summary>
			/// Two param constructor that initializes the owner and node of the ConstIterator
			/// </summary>
			/// <param name="owner"> Takes a const reference to the owning SList </param>
			/// <param name="node"> Takes a Node pointer to the current node </param>
			ConstIterator(const SList& owner, Node* node = nullptr);

			/// <summary>
			/// Compares the equality of 2 ConstIterators depending on the value of their data
			/// </summary>
			/// <param name="other"> Takes a const reference to a ConstIterator </param>
			/// <returns> Returns a bool </returns>
			bool operator==(const ConstIterator& other) const;

			/// <summary>
			/// Compares the inequality of 2 ConstIterators depending on the value of their data
			/// </summary>
			/// <param name="other"> Takes a reference to a ConstIterator </param>
			/// <returns> Returns a bool </returns>
			bool operator!=(const ConstIterator& other) const;

			/// <summary>
			/// Pre-increments the ConstIterator to the next member in the List
			/// </summary>
			/// <returns> Returns a reference to a ConstIterator </returns>
			ConstIterator& operator++();

			/// <summary>
			/// Post-increments the ConstIterator to the next member in the list
			/// </summary>
			/// <param name=""> Takes a bogus int (not to be used) </param>
			/// <returns> Returns a bool </returns>
			ConstIterator operator++(int);
			 
			/// <summary>
			/// Dereferences the ConstIterator to get data within
			/// </summary>
			/// <returns> Returns a const reference to a templated type T </returns>
			const T& operator*() const;

		private:
			Node* _node{ nullptr };
			const SList* _owner{ nullptr };
		}; 

		/// <summary>
		/// Takes a given SList Iterator, and templated data T, and inserts the data AFTER the given Iterator in the list
		/// </summary>
		/// <param name="data"> Takes a const reference to a templated type T </param>
		/// <param name="place"> Takes a copy of an Iterator </param>
		/// <param name="equalityFunctor"> Takes a function that takes 2 const references to a templated type T, and returns a bool </param>
		Iterator InsertAfter(const T& data, Iterator place, std::function<bool(const T&, const T&)> equalityFunctor = std::equal_to());

		/// <summary>
		/// Takes a given templated data T, and removes that element from the list if it's present
		/// </summary>
		/// <param name="data"> Takes a const reference to a templated type T </param>
		/// <param name="equalityFunctor"> Takes a function that takes 2 const references to a templated type T, and returns a bool </param>
		bool Remove(const T& data, std::function<bool(const T&, const T&)> equalityFunctor = std::equal_to());
		
		/// <summary>
		/// Takes a given Iterator and removes the element at that position from the list if present
		/// </summary>
		/// <param name="position"> Takes a copy of an Iterator </param>
		/// <returns> Returns a copy of a bool </returns>
		bool Remove(Iterator position);

		/// <summary>
		/// Attempts to find a given Iterator in the list if it's present
		/// </summary>
		/// <param name="value"> Takes a const reference to a templated type T </param>
		/// <param name="equalityFunctor"> Takes a function that takes 2 const references to a templated type T, and returns a bool </param>
		/// <returns> Returns a copy of an Iterator </returns>
		Iterator Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor = std::equal_to<T>());

		/// <summary>
		/// Attempts to find a given ConstIterator in the list if it's present
		/// </summary>
		/// <param name="value"> Takes a const reference to a templated type T </param>
		/// <param name="equalityFunctor"> Takes a function that takes 2 const references to a templated type T, and returns a bool </param>
		/// <returns> Returns a copy of a ConstIterator </returns>
		ConstIterator Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor = std::equal_to<T>()) const;

		/// <summary>
		/// Gets the Iterator that's at the beginning of the list
		/// </summary>
		/// <returns> Returns a copy of an Iterator </returns>
		Iterator begin();

		/// <summary>
		/// Gets the ConstIterator that's at the beginning of the list
		/// </summary>
		/// <returns> Returns a copy of a ConstIterator </returns>
		ConstIterator begin() const;

		/// <summary>
		/// Gets the Iterator that's at the beginning of the list and returns it as a ConstIterator
		/// </summary>
		/// <returns> Returns a copy to a ConstIterator </returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets the Iterator at the end of the list
		/// </summary>
		/// <returns> Returns a copy of an Iterator </returns>
		Iterator end();

		/// <summary>
		/// Gets the ConstIterator at the end of the list
		/// </summary>
		/// <returns> Returns a copy of the ConstIterator </returns>
		ConstIterator end() const;

		/// <summary>
		/// Gets the Iterator at the end of the list and returns it as a ConstIterator
		/// </summary>
		/// <returns> Returns a copy of a ConstIterator </returns>
		ConstIterator cend() const;


		/// <summary>
		/// Default Constructor that initializes link list to _size = 0, _front = _back = nullptr
		/// </summary>
		SList();

		/// <summary>
		/// Initializer List Constructor that initializes the linked list w/ the given initializer list of T elements
		/// </summary>
		/// <param name="list"> Takes a templated initializer list of type T</param>
		SList(std::initializer_list<T> list);

		/// <summary>
		/// Deconstructor that calls the Clear() function 
		/// </summary>
		~SList();

		/// <summary>
		/// Constructs a deep copy from a given SList
		/// </summary>
		/// <param name="other"> Takes a reference to a const SList </param>
		SList(const SList& other);

		/// <summary>
		/// First calls Clear(), then assigns a deep copy from a given SList
		/// </summary>
		/// <param name="other"> Takes a reference to a const SList </param>
		/// <returns> Returns a reference of a SList </returns>
		SList& operator=(const SList& other);
		
		/// <summary>
		/// Creates a Node and pushes it to the front of the list, increments size
		/// </summary>
		/// <param name="data"> Takes a reference to a const templated type T </param>
		Iterator PushFront(const T& data);

		/// <summary>
		/// Creates a Node and pushes it to the back of the list, increments size
		/// </summary>
		/// <param name="data"> Takes a reference to a const templated type T</param>
		Iterator PushBack(const T& data);

		/// <summary>
		/// Removes the Node at the front of the list, and re-arranges the _front pointer to the new front
		/// </summary>
		void PopFront();

		/// <summary>
		/// Removes the Node at the back of the list, and rearranges the _back pointer to the new back
		/// </summary>
		void PopBack();

		/// <summary>
		/// Iterates through the list and Destroys all Nodes, resets _size = 0, _front = nullptr, _back = nullptr
		/// </summary>
		void Clear();

		/// <summary>
		/// Non-const version of Front, returns the value of the data sitting in the Node at the front of the list
		/// </summary>
		/// <returns> A reference to a templated type T </returns>
		T& Front();

		/// <summary>
		/// Const version of Front, returns the value of the data sitting in the Node at the front of the list
		/// </summary>
		/// <returns> A reference to a const templated type T </returns>
		const T& Front() const;

		/// <summary>
		/// Non-const version of Back, returns the value of data sitting in the Node at the back of the list
		/// </summary>
		/// <returns> A reference to a templated type T </returns>
		T& Back();

		/// <summary>
		/// Const version of Back, returns the value of data sitting in the Node at the back of the list
		/// </summary>
		/// <returns> A reference to a const templated type T </returns>
		const T& Back() const;

		/// <summary>
		/// Returns true if the List is empty (i.e. _size == 0), false otherwise
		/// </summary>
		/// <returns> A constant boolean dependent on the condition (_size == 0) </returns>
		bool IsEmpty() const;

		/// <summary>
		/// Gives the size of the list, determined by _size
		/// </summary>
		/// <returns> A reference to a const size_t which corresponds to _size </returns>
		std::size_t Size() const;

	private:
		std::size_t _size{ 0 };
		Node* _front{ nullptr };
		Node* _back{ nullptr };
	};


}

#include "SList.inl"