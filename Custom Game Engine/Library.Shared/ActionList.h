#pragma once

#include "Action.h"

namespace FieaGameEngine
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)

	public:
		/// <summary>
		/// Default Constructor for ActionList 
		/// </summary>
		ActionList();

		/// <summary>
		/// Compiler supplied Defaulted Copy Constructor for ActionList
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an ActionList  </param>
		ActionList(const ActionList& rhs) = default;

		/// <summary>
		/// Compiler supplied Defaulted Copy Assignment Operator for ActionList
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an ActionList </param>
		/// <returns> Returns an lvalue reference to an ActionList </returns>
		ActionList& operator=(const ActionList& rhs) = default;

		/// <summary>
		/// Compiler supplied Defaulted Move Constructor for ActionList
		/// </summary>
		/// <param name="rhs"> Takes an rvalue reference to an Action </param>
		/// <returns></returns>
		ActionList(ActionList&& rhs) noexcept = default;

		/// <summary>
		/// Compiler supplied Defaulted Move Assignment Operator for ActionList 
		/// </summary>
		/// <param name="rhs"> Takes an rvalue reference to an Action </param>
		/// <returns> Returns an lvalue reference to an ActionList </returns>
		ActionList& operator=(ActionList&& rhs) noexcept = default;

		/// <summary>
		/// Compiler supplied Defaulted Virtual Destructor for ActionList 
		/// </summary>
		virtual ~ActionList() = default;

		/// <summary>
		/// Virtual Update method for ActionList 
		/// </summary>
		/// <param name="gameState"> Takes a pointer to a GameState </param>
		virtual void Update(GameState* gameState) override;
		 
		static const Vector<Signature> Signatures();
	protected:
		ActionList(RTTI::IdType type);
	};

	ConcreteFactory(ActionList, Scope)
}