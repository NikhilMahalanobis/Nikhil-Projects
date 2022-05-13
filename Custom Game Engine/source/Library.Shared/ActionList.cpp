#include "ActionList.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionList)

	const Vector<Signature> ActionList::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{"Actions", DatumTypes::Table, 0, 0}
		};
	}

	ActionList::ActionList() :
		Action(ActionList::TypeIdClass())
	{

	}

	ActionList::ActionList(RTTI::IdType type) :
		Action(type)
	{
	}

	void ActionList::Update(GameState* gameState)
	{
		auto& actionDatum = At("Actions"s);
		for (size_t i = 0; i < actionDatum.Size(); ++i)
		{
			Scope* action = &(actionDatum.GetTable(i));
			assert(action->Is(Action::TypeIdClass()));
			Action* realActionPtr = static_cast<Action*>(action);
			realActionPtr->Update(gameState);
		}
	}

}