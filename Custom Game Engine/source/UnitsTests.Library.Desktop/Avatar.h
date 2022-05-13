#pragma once

#include "GameObject.h"

namespace UnitsTestsLibraryDesktop
{
	class Avatar : public FieaGameEngine::GameObject
	{
		RTTI_DECLARATIONS (Avatar, FieaGameEngine::GameObject)

	public:
		Avatar();

		~Avatar() = default;

		int IncrementInteger = 10;

		virtual void Update(FieaGameEngine::GameState* gameState);

		virtual Avatar* Clone() const override;

		bool Equals(const RTTI* rhs) const override;

		std::string ToString() const override;

		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	protected:
		Avatar(FieaGameEngine::RTTI::IdType type);
	};

	ConcreteFactory(Avatar, FieaGameEngine::Scope)
}