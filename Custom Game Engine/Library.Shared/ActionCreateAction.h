#pragma once

#include "Action.h"

namespace FieaGameEngine
{
	class ActionCreateAction final : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)

	public:
		
		/// <summary>
		/// Defualt Constructor for ActionCreateAction 
		/// </summary>
		ActionCreateAction();

		/// <summary>
		/// Compiler supplied Defaulted Copy Constructor for ActionCreateAction 
		/// </summary>
		/// <param name="other"> Takes a const reference to an ActionCreateAction </param>
		ActionCreateAction(const ActionCreateAction& other) = default;

		/// <summary>
		/// Compiler supplied Defaulted Copy Assignment Operator for ActionCreateAction 
		/// </summary>
		/// <param name="other"> Takes a const reference to an ActionCreateAction </param>
		/// <returns> Returns a lvalue reference to an ActionCreateAction </returns>
		ActionCreateAction& operator=(const ActionCreateAction& other) = default;
		
		/// <summary>
		/// Compiler supplied Defaulted Move Constructor for ActionCreateAction 
		/// </summary>
		/// <param name="other"> Takes a rvalue reference to an ActionCreateAction </param>
		ActionCreateAction(ActionCreateAction&& other) = default;

		/// <summary>
		/// Compiler supplied Defaulted Move Assignment Operator for ActionCreateAction 
		/// </summary>
		/// <param name="other"> Takes a rvalue reference to ActionCreateAction </param>
		/// <returns> Returns a lvalue reference to an ActionCreateAction </returns>
		ActionCreateAction& operator=(ActionCreateAction&& other) = default;

		/// <summary>
		/// Compiler Supplied Defaulted Destructor for ActionCreateAction
		/// </summary>
		~ActionCreateAction() = default;

		/// <summary>
		/// Update method for ActionCreateAction 
		/// </summary>
		/// <param name="gameState"> Takes a pointer to a GameState </param>
		void Update(GameState* gameState);

		/// <summary>
		/// ClassName getter for ActionCreateAction 
		/// </summary>
		/// <returns> Returns a const reference to a std::string </returns>
		const std::string& GetClassName() const;

		/// <summary>
		/// ClassName setter for ActionCreateAction 
		/// </summary>
		/// <param name="className"> Takes a const reference to a std::string </param>
		void SetClassName(const std::string& className);

		/// <summary>
		/// InstanceName getter method for ActionCreateAction 
		/// </summary>
		/// <returns> Returns a const reference to a std::string </returns>
		const std::string& GetInstanceName() const;

		/// <summary>
		/// InstanceName setter method for ActionCreateAction 
		/// </summary>
		/// <param name="instanceName"> Takes a const reference to a std::string </param>
		void SetInstanceName(const std::string& instanceName);

		static const Vector<Signature> Signatures();

		std::string ClassName{ std::string() };
		std::string InstanceName{ std::string() };
	protected:
		ActionCreateAction(RTTI::IdType type);
	};

	ConcreteFactory(ActionCreateAction, Scope)
}