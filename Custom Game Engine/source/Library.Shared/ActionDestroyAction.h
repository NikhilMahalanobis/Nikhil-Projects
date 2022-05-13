#pragma once

#include "Action.h"

namespace FieaGameEngine
{
	class ActionDestroyAction final : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action)

	public:
		/// <summary>
		/// Default Constructor for ActionDestroyAction
		/// </summary>
		ActionDestroyAction();

		/// <summary>
		/// Defaulted Compiler supplied Copy Constructor for ActionDestroyAction
		/// </summary>
		/// <param name="other"> Takes a const reference to an ActionDestroyAction </param>
		ActionDestroyAction(const ActionDestroyAction& other) = default;

		/// <summary>
		/// Defaulted Compiler supplied Copy Assignment Operator for ActionDestroyAction
		/// </summary>
		/// <param name="other"> Takes a const reference to a ActionDestroyAction </param>
		/// <returns> Returns a lvalue reference to an ActionDestroyAction </returns>
		ActionDestroyAction& operator=(const ActionDestroyAction & other) = default;

		/// <summary>
		/// Defaulted Compiler supplied Move Constructor for ActionDestroyAction
		/// </summary>
		/// <param name="other"> Takes a rvalue reference to an ActionDestroyAction </param>
		ActionDestroyAction(ActionDestroyAction && other) = default;

		/// <summary>
		/// Defaulted Compiler supplied Move Assignment Operator for ActionDestroyAction
		/// </summary>
		/// <param name="other"> Takes a rvalue reference to a ActionDestroyAction </param>
		/// <returns> Returns a lvalue reference to an ActionDestroyAction </returns>
		ActionDestroyAction& operator=(ActionDestroyAction && other) = default;

		/// <summary>
		/// Defualted Compiler supplied Destructor for ActionDestroyAction
		/// </summary>
		~ActionDestroyAction() = default;

		/// <summary>
		/// Update method for ActionDestroyAction
		/// </summary>
		/// <param name="gameState"> Takes a pointer to a GameState </param>
		void Update(GameState* gameState);

		/// <summary>
		/// InstanceName getter for ActionDestroyAction
		/// </summary>
		/// <returns> Returns const reference to a std::string </returns>
		const std::string& GetInstanceName() const;

		/// <summary>
		/// InstanceName setter for ActionDestroyAction
		/// </summary>
		/// <param name="instanceName"> Takes a const reference to a std::string </param>
		void SetInstanceName(const std::string & instanceName);

		static const Vector<Signature> Signatures();

		std::string InstanceNameToDestroy{ std::string() };
	protected:
		ActionDestroyAction(RTTI::IdType type);
	};

	ConcreteFactory(ActionDestroyAction, Scope)
}