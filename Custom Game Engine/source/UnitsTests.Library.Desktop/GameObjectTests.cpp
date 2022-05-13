#include "pch.h"
#include "GameObject.h"
#include "ToStringSpecializations.h"
#include "Avatar.h"
#include "GameState.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitsTestsLibraryDesktop
{
	TEST_CLASS(GameObjectTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(RTTITest)
		{
			GameObject a;
			RTTI* rtti = &a;

			Assert::IsTrue(rtti->Is("GameObject"s));
			Assert::IsTrue(rtti->Is("Attribute"s));
			Assert::IsFalse(rtti->Is("AttributedFoo"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsTrue(rtti->Is(GameObject::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attribute::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(GameObject::TypeIdClass(), rtti->TypeIdInstance());

			GameObject* f = rtti->As<GameObject>();
			Assert::IsNotNull(f);
			Assert::AreEqual(&a, f);
		}

		TEST_METHOD(GameObjectAttributes)
		{
			GameObject gameobject;

			Assert::IsTrue(gameobject.IsAttribute("this"));
			Assert::IsTrue(gameobject.IsAttribute("Name"));
			Assert::IsTrue(gameobject.IsAttribute("Tag"));
			Assert::IsTrue(gameobject.IsAttribute("Layer"));
			Assert::IsTrue(gameobject.IsAttribute("Transform"));
			Assert::IsTrue(gameobject.IsAttribute("Children"));

			Assert::IsTrue(gameobject.IsPrescribedAttribute("this"));
			Assert::IsTrue(gameobject.IsPrescribedAttribute("Name"));
			Assert::IsTrue(gameobject.IsPrescribedAttribute("Tag"));
			Assert::IsTrue(gameobject.IsPrescribedAttribute("Layer"));
			Assert::IsTrue(gameobject.IsPrescribedAttribute("Transform"));
			Assert::IsTrue(gameobject.IsPrescribedAttribute("Children"));

			Assert::IsFalse(gameobject.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(gameobject.IsAuxiliaryAttribute("Name"));
			Assert::IsFalse(gameobject.IsAuxiliaryAttribute("Tag"));
			Assert::IsFalse(gameobject.IsAuxiliaryAttribute("Layer"));
			Assert::IsFalse(gameobject.IsAuxiliaryAttribute("Transform"));
			Assert::IsFalse(gameobject.IsAuxiliaryAttribute("Children"));

			Assert::AreEqual(&gameobject, gameobject["this"].GetPointer()->As<GameObject>());
		}

		TEST_METHOD(GameObjectAuxiliaryAttributes)
		{
			GameObject gameobject;

			gameobject.AppendAuxiliaryAttribute("AuxiliaryInteger");
			Assert::IsTrue(gameobject.IsAttribute("AuxiliaryInteger"));
			Assert::IsTrue(gameobject.IsAuxiliaryAttribute("AuxiliaryInteger"));
			Assert::IsFalse(gameobject.IsPrescribedAttribute("AuxiliaryInteger"));

			Assert::ExpectException<std::runtime_error>([&gameobject] {gameobject.AppendAuxiliaryAttribute("Name"); });
		}

		TEST_METHOD(GameObjectExternalAssignment)
		{
			GameObject gameobject;

			Datum* datum = gameobject.Find("Layer");
			Assert::AreEqual(size_t(1), datum->Size());
			Assert::AreSame(datum->GetInt(), gameobject.Layer);

			int a = 5;
			datum->Set(a);
			Assert::AreEqual(a, datum->GetInt());
			Assert::AreEqual(a, gameobject.Layer);

			string b = "New Name";
			gameobject["Name"] = b;
			Assert::AreEqual(b, gameobject.Name);
		}

		TEST_METHOD(GameObjectCopySemantics)
		{
			{
				GameObject gameobject;
				GameObject anotherGameobject = gameobject;

				Assert::IsTrue(anotherGameobject.IsPrescribedAttribute("this"));
				Assert::IsTrue(anotherGameobject.IsPrescribedAttribute("Name"));
				Assert::IsTrue(anotherGameobject.IsPrescribedAttribute("Tag"));
				Assert::IsTrue(anotherGameobject.IsPrescribedAttribute("Layer"));
				Assert::IsTrue(anotherGameobject.IsPrescribedAttribute("Transform"));
				Assert::IsTrue(anotherGameobject.IsPrescribedAttribute("Children"));

				Assert::AreEqual(&anotherGameobject, anotherGameobject["this"].GetPointer()->As<GameObject>());

				const int& gameobjectData = gameobject["Layer"].GetInt();
				const int& anotherGameobjectData = anotherGameobject["Layer"].GetInt();
				Assert::AreNotSame(anotherGameobjectData, gameobjectData);
			}
			{
				GameObject gameobject;
				GameObject anotherGameobject;
				anotherGameobject = gameobject;

				Assert::IsTrue(anotherGameobject.IsPrescribedAttribute("this"));
				Assert::IsTrue(anotherGameobject.IsPrescribedAttribute("Name"));
				Assert::IsTrue(anotherGameobject.IsPrescribedAttribute("Tag"));
				Assert::IsTrue(anotherGameobject.IsPrescribedAttribute("Layer"));
				Assert::IsTrue(anotherGameobject.IsPrescribedAttribute("Transform"));
				Assert::IsTrue(anotherGameobject.IsPrescribedAttribute("Children"));

				Assert::AreEqual(&anotherGameobject, anotherGameobject["this"].GetPointer()->As<GameObject>());

				const int& gameobjectData = gameobject["Layer"].GetInt();
				const int& anotherGameobjectData = anotherGameobject["Layer"].GetInt();
				Assert::AreNotSame(anotherGameobjectData, gameobjectData);
			}
		}

		TEST_METHOD(GameObjectMoveSemantics)
		{
			{
				GameObject gameobject;

				size_t ogSize = gameobject.Size();
				Assert::AreNotEqual(size_t(0), ogSize);
				Assert::AreNotEqual(size_t(1), ogSize);

				gameobject.Name = "New Name";
				gameobject.Tag = "Tag";
				gameobject.Layer = 100;
				gameobject.Transform = glm::mat4(1.0f);

				char helloWorld[9];
				strcpy_s(helloWorld, gameobject.Name.c_str());
				string helloWorldString = helloWorld;
				Assert::AreEqual(helloWorldString, gameobject.Name);

				GameObject anotherGameobject = std::move(gameobject);
				Assert::AreEqual(ogSize, anotherGameobject.Size());
				Assert::AreEqual(size_t(0), gameobject.Size());
				Assert::AreEqual("Tag"s, anotherGameobject.Tag);
				Assert::AreEqual(helloWorldString, anotherGameobject.Name);
				Assert::AreNotSame(helloWorldString, anotherGameobject.Name);
				Assert::IsTrue(gameobject.Name.empty());
				Assert::AreEqual(anotherGameobject.Layer, 100);
				Assert::AreEqual(anotherGameobject.Transform, glm::mat4(1.0f));

				Assert::AreEqual(&anotherGameobject, anotherGameobject["this"].GetPointer()->As<GameObject>());
			}

			{
				GameObject gameobject;

				size_t ogSize = gameobject.Size();
				Assert::AreNotEqual(size_t(0), ogSize);
				Assert::AreNotEqual(size_t(1), ogSize);

				gameobject.Name = "New Name";
				gameobject.Tag = "Tag";
				gameobject.Layer = 100;
				gameobject.Transform = glm::mat4(1.0f);

				char helloWorld[9];
				strcpy_s(helloWorld, gameobject.Name.c_str());
				string helloWorldString = helloWorld;
				Assert::AreEqual(helloWorldString, gameobject.Name);

				GameObject anotherGameobject;
				anotherGameobject = std::move(gameobject);
				Assert::AreEqual(ogSize, anotherGameobject.Size());
				Assert::AreEqual(size_t(0), gameobject.Size());
				Assert::AreEqual("Tag"s, anotherGameobject.GetTag());
				Assert::AreEqual(helloWorldString, anotherGameobject.GetName());
				Assert::AreNotSame(helloWorldString, anotherGameobject.Name);
				Assert::IsTrue(gameobject.Name.empty());
				Assert::AreEqual(anotherGameobject.GetLayer(), 100);
				Assert::AreEqual(anotherGameobject.GetTransform(), glm::mat4(1.0f));
				Assert::AreEqual(&anotherGameobject, anotherGameobject["this"].GetPointer()->As<GameObject>());
			}
		}

		TEST_METHOD(ChildAttributes)
		{
			GameObject gameobject;
			Datum& children = gameobject.At("Children"s);
			Assert::AreEqual(size_t(0), children.Size());
		}

		TEST_METHOD(GameObjectClone)
		{
			GameObject gameobject;
			auto clone = gameobject.Clone();
			auto anotherGameobject = clone->As<GameObject>();
			Assert::IsNotNull(anotherGameobject);

			Assert::IsTrue(anotherGameobject->IsPrescribedAttribute("this"));
			Assert::IsTrue(anotherGameobject->IsPrescribedAttribute("Name"));
			Assert::IsTrue(anotherGameobject->IsPrescribedAttribute("Tag"));
			Assert::IsTrue(anotherGameobject->IsPrescribedAttribute("Layer"));
			Assert::IsTrue(anotherGameobject->IsPrescribedAttribute("Transform"));
			Assert::IsTrue(anotherGameobject->IsPrescribedAttribute("Children"));

			Assert::AreEqual(anotherGameobject, (*anotherGameobject)["this"s].GetPointer()->As<GameObject>());

			const int& gameobjectData = gameobject["Layer"].GetInt();
			const int& anotherGameobjectData = (*anotherGameobject)["Layer"s].GetInt();
			Assert::AreNotSame(anotherGameobjectData, gameobjectData);

			Assert::AreEqual(gameobject, *anotherGameobject);

			delete clone;
		}

		TEST_METHOD(GameObjectParsingTest)
		{
			const std::string filename = R"(Content\TestGameObject.json)";

			GameObject rootGameObject;

			JsonParseCoordinator parseMaster(std::make_shared<JsonTableParseHelper::SharedData>(rootGameObject));
			parseMaster.AddHelper(std::make_shared<JsonTableParseHelper>());

			ScopeFactory scopeFactory;
			GameObjectFactory gameObjFactory;
			AvatarFactory avatarFactory;

			parseMaster.DeserializeObjectFromFile(filename);

			Assert::AreEqual(size_t(7), rootGameObject.Size());
			Assert::AreEqual("Scene"s, rootGameObject["Name"s].GetString());
			Assert::AreEqual("root"s, rootGameObject["Tag"s].GetString());
			Assert::AreEqual(0, rootGameObject["Layer"s].GetInt());
			Assert::AreEqual(glm::mat4(1.0f), rootGameObject["Transform"s].GetMatrix());
			Assert::AreEqual(size_t(1), rootGameObject["Children"s].Size());

			GameTime gametime;
			GameState gameState;
			gameState._wallTime = gametime;

			Scope& child = rootGameObject["Children"s].GetTable();
			Assert::AreEqual(size_t(8), child.Size());
			Assert::AreEqual(500, child["IncrementInteger"s].GetInt());
			rootGameObject.Update(&gameState);
			Assert::AreEqual(505, child["IncrementInteger"s].GetInt());
			rootGameObject.Update(&gameState);
			Assert::AreEqual(510, child["IncrementInteger"s].GetInt());
			rootGameObject.Update(&gameState);
			Assert::AreEqual(515, child["IncrementInteger"s].GetInt());
		}

		TEST_METHOD(GettersAndSetters)
		{
			const std::string filename = R"(Content\TestGameObject.json)";

			GameObject rootGameObject;

			JsonParseCoordinator parseMaster(std::make_shared<JsonTableParseHelper::SharedData>(rootGameObject));
			parseMaster.AddHelper(std::make_shared<JsonTableParseHelper>());

			ScopeFactory scopeFactory;
			GameObjectFactory gameObjFactory;
			AvatarFactory avatarFactory;

			parseMaster.DeserializeObjectFromFile(filename);
			Assert::AreEqual(size_t(7), rootGameObject.Size());

			Assert::AreEqual("Scene"s, rootGameObject["Name"s].GetString());
			Assert::AreEqual("root"s, rootGameObject["Tag"s].GetString());
			Assert::AreEqual(0, rootGameObject["Layer"s].GetInt());
			Assert::AreEqual(glm::mat4(1.0f), rootGameObject["Transform"s].GetMatrix());
			Assert::AreEqual(size_t(1), rootGameObject["Children"s].Size());
			
			rootGameObject.SetName("New Name"s);
			Assert::AreEqual("New Name"s, rootGameObject["Name"s].GetString());

			rootGameObject.SetTag("New Tag"s);
			Assert::AreEqual("New Tag"s, rootGameObject["Tag"s].GetString());

			rootGameObject.SetLayer(10);
			Assert::AreEqual(10, rootGameObject["Layer"s].GetInt());

			rootGameObject.SetTransform(glm::mat4(0.0f));
			Assert::AreEqual(glm::mat4(0.0f), rootGameObject["Transform"s].GetMatrix());
			Assert::AreEqual(glm::vec4(0.0f), rootGameObject.GetPosition());
			Assert::AreEqual(glm::vec4(0.0f), rootGameObject.GetRotation());
			Assert::AreEqual(glm::vec4(0.0f), rootGameObject.GetScale());

			rootGameObject.SetPosition(glm::vec4(1.0f));
			Assert::AreEqual(glm::mat4(glm::vec4(1.0f), glm::vec4(0.0f), glm::vec4(0.0f), glm::vec4(0.0f)), rootGameObject["Transform"s].GetMatrix());
			Assert::AreEqual(glm::vec4(1.0f), rootGameObject.GetPosition());
			Assert::AreEqual(glm::vec4(0.0f), rootGameObject.GetRotation());
			Assert::AreEqual(glm::vec4(0.0f), rootGameObject.GetScale());

			rootGameObject.SetRotation(glm::vec4(2.0f));
			Assert::AreEqual(glm::mat4(glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(0.0f), glm::vec4(0.0f)), rootGameObject["Transform"s].GetMatrix());
			Assert::AreEqual(glm::vec4(1.0f), rootGameObject.GetPosition());
			Assert::AreEqual(glm::vec4(2.0f), rootGameObject.GetRotation());
			Assert::AreEqual(glm::vec4(0.0f), rootGameObject.GetScale());

			rootGameObject.SetScale(glm::vec4(3.0f));
			Assert::AreEqual(glm::mat4(glm::vec4(1.0f), glm::vec4(2.0f), glm::vec4(3.0f), glm::vec4(0.0f)), rootGameObject["Transform"s].GetMatrix());
			Assert::AreEqual(glm::vec4(1.0f), rootGameObject.GetPosition());
			Assert::AreEqual(glm::vec4(2.0f), rootGameObject.GetRotation());
			Assert::AreEqual(glm::vec4(3.0f), rootGameObject.GetScale());

			Avatar avatar;
			avatar.Name = "Child Name"s;
			avatar.Tag = std::string();
			avatar.Layer = 0;
			avatar.Transform = glm::mat4(0.0f);
			avatar.IncrementInteger = 500;

			Assert::IsTrue(rootGameObject["Children"s].GetTable().Equals(&avatar));
			avatar.SetName("New Avatar"s);
			Assert::IsFalse(rootGameObject["Children"s].GetTable().Equals(&avatar));
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState GameObjectTests::_startMemState;
}