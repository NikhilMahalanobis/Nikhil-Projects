#include "ActionDestroyAction.h"
#include "GameState.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionDestroyAction) 

	const Vector<Signature> ActionDestroyAction::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{ "InstanceNameToDestroy", DatumTypes::String, 1, offsetof(ActionDestroyAction, InstanceNameToDestroy) }
		};
	}

	ActionDestroyAction::ActionDestroyAction() :
		Action(ActionDestroyAction::TypeIdClass())
	{

	}

	ActionDestroyAction::ActionDestroyAction(RTTI::IdType type) :
		Action(type)
	{
	}

	void ActionDestroyAction::Update(GameState* gameState)
	{
		assert(GetParent() != nullptr);

		Datum& actions = GetParent()->operator[]("Actions"s);

		size_t counter = 0;
		for (size_t i = 0; i < actions.Size(); ++i)
		{
			Scope* scope = &(actions.GetTable(i));
			assert(scope->Is(Action::TypeIdClass()));
			Action* action = static_cast<Action*>(scope);

			if (action->GetName() == InstanceNameToDestroy)
			{
				gameState->AddDestroyAction(action);
				break;
			}
			++counter;
		}

		if (counter == actions.Size())
		{

		}
	}

	const std::string& ActionDestroyAction::GetInstanceName() const
	{
		return InstanceNameToDestroy;
	}
	void ActionDestroyAction::SetInstanceName(const std::string& instanceName)
	{
		InstanceNameToDestroy = instanceName;
	}
}