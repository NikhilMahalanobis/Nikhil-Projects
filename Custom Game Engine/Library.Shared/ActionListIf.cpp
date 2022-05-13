#include "ActionListIf.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionListIf)

	const Vector<Signature> ActionListIf::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{"Condition", DatumTypes::Integer, 1, offsetof(ActionListIf, Condition)},
			{"Then", DatumTypes::Table, 0, 0},
			{"Else", DatumTypes::Table, 0, 0}
		};
	}

	ActionListIf::ActionListIf() :
		ActionList(ActionListIf::TypeIdClass())
	{
	}

	ActionListIf::ActionListIf(RTTI::IdType type) :
		ActionList(type)
	{
	}

	void ActionListIf::Update(GameState* gameState)
	{
		if (Condition)
		{
			Datum& thenBlock = At("Then"s);
			for (size_t i = 0; i < thenBlock.Size(); ++i)
			{
				Scope* action = &(thenBlock.GetTable(i));
				assert(action->Is(Action::TypeIdClass()));
				Action* realActionPtr = static_cast<Action*>(action);
				realActionPtr->Update(gameState);
			}
		}

		else
		{
			Datum& elseBlock = At("Else"s);
			for (size_t i = 0; i < elseBlock.Size(); ++i)
			{
				Scope* action = &(elseBlock.GetTable(i));
				assert(action->Is(Action::TypeIdClass()));
				Action* realActionPtr = static_cast<Action*>(action);
				realActionPtr->Update(gameState);
			}
		}
	}
	void ActionListIf::SetCondition(int condition)
	{
		Condition = condition;
	}
	int ActionListIf::GetCondition() const
	{
		return Condition;
	}
}