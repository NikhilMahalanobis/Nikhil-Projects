#include "pch.h"
#include "MonsterFoo.h"

namespace UnitsTestsLibraryDesktop
{
	RTTI_DEFINITIONS(MonsterFoo)
	
	MonsterFoo::MonsterFoo() :
	AttributedFoo(MonsterFoo::TypeIdClass())
	{

	}

	const FieaGameEngine::Vector<FieaGameEngine::Signature> MonsterFoo::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{"MonsterExternalInteger", FieaGameEngine::DatumTypes::Integer, 1, offsetof(MonsterFoo, MonsterExternalInteger)}
		};
	}
	MonsterFoo* MonsterFoo::Clone() const
	{
		return new MonsterFoo(*this);
	}
	bool MonsterFoo::Equals(const RTTI* rhs) const
	{
		const auto rhsFoo = rhs->As<MonsterFoo>();
		if (rhsFoo == nullptr)
		{
			return false;
		}

		return MonsterExternalInteger == rhsFoo->MonsterExternalInteger;
	}
	std::string MonsterFoo::ToString() const
	{
		return "MonsterFoo"s;
	}
}