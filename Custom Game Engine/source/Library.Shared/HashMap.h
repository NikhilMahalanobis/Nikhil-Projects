#pragma once
#include "SList.h"
#include "Vector.h"
#include "DefaultHash.h"

using namespace std;

namespace FieaGameEngine
{
	template <typename TKey, typename TValue> 
	class HashMap
	{
	public:
		using PairType = std::pair<const TKey, TValue>;
		using ChainType = SList<PairType>;
		using ConstChainType = const SList<PairType>;
		using BucketType = Vector<ChainType>;
		using ChainIterator = typename ChainType::Iterator;
		using ChainConstIterator = typename ChainType::ConstIterator;
		using HashFunctor = std::function<size_t(const TKey&)>;
		using EqualityFunctor = std::function<bool(const TKey&, const TKey&)>;

		class Iterator
		{
			friend HashMap;

		public:
			/// <summary>
			/// Compiler supplied default constructor for Iterator
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Compiler supplied default copy constructor for Iterator
			/// </summary>
			/// <param name="other"></param>
			Iterator(const Iterator& other) = default;

			/// <summary>
			/// Compiler supplied default copy assignment operator for Iterator
			/// </summary>
			/// <param name="other"></param>
			/// <returns></returns>
			Iterator& operator=(const Iterator& other) = default;

			/// <summary>
			/// Non-equality operator for Iterator
			/// </summary>
			/// <param name="other"> Takes a const reference to an Iterator </param>
			/// <returns> Returns a copy of a bool </returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Equality operator for Iterator
			/// </summary>
			/// <param name="other"> Takes a const reference to an Iterator </param>
			/// <returns> Returns a copy of a bool </returns>
			bool operator==(const Iterator& other) const;

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
			/// De-reference operator for Iterator
			/// </summary>
			/// <returns> Returns a reference to a PairType </returns>
			PairType& operator*() const;

			/// <summary>
			/// Arrow operator for Iterator
			/// </summary>
			/// <returns> Returns a pointer to a PairType </returns>
			PairType* operator->() const;

		private:
			/// <summary>
			/// Default constructor for Iterator w/ 2 parameters
			/// </summary>
			/// <param name="index"> Takes a copy of a size_t </param>
			/// <param name="owner"> Takes a reference to a HashMap </param>
			Iterator(size_t index, HashMap& owner);

			/// <summary>
			/// Default constructor for Iterator w/ 3 parameters
			/// </summary>
			/// <param name="index"> Takes a copy of a size_t </param>
			/// <param name="owner"> Takes a reference to a HashMap </param>
			/// <param name="chainIterator"> Takes a copy of a ChainIterator </param>
			Iterator(size_t index, HashMap& owner, ChainIterator chainIterator);
			
			size_t currentIndex{ 0 };
			HashMap* _owner{ nullptr };
			ChainIterator _chainIterator;
		};

		class ConstIterator
		{
			friend HashMap;

		public:
			/// <summary>
			/// Compiler supplied default constructor for ConstIterator
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Default constructor for ConstIterator that takes an Iterator
			/// </summary>
			/// <param name="other"> Takes a const reference to an Iterator </param>
			ConstIterator(const Iterator& other);

			/// <summary>
			/// Compiler supplied default copy constructor for ConstIterator
			/// </summary>
			/// <param name="other"> Takes a const reference to a ConstIterator </param>
			ConstIterator(const ConstIterator& other) = default;

			/// <summary>
			/// Compiler supplied default copy assignment operator for ConstIterator
			/// </summary>
			/// <param name="other"> Takes a const reference to a ConstIterator </param>
			/// <returns> Returns a reference to a ConstIterator </returns>
			ConstIterator& operator=(const ConstIterator & other) = default;

			/// <summary>
			/// Non-equality operator for ConstIterator
			/// </summary>
			/// <param name="other"> Takes a const reference to a ConstIterator </param>
			/// <returns> Returns a copy of a bool </returns>
			bool operator!=(const ConstIterator & other) const;

