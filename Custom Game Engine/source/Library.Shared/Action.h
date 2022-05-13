#pragma once

#include "Attribute.h"
#include "GameClock.h"
#include "GameTime.h"
#include "Signature.h"

namespace FieaGameEngine
{
	class GameState;
	
	class Action : public Attribute
	{

		RTTI_DECLARATIONS(Action, Attribute)

	public:

		/// <summary>
		/// Default Constructor for Action 
		/// </summary>
		Action();

		/// <summary>
		/// Compiler supplied defaulted Copy Constructor for Action
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an Action </param>
		Action(const Action& rhs) = default;

		/// <summary>
		/// Compiler supplied defaulted Copy Assignment Operator for Action
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an Action </param>
		/// <returns></returns>
		Action& operator=(const Action& rhs) = default;

		/// <summary>
		/// Compiler supplied defaulted Move Constructor for Action
		/// </summary>
		/// <param name="rhs"> Takes an rvalue reference to an Action </param>
		/// <returns> Returns an lvalue reference to an Action </returns>
		Action(Action&& rhs) noexcept = default;

		/// <summary>
		/// Compiler supplied defaulted Move Assignment Operator for Action 
		/// </summary>
		/// <param name="rhs"> Takes an rvalue reference to an Action </param>
		/// <returns> Returns an lvalue reference to an Action </returns>
		Action& operator=(Action&& rhs) noexcept = default;

		/// <summary>
		/// Compiler supplied defaulted virtual Destructor for Action
		/// </summary>
		virtual ~Action() = default;

		/// <summary>
		/// Pure Virtual Update method for Action 
		/// </summary>
		/// <param name="gameState"> Takes a pointer to a GameState </param>
		virtual void Update(GameState* gameState) = 0;

		/// <summary>
		/// Name Getter method for Action
		/// </summary>
		/// <returns> Returns a const reference to a std::string </returns>
		const std::string& GetName() const;

		/// <summary>
		/// Name Setter method for Action 
		/// </summary>
		/// <param name="name"> Takes a const reference to a std::string </param>
		void SetName(const std::string& name);

		static const Vector<Signature> Signatures();

		std::string Name;
		std::string Target;
	protected:
		Action(RTTI::IdType type);
	};
}