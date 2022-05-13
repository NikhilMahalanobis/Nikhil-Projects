#pragma once

#include "Action.h"

namespace FieaGameEngine
{
	class ActionIncrement : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action)

	public:

		/// <summary>
		/// Default Constructor for ActionIncrement
		/// </summary>
		ActionIncrement();

		/// <summary>
		/// Compiler supplied Defaulted Copy Constructor for ActionIncrement
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an ActionIncrement </param>
		ActionIncrement(const ActionIncrement& rhs) = default;

		/// <summary>
		/// Compiler supplied Defaulted Copy Assignment Operator for ActionIncrement
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an ActionIncrement </param>
		/// <returns> Returns an lvalue reference to an ActionIncrement </returns>
		ActionIncrement& operator=(const ActionIncrement& rhs) = default;
		
		/// <summary>
		/// Compiler supplied Defaulted Move Constructor for ActionIncrement
		/// </summary>
		/// <param name="rhs"> Takes an rvalue reference to an Action </param>
		/// <returns></returns>
		ActionIncrement(ActionIncrement&& rhs) noexcept = default;

		/// <summary>
		/// Compiler supplied Defaulted Move Assignment operator for ActionIncrement
		/// </summary>
		/// <param name="rhs"> Takes an rvalue reference to an Action </param>
		/// <returns> Returns an lvalue reference to an ActionIncrement </returns>
		ActionIncrement& operator=(ActionIncrement&& rhs) noexcept = default;

		/// <summary>
		/// Compiler supplied Defaulted Virtual Destructor for ActionIncrement
		/// </summary>
		virtual ~ActionIncrement() = default;

		/// <summary>
		/// Virtual Update method for ActionIncrement
		/// </summary>
		/// <param name="gameState"> Takes a pointer to a GameState </param>
		virtual void Update(GameState* gameState) override;

		/// <summary>
		/// Step Setter method for ActionIncrement
		/// </summary>
		/// <param name="step"> Takes a copy of a float </param>
		void SetStep(float step);

		/// <summary>
		/// Step Getter method for ActionIncrement
		/// </summary>
		/// <returns> Returns a copy of a float </returns>
		float GetStep() const;

		/// <summary>
		/// Target Setter method for ActionIncrement
		/// </summary>
		/// <param name="name"> Takes a const reference to a std::string </param>
		void SetTarget(const std::string& name);

		/// <summary>
		/// Target Getter method for ActionIncrement
		/// </summary>
		/// <returns> Returns a const reference to a std::string </returns>
		const std::string& GetTarget() const;

		static const Vector<Signature> Signatures();

		float Step{ 1.0f };

	protected:
		ActionIncrement(RTTI::IdType type);
	};

	ConcreteFactory(ActionIncrement, Scope)
}