#pragma once

#include "Action.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action)

	const Vector<Signature> Action::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{"Name", DatumTypes::String, 1, offsetof(Action, Name)},
			{"Target", DatumTypes::String, 1, offsetof(Action, Target)}
		};
	}
	
	Action::Action() :
		Attribute(Action::TypeIdClass())
	{

	}

	Action::Action(RTTI::IdType type) :
		Attribute(type)
	{
	}
	
	const std::string& Action::GetName() const
	{
		return Name;
	}
	void Action::SetName(const std::string& name)
	{
		Name = name;
	}
}