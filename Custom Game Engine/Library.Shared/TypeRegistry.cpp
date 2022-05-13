#include "TypeRegistry.h"

namespace FieaGameEngine
{
	TypeRegistry* TypeRegistry::_instance = nullptr;

	TypeRegistry* TypeRegistry::getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new TypeRegistry;
		}

		return _instance;
	}

	typename HashMap<RTTI::IdType, Vector<Signature>>::PairType& TypeRegistry::Find(RTTI::IdType typeId)
	{
		if (_instance == nullptr)
		{
			throw std::runtime_error("Error, Type Registry not instantiated yet!");
		}

		auto foundResult = _instance->_typeRegistry.Find(typeId);
		return *foundResult;
	}
	void TypeRegistry::Clear()
	{
		_instance->_typeRegistry.Clear();
	}
}