			/// <summary>
			/// Equality operator for ConstIterator
			/// </summary>
			/// <param name="other"> Takes a const reference to a ConstIterator </param>
			/// <returns> Returns a copy of a bool </returns>
			bool operator==(const ConstIterator & other) const;

			/// <summary>
			/// Pre-increment operator for ConstIterator
			/// </summary>
			/// <returns> Returns a reference to a ConstIterator </returns>
			ConstIterator& operator++();

			/// <summary>
			/// Post-increment operator for ConstIterator
			/// </summary>
			/// <param name=""> Takes a bogus int </param>
			/// <returns> Returns a copy of a ConstIterator </returns>
			ConstIterator operator++(int);

			/// <summary>
			/// Dereference operator for ConstIterator
			/// </summary>
			/// <returns> Returns a const reference to a PairType </returns>
			const PairType& operator*() const;

			/// <summary>
			/// Arrow operator for ConstIterator
			/// </summary>
			/// <returns> Returns a const pointer to a PairType </returns>
			const PairType* operator->() const;
		private:
			/// <summary>
			/// Default constructor for ConstIterator w/ 2 parameters
			/// </summary>
			/// <param name="index"> Takes a copy of a size_t </param>
			/// <param name="owner"> Takes a const reference to a HashMap </param>
			ConstIterator(size_t index, const HashMap& owner);

			/// <summary>
			/// Default Constructor for ConstIterator w/ 3 parameters
			/// </summary>
			/// <param name="index"> Takes a copy to a size_t </param>
			/// <param name="owner"> Takes a const reference to a HashMap </param>
			/// <param name="chainIterator"> Takes a copy of a ChainIterator </param>
			ConstIterator(size_t index, const HashMap& owner, ChainConstIterator chainIterator);

			size_t currentIndex{ 0 };
			const HashMap* _owner{ nullptr };
			ChainConstIterator _chainIterator;
		};

		/// <summary>
		/// Gets the Iterator at the beginning of the HashMap
		/// </summary>
		/// <returns> Returns a copy of an Iterator </returns>
		Iterator begin();

		/// <summary>
		/// Gets the Iterator at the end of the HashMap
		/// </summary>
		/// <returns> Returns a copy of an Iterator </returns>
		Iterator end();

		/// <summary>
		/// Gets the ConstIterator at the beginning of the HashMap
		/// </summary>
		/// <returns> Returns a copy of a ConstIterator </returns>
		ConstIterator begin () const;
		 
		/// <summary>
		/// Gets the ConstIterator at the end of the HashMap
		/// </summary>
		/// <returns> Returns a copy of a ConstIterator </returns>
		ConstIterator end() const;

		/// <summary>
		/// Gets the Iterator at the beginning of the HashMap, and returns it as a ConstIterator
		/// </summary>
		/// <returns> Returns a copy of a ConstIterator </returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets the Iterator at the end of the HashMap, and returns it as a ConstIterator
		/// </summary>
		/// <returns> Returns a copy of a ConstIterator </returns>
		ConstIterator cend() const;

		/// <summary>
		/// Default constructor for HashMap
		/// </summary>
		/// <param name="hashMapSize"> Takes a copy of a size_t </param>
		/// <param name="hashFunction"> Takes a function that takes a const reference to a templated type TKey, and returns a copy of a size_t </param>
		/// <param name="equalityFunction"> Takes a fucntion that takes 2 const references to a templated type TKey, and returns a copy of a bool </param>
		HashMap(size_t hashMapSize = 11, HashFunctor hashFunction = DefaultHash<TKey>(), EqualityFunctor equalityFunction = std::equal_to<TKey>());

		/// <summary>
		/// Default initializer list constructor for HashMap
		/// </summary>
		/// <param name="list"> Takes a copy of an initializer_list </param>
		/// <param name="hashFunction"> Takes a function that takes a const reference to a templated type TKey, and returns a copy of a size_t </param>
		/// <param name="equalityFunction"> Takes a fucntion that takes 2 const references to a templated type TKey, and returns a copy of a bool </param>
		HashMap(std::initializer_list<PairType> list, HashFunctor hashFunction = DefaultHash<TKey>(), EqualityFunctor equalityFunction = std::equal_to<TKey>());

