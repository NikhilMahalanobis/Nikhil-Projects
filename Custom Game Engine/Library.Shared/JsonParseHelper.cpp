#include "JsonParseHelper.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonParseHelper)
	RTTI_DEFINITIONS(JsonParseHelper::SharedData)

	JsonParseCoordinator* JsonParseHelper::SharedData::GetJsonParseCoordinator() const
	{
		return _owningParseCoordinator;
	}

	void JsonParseHelper::SharedData::SetJsonParseCoordinator(JsonParseCoordinator& owner)
	{
		_owningParseCoordinator = &owner;
	}

	void JsonParseHelper::SharedData::IncrementDepth()
	{
		++_nestingDepth;
	}
	void JsonParseHelper::SharedData::DecrementDepth()
	{
		if (_nestingDepth == 0)
		{
			throw std::runtime_error("Error, Nesting Depth already 0!");
		}

		--_nestingDepth;
	}
	size_t JsonParseHelper::SharedData::Depth() const
	{
		return _nestingDepth;
	}

}