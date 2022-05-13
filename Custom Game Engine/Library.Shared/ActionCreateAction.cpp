#include "ActionCreateAction.h"
#include "GameState.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionCreateAction)

	const Vector<Signature> ActionCreateAction::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{"ClassName", DatumTypes::String, 1, offsetof(ActionCreateAction, ClassName)},
			{ "InstanceName", DatumTypes::String, 1, offsetof(ActionCreateAction, InstanceName)}		
		};
	}

	ActionCreateAction::ActionCreateAction(RTTI::IdType type) :
		Action(type)
	{
	}

	ActionCreateAction::ActionCreateAction() :
		Action(ActionCreateAction::TypeIdClass())
	{
	}

	void ActionCreateAction::Update(GameState* gameState)
	{
		assert(GetParent() != nullptr);

		Datum& actions = GetParent()->operator[]("Actions"s);
		
		bool alreadyInserted = false;
		for (size_t i = 0; i < actions.Size(); ++i)
		{
			Scope* scope = &(actions.GetTable(i));
			assert(scope->Is(Action::TypeIdClass()));
			Action* action = static_cast<Action*>(scope);

			if (action->GetName() == InstanceName)
			{
				alreadyInserted = true;
				break;
			}
		}

		if (!alreadyInserted)
		{
			gameState->AddCreateAction(ClassName, InstanceName, Target, GetParent());
		}
	}
	const std::string& ActionCreateAction::GetClassName() const
	{
		return ClassName;
	}
	void ActionCreateAction::SetClassName(const std::string& className)
	{
		ClassName = className;
	}
	const std::string& ActionCreateAction::GetInstanceName() const
	{
		return InstanceName;
	}
	void ActionCreateAction::SetInstanceName(const std::string& instanceName)
	{
		InstanceName = instanceName;
	}
}