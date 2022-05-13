#include "pch.h"
#include "Avatar.h"

namespace UnitsTestsLibraryDesktop
{
	RTTI_DEFINITIONS(Avatar)

	const FieaGameEngine::Vector<FieaGameEngine::Signature> Avatar::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{"IncrementInteger", FieaGameEngine::DatumTypes::Integer, 1, offsetof(Avatar, IncrementInteger)}
		};
	}

	Avatar::Avatar() :
	FieaGameEngine::GameObject(Avatar::TypeIdClass())
	{

	}

	Avatar::Avatar(FieaGameEngine::RTTI::IdType type) :
		FieaGameEngine::GameObject(type)
	{
	}

	void Avatar::Update(FieaGameEngine::GameState* gameState)
	{
		FieaGameEngine::GameObject::Update(gameState);

		IncrementInteger += 5;
	}
	Avatar* Avatar::Clone() const
	{
		return new Avatar(*this);
	}
	bool Avatar::Equals(const RTTI* rhs) const
	{
		const auto rhsGameObject = rhs->As<Avatar>();

		if (rhsGameObject == nullptr)
		{
			return false;
		}

		return FieaGameEngine::GameObject::Equals(rhs) && IncrementInteger == rhsGameObject->IncrementInteger;
	}
	std::string Avatar::ToString() const
	{
		return "Avatar"s;
	}
}