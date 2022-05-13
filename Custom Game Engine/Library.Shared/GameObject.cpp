#include "GameObject.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(GameObject)

	const Vector<Signature> GameObject::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{"Name", DatumTypes::String, 1, offsetof(GameObject, Name)},
			{ "Tag", DatumTypes::String, 1, offsetof(GameObject, Tag) },
			{ "Layer", DatumTypes::Integer, 1, offsetof(GameObject, Layer) },
			{"Transform", DatumTypes::Matrix, 1, offsetof(GameObject, Transform)},
			{ "Children", DatumTypes::Table, 0, 0},
			{"Actions", DatumTypes::Table, 0, 0}
		};
	}


	GameObject::GameObject() :
		Attribute(GameObject::TypeIdClass())
	{
	}

	GameObject::GameObject(RTTI::IdType type) :
		Attribute(type)
	{
	}

	void GameObject::Update(GameState* gameState)
	{
		auto& actionDatum = GetActions();
		for (size_t i = 0; i < actionDatum.Size(); ++i)
		{
			Scope* action = &(actionDatum.GetTable(i));
			assert(action->Is(Action::TypeIdClass()));
			Action* realActionPtr = static_cast<Action*>(action);
			realActionPtr->Update(gameState);
		}

		auto& childDatum = GetChildren();
		for (size_t i = 0; i < childDatum.Size(); ++i)
		{
			Scope* child = &(childDatum.GetTable(i));
			assert(child->Is(GameObject::TypeIdClass()));
			GameObject* gameObjChild = static_cast<GameObject*>(child);
			gameObjChild->Update(gameState);
		}
	}

	Datum& GameObject::GetChildren()
	{
		return At("Children"s);
	}

	Datum& GameObject::GetActions()
	{
		return At("Actions"s);
	}

	Action* GameObject::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Scope* newScope = Factory<Scope>::Create(className);
		assert(newScope != nullptr);

		Action* newAction = newScope->As<Action>();
		if (newAction == nullptr)
		{
			delete newScope;
			throw std::runtime_error("Error, cannot create Action class with a class not Action-derived!");
		}

		Adopt(*newAction, "Actions");
		newAction->SetName(instanceName);

		return newAction;

	}

	const std::string& GameObject::GetName() const
	{
		return Name;
	}

	void GameObject::SetName(const std::string& name)
	{
		Name = name;
	}

	const std::string& GameObject::GetTag() const
	{
		return Tag;
	}

	void GameObject::SetTag(const std::string& tag)
	{
		Tag = tag;
	}

	int GameObject::GetLayer() const
	{
		return Layer;
	}

	void GameObject::SetLayer(int layer)
	{
		Layer = layer;
	}

	const glm::mat4& GameObject::GetTransform() const
	{
		return Transform;
	}

	void GameObject::SetTransform(const glm::mat4& transform)
	{
		Transform = transform;
	}

	const glm::vec4& GameObject::GetPosition() const
	{
		return Transform[0];
	}

	void GameObject::SetPosition(const glm::vec4& position)
	{
		Transform[0] = position;
	}

	const glm::vec4& GameObject::GetRotation() const
	{
		return Transform[1];
	}

	void GameObject::SetRotation(const glm::vec4& rotation)
	{
		Transform[1] = rotation;
	}

	const glm::vec4& GameObject::GetScale() const
	{
		return Transform[2];
	}

	void GameObject::SetScale(const glm::vec4& scale)
	{
		Transform[2] = scale;
	}

	GameObject* GameObject::Clone() const
	{
		return new GameObject(*this);
	}
	bool GameObject::Equals(const RTTI* rhs) const
	{
		const auto rhsGameObject = rhs->As<GameObject>();

		if (rhsGameObject == nullptr)
		{
			return false;
		}

		return Name == rhsGameObject->Name &&
			Tag == rhsGameObject->Tag &&
			Layer == rhsGameObject->Layer &&
			Transform == rhsGameObject->Transform;				
	}
	std::string GameObject::ToString() const
	{
		return "GameObject"s;
	}
}