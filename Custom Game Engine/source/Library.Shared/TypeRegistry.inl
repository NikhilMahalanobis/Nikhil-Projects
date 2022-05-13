//#include "TypeRegistry.h"
//
//namespace FieaGameEngine
//{
//	template<typename TClass>
//	inline void registerType()
//	{
//		if (TypeRegistry::_instance == nullptr)
//		{
//			throw std::runtime_error("Error, Type Registry singleton has not been initialized yet!");
//		}
//
//		TypeRegistry::_instance->_typeRegistry.Insert(std::make_pair(TClass::TypeIdClass(), TClass::Signatures()));
//	};
//
//	template <typename TClass, typename TParentClass>
//	inline void registerParentedType()
//	{
//		auto child = TClass::TypeIdInstance();
//		auto parent = TParentClass::TypeIdInstance();
//
//		if (TypeRegistry::_instance == nullptr)
//		{
//			throw std::runtime_error("Error, Type Registry singleton has not been initialized yet!");
//		}
//
//		Vector<Signature> insertAttribute;
//
//		auto foundParent = TypeRegistry::_instance->_typeRegistry.Find(parent);
//		if (foundParent == TypeRegistry::_instance->_typeRegistry.end())
//		{
//			throw std::runtime_error("Error, associated Parent not registered in Type Registry yet!");
//		}
//		else
//		{
//			insertAttribute = (*foundParent).second;
//		}
//
//		for (auto value : TClass::Signature())
//		{
//			insertAttribute.PushBack(value);
//		}
//
//		TypeRegistry::_instance->_typeRegistry.Insert(std::make_pair(child, insertAttribute));
//	};
//}