		/// <summary>
		/// Compiler supplied default Destructor for HashMap
		/// </summary>
		~HashMap() = default;

		/// <summary>
		/// Compiler supplied default copy constructor for HashMap
		/// </summary>
		/// <param name="other"> Takes a const reference to a HashMap </param>
		HashMap(const HashMap& other) = default;

		/// <summary>
		/// Compiler supplied default copy assignment operator for HashMap
		/// </summary>
		/// <param name="other"> Takes a const reference to a HashMap </param>
		/// <returns> Returns a reference to a HashMap </returns>
		HashMap& operator=(const HashMap& other) = default;

		/// <summary>
		/// Compiler supplied default move constructor for HashMap
		/// </summary>
		/// <param name="other"> Takes a rvalue reference to a HashMap </param>
		HashMap(HashMap&& other) = default;

		/// <summary>
		/// Compiler supplied default move assignment operator for HashMap
		/// </summary>
		/// <param name="other"> Takes a rvalue reference to a HashMap </param>
		/// <returns></returns>
		HashMap& operator=(HashMap&& other) = default;

		/// <summary>
		/// Non-const Index operator for HashMap
		/// </summary>
		/// <param name="key"> Takes a const reference to a templated type TKey </param>
		/// <returns> Returns a reference to a templated type TValue </returns>
		TValue& operator[](const TKey& key);

		/// <summary>
		/// Non-const At function for HashMap
		/// </summary>
		/// <param name="key"> Takes a const reference to a templated type TKey </param>
		/// <returns> Returns a reference to a templated type TValue </returns>
		TValue& At(const TKey& key);
		
		/// <summary>
		/// Const At function for HashMap
		/// </summary>
		/// <param name="key"> Takes a const reference to a templated type TKey </param>
		/// <returns> Returns a const reference to a templated type TValue </returns>
		const TValue& At(const TKey& key) const;

		/// <summary>
		/// Non-const Find function for HashMap
		/// </summary>
		/// <param name="key"> Takes a const reference to a templated type TKey </param>
		/// <returns> Returns a copy of a std::pair<Iterator, bool> </returns>
		Iterator Find(const TKey& key);

		/// <summary>
		/// Const Find function for HashMap
		/// </summary>
		/// <param name="key"> Takes a const reference to a templated type TKey </param>
		/// <returns> Returns a copy of a std::pair<ConstIterator, bool> </returns>
		ConstIterator Find(const TKey& key) const;

		/// <summary>
		/// Const ContainsKey function for HashMap
		/// </summary>
		/// <param name="key"> Takes a const reference to a templated type TKey </param>
		/// <returns> Returns a copy of a bool </returns>
		bool ContainsKey(const TKey& key) const;

		/// <summary>
		/// Insert function for HashMap
		/// </summary>
		/// <param name="other"> Takes a const reference to a PairType </param>
		/// <returns> Returns a copy of an Iterator </returns>
		std::pair<Iterator, bool> Insert(const PairType& other);

		/// <summary>
		/// Remove function for HashMap
		/// </summary>
		/// <param name="key"> Takes a const reference to a TKey </param>
		void Remove(const TKey& key);

		/// <summary>
		/// Clear function for HashMap
		/// </summary>
		void Clear();

		/// <summary>
		/// Size function for HashMap
		/// </summary>
		/// <returns> Returns a copy of a size_t </returns>
		size_t Size() const;

		/// <summary>
		/// Capacity function for HashMap
		/// </summary>
		/// <returns> Returns a copy of a size_t </returns>
		size_t Capacity() const;
	private:
		Iterator Find(const TKey & key, size_t & index);
		ConstIterator Find(const TKey & key, size_t & index) const;

		size_t _size{ 0 };
		HashFunctor _hashFunction;
		EqualityFunctor _equalityFunction;
		BucketType _hashMap;

	};
}

#include "HashMap.inl"