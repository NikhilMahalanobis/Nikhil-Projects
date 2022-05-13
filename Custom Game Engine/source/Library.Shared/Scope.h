#pragma once
#include "HashMap.h"
#include "DefaultHash.h"
#include "Datum.h"
#include<string>
#include<functional>
#include<cassert>
#include "RTTI.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI)

	public:
		using ScopeTable = HashMap<std::string, Datum>;
		using ScopeTablePointer = HashMap<std::string, Datum>::PairType*;

		/// <summary>
		/// TypeCast Constructor for Scope
		/// </summary>
		/// <param name="capacity"> Takes a copy of a size_t </param>
		Scope(size_t capacity = 10);

		/// <summary>
		/// Copy Constructor for Scope
		/// </summary>
		/// <param name="rhs"> Takes a const reference to a Scope </param>
		Scope(const Scope& rhs);

		/// <summary>
		/// Copy Assignment Operator for Scope
		/// </summary>
		/// <param name="rhs"> Takes a const reference to a Scope </param>
		/// <returns> Returns a reference to a Scope </returns>
		Scope& operator=(const Scope& rhs);

		/// <summary>
		/// Move Constructor for Scope
		/// </summary>
		/// <param name="rhs"> Takes a rValue reference to a Scope </param>
		Scope(Scope&& rhs) noexcept;

		/// <summary>
		/// Move Assignment Operator for Scope
		/// </summary>
		/// <param name="rhs"> Takes a rValue reference to a Scope</param>
		/// <returns> Returns a lValue reference to a Scope </returns>
		Scope& operator=(Scope&& rhs) noexcept;

		/// <summary>
		/// Virtual Destructor for Scope, calls Orphan and Clear
		/// </summary>
		virtual ~Scope();
		
		/// <summary>
		/// Clear method for Scope, clears out _scopeMap and _orderArray
		/// </summary>
		void Clear();

		/// <summary>
		/// Equality operator for Scope
		/// </summary>
		/// <param name="rhs"> Takes a const reference to a Scope </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator==(const Scope& rhs) const;

		/// <summary>
		/// Inequality Operator for Scope, calls Equality Operator
		/// </summary>
		/// <param name="rhs"> Takes a const reference to a Scope </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator!=(const Scope& rhs) const;

		bool operator==(const Scope& rhs);
		bool operator!=(const Scope& rhs);
		/// <summary>
		/// Virtual Clone method for Scope, heap allocates a Scope
		/// </summary>
		/// <returns> Returns a pointer to a heap allocated Scope </returns>
		virtual Scope* Clone() const;

		/// <summary>
		/// Append method for Scope, appends a string,Datum pair to the Scope
		/// If it exists already, return that one
		/// </summary>
		/// <param name="name"> Takes a const reference to a std::string </param>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& Append(const std::string& name);

		/// <summary>
		/// AppendScope method for Scope, appends a nested Scope to the current Scope
		/// If it exists already, return that one
		/// </summary>
		/// <param name="name"> Takes a const reference to a std::string </param>
		/// <returns> Returns a reference to a Scope </returns>
		Scope& AppendScope(const std::string& name);

		/// <summary>
		/// Find method for Scope, finds the Datum at the given key in this Scope
		/// </summary>
		/// <param name="name"> Takes a const reference to a std::string </param>
		/// <returns> Returns a pointer to a Datum </returns>
		Datum* Find(const std::string& name);

		/// <summary>
		/// Search method for Scope, finds the most closely nested Datum from this Scope
		/// </summary>
		/// <param name="name"> Takes a const reference to a std::string </param>
		/// <param name="scope"> Takes a reference to a pointer to a Scope </param>
		/// <returns> Returns a pointer to a Datum </returns>
		Datum* Search(const std::string& name, Scope*& scope);
		
		/// <summary>
		/// Find Contained Scope method for Scope, finds the corresponding scope in this Scope
		/// </summary>
		/// <param name="scope"> Takesa a const reference to a Scope </param>
		/// <returns> Returns a copy of a std::pair of type <Datum*, size_t> </returns>
		std::pair<Datum*, size_t> FindContainedScope(const Scope& scope) const;

		/// <summary>
		/// Adopt method for Scope, nests' the given Scope inside of this one
		/// </summary>
		/// <param name="child"> Takes a reference to a Scope </param>
		/// <param name="name"> Takes a const reference to a std::string </param>
		void Adopt(Scope& child, const std::string& name);

		/// <summary>
		/// Orphan method for Scope, un-parents this Scope from it's owner
		/// </summary>
		void Orphan();

		/// <summary>
		/// Is Ancestor of method for Scope, searches if this Scope is an ancestor of the given Scope
		/// </summary>
		/// <param name="scope"> Takes a const reference to a Scope </param>
		/// <returns> Returns a copy of a bool </returns>
		bool IsAncestorOf(const Scope& scope) const;

		/// <summary>
		/// Is Descendant of method for Scope, checks if this Scope is a descendant of the given Scope
		/// </summary>
		/// <param name="scope"> Takes a const reference to a Scope </param>
		/// <returns> Return a copy of a bool </returns>
		bool IsDescendantOf(const Scope& scope) const;

		/// <summary>
		/// Get Parent method for Scope, gets the owner of this Scope
		/// </summary>
		/// <returns> Returns a pointer to a Scope </returns>
		Scope* GetParent() const;

		/// <summary>
		/// Size method for Scope, gets the number of elements appended
		/// </summary>
		/// <returns> Returns a copy of a size_t </returns>
		size_t Size() const;

		/// <summary>
		/// Is Empty method for Scope, checks if _size == 0
		/// </summary>
		/// <returns> Returns a copy of a bool </returns>
		bool IsEmpty() const;

		/// <summary>
		/// Index Operator overload for Datum, thinly wraps Append
		/// </summary>
		/// <param name="name"> Takes a reference to a const std::string </param>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& operator[](const std::string& name);
		
		/// <summary>
		/// Index Operator overload for Datum, calls Index Operator in _orderArray
		/// </summary>
		/// <param name="index"> Takes a copy of a size_t </param>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& operator[](size_t index);
	protected:
		Scope* _owner{ nullptr };
		ScopeTable _scopeMap{ 10 };
		Vector<ScopeTablePointer> _orderArray;

	private:
		Datum& Append(const std::string& name, bool& wasNotInserted);
	};

	ConcreteFactory(Scope, Scope)
}