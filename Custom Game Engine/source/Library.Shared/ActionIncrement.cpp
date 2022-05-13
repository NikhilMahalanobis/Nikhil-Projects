#include "ActionIncrement.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionIncrement)

	const Vector<Signature> ActionIncrement::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{"Step", DatumTypes::Float, 1, offsetof(ActionIncrement, Step)},
			{ "Target", DatumTypes::String, 1, offsetof(ActionIncrement, Target) }
		};
	}

	ActionIncrement::ActionIncrement() :
		Action(ActionIncrement::TypeIdClass())
	{
	}

	ActionIncrement::ActionIncrement(RTTI::IdType type) :
		Action(type)
	{
	}

	void ActionIncrement::Update(GameState* gameState)
	{
		gameState;

		Scope* foundScope;
		Datum* foundDatum = Search(Target, foundScope);

		if (foundDatum == nullptr)
		{
			throw std::runtime_error("Error, could not find the given Target to Increment!");
		}

		float stepValue = foundDatum->GetFloat();
		foundDatum->Set(stepValue + Step);
	}
	void ActionIncrement::SetStep(float step)
	{
		Step = step;
	}
	float ActionIncrement::GetStep() const
	{
		return Step;
	}
	void ActionIncrement::SetTarget(const std::string& name)
	{
		Target = name;
	}
	const std::string& ActionIncrement::GetTarget() const
	{
		return Target;
	}
}