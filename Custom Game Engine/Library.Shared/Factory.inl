#include "Factory.h"

namespace FieaGameEngine
{
	template<typename T>
	HashMap<std::string, const Factory<T>* const> Factory<T>::_factoryTable{ 31 };

	template<typename T>
	inline const Factory<T>* const Factory<T>::Find(const std::string& className) 
	{
		auto it = _factoryTable.Find(className);

		if (it == _factoryTable.end())
		{
			return nullptr;
		}

		return it->second;
	}

	template<typename T>
	inline gsl::owner<T*> Factory<T>::Create(const std::string className)
	{
		auto it = _factoryTable.Find(className);

		if (it == _factoryTable.end())
		{
			return nullptr;
		}

		return it->second->Create();
	}

	template<typename T>
	inline void Factory<T>::Add(const Factory<T>& factory)
	{
		auto [it, insertResult] = _factoryTable.Insert(std::make_pair(factory.ClassName(), &factory));

		if (insertResult)
		{
			throw std::runtime_error("Error, tried to add a type w/ an already existing ClassName!");
		}
	}

	template<typename T>
	inline void Factory<T>::Remove(const Factory<T>& factory)
	{
		_factoryTable.Remove(factory.ClassName());
	}
	
}