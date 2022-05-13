#include "GameState.h"
#include "Action.h"

namespace FieaGameEngine
{
	void GameState::UpdateQueues()
	{
		ProcessCreateActionsQueue();
		ProcessDestroyActionsQueue();
	}

	void GameState::AddCreateAction(const std::string& className, const std::string& instanceName, const std::string& targetName, Scope* parentContext)
	{
		createActionQueue.PushBack(CreateAction(className, instanceName, targetName, parentContext));
	}

	void GameState::AddDestroyAction(Action* actionToDestroy)
	{
		destroyActionQueue.PushBack(actionToDestroy);
	}

	void GameState::ProcessCreateActionsQueue()
	{
		for (auto& createRequests : createActionQueue)
		{
			Scope* newScope = Factory<Scope>::Create(createRequests._className);
			assert(newScope != nullptr);
			
			Action* newAction = newScope->As<Action>();
			if (newAction == nullptr)
			{
				delete newScope;
				throw std::runtime_error("Error, cannot create Action w/ non derived Action class!");
			}

			newAction->SetName(createRequests._instanceName);
			newAction->Target = createRequests._targetName;
			createRequests._currentContext->Adopt(*newAction, "Actions"s);
		}

		createActionQueue.Clear();
	}

	void GameState::ProcessDestroyActionsQueue()
	{
		for (Action* action : destroyActionQueue)
		{
			delete action;
		}

		destroyActionQueue.Clear();
	}

}