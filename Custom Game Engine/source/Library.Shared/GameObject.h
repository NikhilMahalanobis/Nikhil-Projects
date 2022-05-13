#pragma once

#include "GameClock.h"
#include "GameTime.h"
#include "Vector.h"
#include "Factory.h"
#include "Signature.h"
#include "Attribute.h"
#include "Action.h"

namespace FieaGameEngine
{
	class GameState;

	class GameObject : public Attribute
	{
		RTTI_DECLARATIONS(GameObject, Attribute)

	public:

		/// <summary>
		/// Default Constructor for GameObject
		/// </summary>
		GameObject();

		/// <summary>
		/// Compiler supplied Defaulted Copy Constructor for GameObject
		/// </summary>
		/// <param name="rhs"> Takes a const reference to a GameObject </param>
		GameObject(const GameObject& rhs) = default;

		/// <summary>
		/// Compiler supplied Defaulted Copy Assignment Operator for GameObject
		/// </summary>
		/// <param name="rhs"> Takes a const reference to a GameObject </param>
		/// <returns> Returns a reference to a GameObject </returns>
		GameObject& operator=(const GameObject& rhs) = default;

		/// <summary>
		/// Compiler supplied Defaulted Move Constructor for GameObject 
		/// </summary>
		/// <param name="rhs"> Takes a rvalue reference to a GameObject </param>
		GameObject(GameObject&& rhs) noexcept = default;

		/// <summary>
		/// Compiler supplied Defaulted Move Assignment Operator for GameObject
		/// </summary>
		/// <param name="rhs"> Takes a rvalue reference to a GameObject </param>
		/// <returns> Returns a lvalue reference to a GameObject </returns>
		GameObject& operator=(GameObject&& rhs) noexcept = default;

		/// <summary>
		/// Virtual Compiler supplied Defaulted Destructor for GameObject
		/// </summary>
		virtual ~GameObject() = default;

		/// <summary>
		/// Update method for GameObject
		/// </summary>
		/// <param name="deltaTime"> Takes a const reference to a GameTime </param>
		virtual void Update(GameState* gameState);

		/// <summary>
		/// Getter method for Child Game Objects
		/// </summary>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& GetChildren();

		/// <summary>
		/// Getter method for Actions of Game Object
		/// </summary>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& GetActions();

		/// <summary>
		/// CreateAction convenience method for Game Object 
		/// </summary>
		/// <param name="className"> Takes a const reference to a std::string </param>
		/// <param name="instanceName"> Takes a const reference to a std::string </param>
		/// <returns> Returns a pointer to an Action </returns>
		Action* CreateAction(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Getter method for Name for GameObjects
		/// </summary>
		/// <returns> Returns a const reference to a std::string </returns>
		const std::string& GetName() const;

		/// <summary>
		/// Setter method for Name for GameObjects
		/// </summary>
		/// <param name="name"> Takes a const reference to a std::string </param>
		void SetName(const std::string& name);

		/// <summary>
		/// Getter method for Tag for GameObjects
		/// </summary>
		/// <returns> Returns a const reference to a std::string </returns>
		const std::string& GetTag() const;

		/// <summary>
		/// Setter method for Tag for GameObjects
		/// </summary>
		/// <param name="tag"> Takes a const reference to a std::string </param>
		void SetTag(const std::string& tag);

		/// <summary>
		/// Getter method for Layer for GameObjects
		/// </summary>
		/// <returns> Returns a copy of an int </returns>
		int GetLayer() const;

		/// <summary>
		/// Setter method for Layer for GameObjects
		/// </summary>
		/// <param name="layer"> Takes a copy of an int </param>
		void SetLayer(int layer);

		/// <summary>
		/// Getter method for Transform for GameObjects
		/// </summary>
		/// <returns> Returns a const reference to a glms::mat4 </returns>
		const glm::mat4& GetTransform() const;

		/// <summary>
		/// Setter method for Transform for GameObjects
		/// </summary>
		/// <param name="transform"> Takes a const reference to a glm::mat4 </param>
		void SetTransform(const glm::mat4& transform);

		/// <summary>
		/// Getter method for Position for GameObjects
		/// </summary>
		/// <returns> Returns a const reference to a glm::vec4 </returns>
		const glm::vec4& GetPosition() const;

		/// <summary>
		/// Setter method for Position for GameObjects
		/// </summary>
		/// <param name="position"> Takes a const reference to a glm::vec4 </param>
		void SetPosition(const glm::vec4& position);

		/// <summary>
		/// Getter method for Rotation for GameObjects
		/// </summary>
		/// <returns> Returns a const reference to a glm::vec4 </returns>
		const glm::vec4& GetRotation() const;
		
		/// <summary>
		/// Setter method for Rotation for GameObjects 
		/// </summary>
		/// <param name="rotation"> Takes a const reference to a glm::vec4 </param>
		void SetRotation(const glm::vec4& rotation);

		/// <summary>
		/// Getter method for Scale for GameObjects
		/// </summary>
		/// <returns> Returns a const reference to a glm::vec4 </returns>
		const glm::vec4& GetScale() const;
		
		/// <summary>
		/// Setter method for Scale for GameObjects
		/// </summary>
		/// <param name="scale"> Takes a const reference to a glm::vec4 </param>
		void SetScale(const glm::vec4& scale);

		/// <summary>
		/// Clone method for GameObjects
		/// </summary>
		/// <returns> Returns a pointer to a GameObject </returns>
		virtual GameObject* Clone() const override;

		/// <summary>
		/// Equals method for GameObject
		/// </summary>
		/// <param name="rhs"> Takes a pointer to a const RTTI </param>
		/// <returns></returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// ToString method for GameObject
		/// </summary>
		/// <returns> Returns a copy of a std::string </returns>
		std::string ToString() const override;

		static const Vector<Signature> Signatures();

		std::string Name;
		std::string Tag{ std::string() };
		int Layer{ 0 };
		glm::mat4 Transform{ glm::vec4(0), glm::vec4(0), glm::vec4(0), glm::vec4(0) };
	protected:
		GameObject(RTTI::IdType type);
	};

	ConcreteFactory(GameObject, Scope)
}