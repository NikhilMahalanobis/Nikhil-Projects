#include "Scope.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Scope)

	Scope::Scope(size_t capacity) :
		_scopeMap(capacity), _orderArray(capacity)
	{
	}

	Scope::Scope(const Scope& rhs)
	{
		if (this != &rhs)
		{
			for (const auto& it : rhs._orderArray)
			{
				auto foundDatum = it->second;
				Datum newDatum;
				if (foundDatum.Type() == DatumTypes::Table)
				{
					newDatum.SetType(DatumTypes::Table);
					newDatum.Reserve(foundDatum.Size());
					for (size_t i = 0; i < foundDatum.Size(); ++i)
					{
						Scope& nestedScope = foundDatum[i];
						Scope* childCopy = nestedScope.Clone();
						childCopy->_owner = this;
						newDatum.PushBack(*childCopy);
					}
				}
				else
				{
					newDatum = foundDatum;
				}

				auto pair = std::make_pair(it->first, newDatum);

				auto [iteratorResult, insertBool] = _scopeMap.Insert(pair);
				_orderArray.PushBack(&(*iteratorResult));
			}
		}
	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			for (const auto& it : rhs._orderArray)
			{
				auto foundDatum = it->second;
				Datum newDatum;
				if (foundDatum.Type() == DatumTypes::Table)
				{
					newDatum.SetType(DatumTypes::Table);
					newDatum.Reserve(foundDatum.Size());
					for (size_t i = 0; i < foundDatum.Size(); ++i)
					{
						Scope& nestedScope = foundDatum[i];
						Scope* childCopy = nestedScope.Clone();
						childCopy->_owner = this;
						newDatum.PushBack(*childCopy);
					}
				}
				else
				{
					newDatum = foundDatum;
				}

				auto pair = std::make_pair(it->first, newDatum);

				auto [iteratorResult, insertBool] = _scopeMap.Insert(pair);
				_orderArray.PushBack(&(*iteratorResult));
			}
		}

		return *this;
	}

	Scope::Scope(Scope&& rhs) noexcept :
		_scopeMap(std::move(rhs._scopeMap)), _orderArray(std::move(rhs._orderArray)), _owner(rhs._owner)
	{
		if (rhs._owner != nullptr)
		{
			auto [foundDatum, index] = rhs._owner->FindContainedScope(rhs);
			foundDatum->Set(*this, index);
			rhs._owner = nullptr;
		}

		for (const auto& it : _orderArray)
		{
			auto foundDatum = it->second;
			if (foundDatum.Type() == DatumTypes::Table)
			{
				for (size_t i = 0; i < foundDatum.Size(); ++i)
				{
					Scope& nestedScope = foundDatum[i];
					nestedScope._owner = this;
				}
			}
		}
	}

	Scope& Scope::operator=(Scope&& rhs) noexcept
	{ 
		Clear();

		_owner = rhs._owner;
		_scopeMap = std::move(rhs._scopeMap);
		_orderArray = std::move(rhs._orderArray);

		if (rhs._owner != nullptr)
		{
			auto [foundDatum, index] = rhs._owner->FindContainedScope(rhs);
			foundDatum->Set(*this, index);
			rhs._owner = nullptr;
		}

		for (const auto& it : _orderArray)
		{
			auto foundDatum = it->second;
			if (foundDatum.Type() == DatumTypes::Table)
			{
				for (size_t i = 0; i < foundDatum.Size(); ++i)
				{
					Scope& nestedScope = foundDatum[i];
					nestedScope._owner = this;
				}
			}
		}

		return *this;
	}

	Scope::~Scope()
	{
		if (_owner != nullptr)
		{
			Orphan();
		}

		Clear();
	}

	void Scope::Clear()
	{
		for (auto it = _orderArray.begin(); it != _orderArray.end(); ++it)
		{
			if ((*it)->second.Type() == DatumTypes::Table)
			{
				size_t size = (*it)->second.Size();
				for (size_t i = 0; i < size; ++i)
				{
					Scope* scope = &(*it)->second.GetTable(i);
					scope->_owner = nullptr;
					delete scope;
				}
			}
			(*it)->second.Clear();
		}
		_scopeMap.Clear();
		_orderArray.Clear();
	}

	bool Scope::operator==(const Scope& rhs) const
	{
		if (_scopeMap.Size() != rhs._scopeMap.Size() &&  _orderArray.Size() != rhs._orderArray.Size())
		{
			return false;
		}

		auto lVecIt = _orderArray.begin();
		auto rVecIt = rhs._orderArray.begin();
		for (lVecIt, rVecIt; lVecIt != _orderArray.end() && rVecIt != rhs._orderArray.end(); ++lVecIt, ++rVecIt)
		{
			auto left = *(*lVecIt);
			auto right = *(*rVecIt);

			if (left.second.Type() == DatumTypes::Table && right.second.Type() == DatumTypes::Table)
			{
				for (size_t i = 0; i < left.second.Size() && i < right.second.Size(); ++i)
				{
					if (left.second.GetTable(i).operator==(right.second.GetTable(i)) == false)
					{
						return false;
					}
				}
			}
			else if (*(*lVecIt) != *(*rVecIt))
			{
				return false;
			}
		}

		return true;
	}

	bool Scope::operator!=(const Scope& rhs) const
	{
		return !operator==(rhs);
	}

	bool Scope::operator==(const Scope& rhs)
	{
		if (_scopeMap.Size() != rhs._scopeMap.Size() && _orderArray.Size() != rhs._orderArray.Size())
		{
			return false;
		}

		auto lVecIt = _orderArray.begin();
		auto rVecIt = rhs._orderArray.begin();
		for (lVecIt, rVecIt; lVecIt != _orderArray.end() && rVecIt != rhs._orderArray.end(); ++lVecIt, ++rVecIt)
		{
			auto left = *(*lVecIt);
			auto right = *(*rVecIt);

			if (left.second.Type() == DatumTypes::Table && right.second.Type() == DatumTypes::Table)
			{
				for (size_t i = 0; i < left.second.Size() && i < right.second.Size(); ++i)
				{
					if (left.second.GetTable(i).operator==(right.second.GetTable(i)) == false)
					{
						return false;
					}
				}
			}
			else if (*(*lVecIt) != *(*rVecIt))
			{
				return false;
			}
		}

		return true;
	}

	bool Scope::operator!=(const Scope& rhs)
	{
		return !operator==(rhs);
	}

	Scope* Scope::Clone() const
	{
		return new Scope(*this);
	}

	Datum& Scope::Append(const std::string& name)
	{
		bool wasNotInserted;
		return Append(name, wasNotInserted);
	}

	Datum& Scope::Append(const std::string& name, bool& wasNotInserted)
	{		
		if (name.empty())
		{
			throw invalid_argument("name cannot be empty.");
		}

		auto result = _scopeMap.Insert(std::make_pair(name, Datum()));
		auto& entry = *result.first;
		wasNotInserted = result.second;
		if (wasNotInserted == false)
		{
			_orderArray.PushBack(&entry);
		}

		return entry.second;
	}

	Scope& Scope::AppendScope(const std::string& name)
	{
		bool entryCreated;
		Datum& datum = Append(name, entryCreated);
		if (entryCreated == false)
		{
			if (datum.Type() != DatumTypes::Unknown && datum.Type() != DatumTypes::Table)
			{
				throw runtime_error("Name already exists but is not of type Scope.");
			}

			assert(!datum.IsExternal());
		}

		Scope* scope = new Scope();
		scope->_owner = this;
		datum.PushBack(*scope);

		return *scope;
	}

	Datum* Scope::Find(const std::string& name)
	{
		auto found = _scopeMap.Find(name);
		
		if (found == _scopeMap.end())
		{
			return nullptr;
		}

		return &(found->second);
	}

	Datum* Scope::Search(const std::string& name, Scope*& scope)
	{
		Datum* foundDat = Find(name);

		if (foundDat != nullptr)
		{
			if (foundDat->Type() == DatumTypes::Table)
			{
				scope = &(foundDat->GetTable());
			}
			else
			{
				scope = nullptr;
			}
			return foundDat;
		}
		
		if (_owner == nullptr)
		{
			return nullptr;
		}

		return _owner->Search(name, scope);
	}

	std::pair<Datum*, size_t> Scope::FindContainedScope(const Scope& scope) const
	{
		size_t count = 0;
		for (auto it = _orderArray.begin(); it != _orderArray.end(); ++it, ++count)
		{
			if ((*it)->second.Type() == DatumTypes::Table)
			{
				auto foundPair = (*it)->second.Find(scope);
				if (foundPair.second)
				{
					//std::pair<Datum*, size_t> pair = 
					return std::make_pair(&((*it)->second), foundPair.first);
				}

			}
		}

		return std::make_pair(nullptr, 0);
	}

	void Scope::Adopt(Scope& child, const std::string& name)
	{		
		if (child == *this)
		{
			throw std::runtime_error("Error, cannot adopt yourself");
		}

		if (child.IsDescendantOf(*this))
		{
			throw std::runtime_error("Error, trying to adopt a descendant!");
		}

		bool wasNotInserted;
		Datum& datum = Append(name, wasNotInserted);
		if (wasNotInserted)
		{
			if (datum.Type() != DatumTypes::Unknown && datum.Type() != DatumTypes::Table)
			{
				throw std::runtime_error("Found an existing datum that is not of type Table.");
			}
		}

		// If already has parent, get rid of entries in Parent
		if (child._owner != nullptr)
		{
			child.Orphan();
		}
		child._owner = this;
		datum.PushBack(child);
	}

	void Scope::Orphan()
	{
		/*if (_owner == nullptr)
		{
			throw std::runtime_error("Error, cannot Orphan Scope with no Parent!");
		}

		auto [foundDatum, foundIndex] = _owner->FindContainedScope(*this);
		assert(foundDatum != nullptr);

		ScopeTablePointer foundVal = nullptr;
		for (auto it = _owner->_orderArray.begin(); it != _owner->_orderArray.end(); ++it)
		{
			if ((*it)->second == *foundDatum)
			{
				foundVal = *it;
				_owner->_orderArray.Remove(it);
				break;
			}
		}
		assert(foundVal != nullptr);
		_owner->_scopeMap.Remove(foundVal->first);

		_owner = nullptr;*/

		if (_owner != nullptr)
		{
			auto [foundDatum, foundIndex] = _owner->FindContainedScope(*this);
			assert(foundDatum != nullptr);

			foundDatum->RemoveAt(foundIndex);
			_owner = nullptr;
		}
	}

	bool Scope::IsAncestorOf(const Scope& scope) const
	{
		bool result = false;
		Scope* parent = scope._owner;
		while (parent != nullptr)
		{
			if (parent == this)
			{
				result = true;
				break;
			}
			parent = parent->_owner;
		}

		return result;
	}

	bool Scope::IsDescendantOf(const Scope& scope) const
	{
		return scope.IsAncestorOf(*this);
	}

	Scope* Scope::GetParent() const
	{
		return _owner;
	}
	size_t Scope::Size() const
	{
		return _orderArray.Size();
	}
	bool Scope::IsEmpty() const
	{
		return Size() == 0;
	}
	Datum& Scope::operator[](const std::string& name)
	{
		return Append(name);
	}
	Datum& Scope::operator[](size_t index)
	{
		return (*(_orderArray.At(index))).second;
	}
}