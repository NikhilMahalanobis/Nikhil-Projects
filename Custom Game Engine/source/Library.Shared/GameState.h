#pragma once

#include "GameTime.h"
#include "Vector.h"
#include "Scope.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class GameObject;
	class Action;

	class GameState
	{
	public:
		struct CreateAction
		{
			std::string _className{ std::string() };
			std::string _instanceName{ std::string() };
			std::string _targetName{ std::string() };
			Scope* _currentContext{ nullptr };

			CreateAction(const std::string& className, const std::string& instanceName, const std::string& targetName, Scope* parentContext) :
				_className(className), _instanceName(instanceName), _targetName(targetName), _currentContext(parentContext)
			{};
		};

		/// <summary>
		/// Compiler supplied Defaulted Default Constructor for GameState
		/// </summary>
		GameState() = default;

		/// <summary>
		/// Compiler supplied Defaulted Copy Constructor for GameState 
		/// </summary>
		/// <param name="rhs"> Takes a const reference to a GameState </param>
		GameState(const GameState& rhs) = default;

		/// <summary>
		/// Compiler supplied Defaulted Copy Assignment Operator for GameState
		/// </summary>
		/// <param name="rhs"> Takes a const reference to a GameState </param>
		/// <returns> Returns a lvalue reference to a GameState </returns>
		GameState& operator=(const GameState& rhs) = default;

		/// <summary>
		/// Compiler supplied Defaulted Move Constructor for GameState
		/// </summary>
		/// <param name="rhs"> Takes a rvalue reference to a GameState </param>
		/// <returns></returns>
		GameState(GameState&& rhs) noexcept = default;

		/// <summary>
		/// Compiler supplied Defaulted Move Assignment Operator for GameState
		/// </summary>
		/// <param name="rhs"> Takes a rvalue reference to a GameState </param>
		/// <returns> Returns a lvalue reference to a GameState </returns>
		GameState& operator=(GameState&& rhs) noexcept = default;

		/// <summary>
		/// Compiler supplied Defaulted Virtual Destructor for GameState
		/// </summary>
		virtual ~GameState() = default;

		/// <summary>
		/// Update method for updating the Create & Destroy queue
		/// </summary>
		void UpdateQueues();

		/// <summary>
		/// AddCreateAction method for GameState, adds a CreateAction to the queue
		/// </summary>
		/// <param name="className"> Take a const reference to a std::string </param>
		/// <param name="instanceName"> Take a const reference to a std::string </param>
		/// <param name="targetName"> Take a const reference to a std::string </param>
		/// <param name="parentContext"> Takes a pointer to a Scope </param>
		void AddCreateAction(const std::string& className, const std::string& instanceName, const std::string& targetName, Scope* parentContext);
		
		/// <summary>
		/// AddDestroyActoin method for GameState, adds Action* to queue 
		/// </summary>
		/// <param name="actionToDestroy"> Takes a pointer to an Action </param>
		void AddDestroyAction(Action* actionToDestroy);

		/// <summary>
		/// ProcessCreateActionsQueue method for GameState 
		/// </summary>
		void ProcessCreateActionsQueue();

		/// <summary>
		/// ProcessDestroyActionQueues method for GameState
		/// </summary>
		void ProcessDestroyActionsQueue();

		GameTime _wallTime;

	private:
		Vector<CreateAction> createActionQueue;

		Vector<Action*> destroyActionQueue;
	};
}