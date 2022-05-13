#pragma once

#include "ActionList.h"

namespace FieaGameEngine
{
	class ActionListIf : public ActionList
	{
		RTTI_DECLARATIONS(ActionListIf, ActionList)

	public:

		/// <summary>
		/// Default Constructor for ActionListIf
		/// </summary>
		ActionListIf();

		/// <summary>
		/// Compiler supplied Defaulted Copy Constructor for ActionListIf
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an ActionListIf </param>
		ActionListIf(const ActionListIf& rhs) = default;

		/// <summary>
		/// Compiler supplied Defaulted Copy Assignment Operator for ActionListIf 
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an ActionListIf </param>
		/// <returns> Returns an lvalue reference to an ActionListIf </returns>
		ActionListIf& operator=(const ActionListIf& rhs) = default;

		/// <summary>
		/// Compiler supplied Defaulted Move Constructor for ActionListIf
		/// </summary>
		/// <param name="rhs"> Takes a rvalue reference to an ActionListIf </param>
		/// <returns></returns>
		ActionListIf(ActionListIf&& rhs) noexcept = default;

		/// <summary>
		/// Compiler supplied Defaulted Move Assignment Operator for ActionListIf
		/// </summary>
		/// <param name="rhs"> Takes a rvalue reference to an ActionListIf </param>
		/// <returns> Returns a lvalue reference to an ActionListIf </returns>
		ActionListIf& operator=(ActionListIf&& rhs) noexcept = default;

		/// <summary>
		/// Compiler supplied Defaulted Virtual Destructor for ActionListIf 
		/// </summary>
		virtual ~ActionListIf() = default;

		/// <summary>
		/// Virtual Update method for ActionListIf 
		/// </summary>
		/// <param name="gameState"> Takes a pointer to a GameState </param>
		virtual void Update(GameState* gameState) override;

		/// <summary>
		/// Condition Setter method for ActionListIf 
		/// </summary>
		/// <param name="condition"> Takes a copy of an int </param>
		void SetCondition(int condition);

		/// <summary>
		/// Condition Getter method for ActionListIf 
		/// </summary>
		/// <returns> Returns a copy of an int </returns>
		int GetCondition() const;

		static const Vector<Signature> Signatures();

		int Condition;

	protected:
		ActionListIf(RTTI::IdType type);
	};

	ConcreteFactory(ActionListIf, Scope)
}