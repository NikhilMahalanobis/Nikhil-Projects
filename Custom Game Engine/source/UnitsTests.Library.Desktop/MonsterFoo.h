#pragma once
#include "AttributedFoo.h"

namespace UnitsTestsLibraryDesktop
{
	class MonsterFoo : public AttributedFoo
	{
		RTTI_DECLARATIONS(MonsterFoo, AttributedFoo)

	public:
		MonsterFoo();
		MonsterFoo(const MonsterFoo&) = default;
		MonsterFoo(MonsterFoo&&) = default;
		MonsterFoo& operator=(const MonsterFoo&) = default;
		MonsterFoo& operator=(MonsterFoo&&) = default;
		~MonsterFoo() = default;

		int MonsterExternalInteger = 0;

		MonsterFoo* Clone() const override;
		bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override;

		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	};
}