#pragma once

#include "RTTI.h"
#include "HashMap.h"
#include "Signature.h"

namespace FieaGameEngine
{
	class TypeRegistry
	{
	public:
		/// <summary>
		/// getInstance method for TypeRegsitry
		/// </summary>
		/// <returns> Returns a static pointer to a TypeRegistry </returns>
		static TypeRegistry* getInstance();

		/// <summary>
		/// Templated registerType method for TypeRegistry
		/// </summary>
		template<typename TClass>
		void registerType()
		{
			if (TypeRegistry::_instance == nullptr)
			{
				throw std::runtime_error("Error, Type Registry singleton has not been initialized yet!");
			}

			TypeRegistry::_instance->_typeRegistry.Insert(std::make_pair(TClass::TypeIdClass(), TClass::Signatures()));
		};

		/// <summary>
		/// Templated registerParentedType method for TypeRegistry
		/// </summary>
		template <typename TClass, typename TParentClass>
		void registerParentedType()
		{
			auto child = TClass::TypeIdClass();
			auto parent = TParentClass::TypeIdClass();

			if (TypeRegistry::_instance == nullptr)
			{
				throw std::runtime_error("Error, Type Registry singleton has not been initialized yet!");
			}

			Vector<Signature> insertAttribute;

			auto foundParent = TypeRegistry::_instance->_typeRegistry.Find(parent);
			if (foundParent == TypeRegistry::_instance->_typeRegistry.end())
			{
				throw std::runtime_error("Error, associated Parent not registered in Type Registry yet!");
			}
			else
			{
				insertAttribute = (*foundParent).second;
			}

			for (auto value : TClass::Signatures())
			{
				insertAttribute.PushBack(value);
			}

			TypeRegistry::_instance->_typeRegistry.Insert(std::make_pair(child, insertAttribute));
		};

		/// <summary>
		/// Find method for TypeRegisty, wraps HashMap Find
		/// </summary>
		/// <param name="typeId"> Takes a copy of a size_t </param>
		/// <returns> Returns a reference to a HashMap Pairtype of <size_t, Vector<Signature>> </returns>
		typename HashMap<RTTI::IdType, Vector<Signature>>::PairType& Find(RTTI::IdType typeId);

		/// <summary>
		/// Clear method for TypeRegistry 
		/// </summary>
		void Clear();

	private:
		/// <summary>
		/// Compiler supplied Default Constructor for TypeRegistry
		/// </summary>
		TypeRegistry() = default;

		/// <summary>
		/// Compiler supplied Destructor for TypeRegistry 
		/// </summary>
		~TypeRegistry() = default;

		/// <summary>
		/// Deleted Copy Constructor for TypeRegistry 
		/// </summary>
		/// <param name="other"></param>
		TypeRegistry(const TypeRegistry& other) = delete;

		/// <summary>
		/// Deleted Move Constructor for TypeRegistry
		/// </summary>
		/// <param name="other"></param>
		TypeRegistry(TypeRegistry&& other) = delete;

		/// <summary>
		/// Deleted Copy Assignment Operator for TypeRegistry
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		TypeRegistry& operator=(const TypeRegistry& other) = delete;

		/// <summary>
		/// Deleted Move Assignment Operator for TypeRegistry 
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		TypeRegistry& operator=(TypeRegistry&& other) = delete;


		static TypeRegistry* _instance;
		HashMap<RTTI::IdType, Vector<Signature>> _typeRegistry;
	};
}

//#include "TypeRegistry.inl"