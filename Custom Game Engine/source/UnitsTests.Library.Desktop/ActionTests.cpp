#include "pch.h"
#include "GameObject.h"
#include "ActionList.h"
#include "ActionIncrement.h"
#include "ActionListIf.h"
#include "ToStringSpecializations.h"
#include "Avatar.h"
#include "GameState.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitsTestsLibraryDesktop
{
	TEST_CLASS(ActionTests)
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
			{
				ActionIncrement a;
				RTTI* rtti = &a;

				Assert::IsTrue(rtti->Is("Action"s));
				Assert::IsTrue(rtti->Is("ActionIncrement"s));
				Assert::IsFalse(rtti->Is("GameObject"s));
				Assert::IsTrue(rtti->Is("Scope"s));
				Assert::IsTrue(rtti->Is(ActionIncrement::TypeIdClass()));
				Assert::IsTrue(rtti->Is(Action::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(ActionIncrement::TypeIdClass(), rtti->TypeIdInstance());

				ActionIncrement* f = rtti->As<ActionIncrement>();
				Assert::IsNotNull(f);
				Assert::AreEqual(&a, f);
			}

			{
				ActionList a;
				RTTI* rtti = &a;

				Assert::IsTrue(rtti->Is("Action"s));
				Assert::IsTrue(rtti->Is("ActionList"s));
				Assert::IsFalse(rtti->Is("ActionIncrement"s));
				Assert::IsTrue(rtti->Is("Scope"s));
				Assert::IsTrue(rtti->Is(ActionList::TypeIdClass()));
				Assert::IsTrue(rtti->Is(Action::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(ActionList::TypeIdClass(), rtti->TypeIdInstance());

				ActionList* f = rtti->As<ActionList>();
				Assert::IsNotNull(f);
				Assert::AreEqual(&a, f);
			}

			{
				ActionCreateAction a;
				RTTI* rtti = &a;

				Assert::IsTrue(rtti->Is("Action"s));
				Assert::IsTrue(rtti->Is("ActionCreateAction"s));
				Assert::IsFalse(rtti->Is("ActionDestroyAction"s));
				Assert::IsTrue(rtti->Is("Scope"s));
				Assert::IsTrue(rtti->Is(ActionCreateAction::TypeIdClass()));
				Assert::IsTrue(rtti->Is(Action::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(ActionCreateAction::TypeIdClass(), rtti->TypeIdInstance());

				ActionCreateAction* f = rtti->As<ActionCreateAction>();
				Assert::IsNotNull(f);
				Assert::AreEqual(&a, f);
			}

			{
				ActionDestroyAction a;
				RTTI* rtti = &a;

				Assert::IsTrue(rtti->Is("Action"s));
				Assert::IsTrue(rtti->Is("ActionDestroyAction"s));
				Assert::IsFalse(rtti->Is("ActionCreateAction"s));
				Assert::IsTrue(rtti->Is("Scope"s));
				Assert::IsTrue(rtti->Is(ActionDestroyAction::TypeIdClass()));
				Assert::IsTrue(rtti->Is(Action::TypeIdClass()));
				Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
				Assert::AreEqual(ActionDestroyAction::TypeIdClass(), rtti->TypeIdInstance());

				ActionDestroyAction* f = rtti->As<ActionDestroyAction>();
				Assert::IsNotNull(f);
				Assert::AreEqual(&a, f);
			}
		}

		TEST_METHOD(ActionConstructors)
		{
			{
				ActionIncrement increment;

				Assert::IsTrue(increment.IsPrescribedAttribute("Name"s));
				Assert::IsTrue(increment.IsPrescribedAttribute("Step"s));
				Assert::IsTrue(increment.IsPrescribedAttribute("Target"s));

				Assert::AreEqual(DatumTypes::String, increment["Name"s].Type());
				Assert::AreEqual(DatumTypes::Float, increment["Step"s].Type());
				Assert::AreEqual(DatumTypes::String, increment["Target"s].Type());

				Assert::AreEqual(std::string(), increment["Name"s].GetString());
				Assert::AreEqual(1.0f, increment["Step"s].GetFloat());
				Assert::AreEqual(std::string(), increment["Target"s].GetString());
			}

			{
				ActionList list;

				Assert::IsTrue(list.IsPrescribedAttribute("Name"s));
				Assert::IsTrue(list.IsPrescribedAttribute("Actions"s));

				Assert::AreEqual(DatumTypes::String, list["Name"s].Type());
				Assert::AreEqual(DatumTypes::Table, list["Actions"s].Type());

				Assert::AreEqual(std::string(), list["Name"s].GetString());
			}

			{
				ActionListIf ifList;

				Assert::IsTrue(ifList.IsPrescribedAttribute("Name"s));
				Assert::IsTrue(ifList.IsPrescribedAttribute("Actions"s));
				Assert::IsTrue(ifList.IsPrescribedAttribute("Condition"s));
				Assert::IsTrue(ifList.IsPrescribedAttribute("Then"s));
				Assert::IsTrue(ifList.IsPrescribedAttribute("Else"s));

				Assert::AreEqual(DatumTypes::String, ifList["Name"s].Type());
				Assert::AreEqual(DatumTypes::Table, ifList["Actions"s].Type());
				Assert::AreEqual(DatumTypes::Integer, ifList["Condition"s].Type());
				Assert::AreEqual(DatumTypes::Table, ifList["Then"s].Type());
				Assert::AreEqual(DatumTypes::Table, ifList["Else"s].Type());

				Assert::AreEqual(std::string(), ifList["Name"s].GetString());
				Assert::AreEqual(0, ifList["Condition"s].GetInt());
			}

			{
				ActionCreateAction createAction;

				Assert::IsTrue(createAction.IsPrescribedAttribute("Name"s));
				Assert::IsTrue(createAction.IsPrescribedAttribute("ClassName"s));
				Assert::IsTrue(createAction.IsPrescribedAttribute("InstanceName"s));

				Assert::AreEqual(DatumTypes::String, createAction["Name"s].Type());
				Assert::AreEqual(DatumTypes::String, createAction["ClassName"s].Type());
				Assert::AreEqual(DatumTypes::String, createAction["InstanceName"s].Type());

				Assert::AreEqual(std::string(), createAction["Name"s].GetString());
				Assert::AreEqual(std::string(), createAction["ClassName"s].GetString());
				Assert::AreEqual(std::string(), createAction["InstanceName"s].GetString());
			}

			{
				ActionDestroyAction destroyAction;

				Assert::IsTrue(destroyAction.IsPrescribedAttribute("Name"s));
				Assert::IsTrue(destroyAction.IsPrescribedAttribute("InstanceNameToDestroy"s));

				Assert::AreEqual(DatumTypes::String, destroyAction["Name"s].Type());
				Assert::AreEqual(DatumTypes::String, destroyAction["InstanceNameToDestroy"s].Type());

				Assert::AreEqual(std::string(), destroyAction["Name"s].GetString());
				Assert::AreEqual(std::string(), destroyAction["InstanceNameToDestroy"s].GetString());
			}
		}

		TEST_METHOD(ActionParsingTests)
		{
			// Testing Actions
			{
				const std::string filename = R"(Content\TestAction.json)";

				GameObject rootGameObject;

				JsonParseCoordinator parseMaster(std::make_shared<JsonTableParseHelper::SharedData>(rootGameObject));
				parseMaster.AddHelper(std::make_shared<JsonTableParseHelper>());

				ScopeFactory scopeFactory;
				GameObjectFactory gameObjFactory;
				AvatarFactory avatarFactory;
				ActionIncrementFactory actionIncrementFactory;

				parseMaster.DeserializeObjectFromFile(filename);
				Assert::AreEqual(size_t(8), rootGameObject.Size());

				Assert::AreEqual(0.0f, rootGameObject["TestFloat"s].GetFloat());

				GameTime gametime;
				GameState gameState;
				gameState._wallTime = gametime;

				Scope& actions = rootGameObject["Actions"s].GetTable();
				Assert::AreEqual(size_t(4), actions.Size());
				Assert::AreEqual(0.0f, rootGameObject["TestFloat"s].GetFloat());
				rootGameObject.Update(&gameState);
				Assert::AreEqual(5.0f, rootGameObject["TestFloat"s].GetFloat());
				rootGameObject.Update(&gameState);
				Assert::AreEqual(10.0f, rootGameObject["TestFloat"s].GetFloat());
				rootGameObject.Update(&gameState);
				Assert::AreEqual(15.0f, rootGameObject["TestFloat"s].GetFloat());

			}

			// Testing ActionIncrement
			{
				const std::string filename = R"(Content\TestActionListFalse.json)";

				GameObject rootGameObject;

				JsonParseCoordinator parseMaster(std::make_shared<JsonTableParseHelper::SharedData>(rootGameObject));
				parseMaster.AddHelper(std::make_shared<JsonTableParseHelper>());

				ScopeFactory scopeFactory;
				GameObjectFactory gameObjFactory;
				AvatarFactory avatarFactory;
				ActionIncrementFactory actionIncrementFactory;
				ActionListIfFactory actionListIfFactory;

				parseMaster.DeserializeObjectFromFile(filename);
				Assert::AreEqual(size_t(9), rootGameObject.Size());

				Assert::AreEqual(0.0f, rootGameObject["TestFloat1"s].GetFloat());
				Assert::AreEqual(0.0f, rootGameObject["TestFloat2"s].GetFloat());

				GameTime gametime;
				GameState gameState;
				gameState._wallTime = gametime;

				Scope& actions = rootGameObject["Actions"s].GetTable();
				Assert::AreEqual(size_t(7), actions.Size());
				Assert::AreEqual(0.0f, rootGameObject["TestFloat1"s].GetFloat());
				Assert::AreEqual(0.0f, rootGameObject["TestFloat2"s].GetFloat());

				rootGameObject.Update(&gameState);
				Assert::AreEqual(0.0f, rootGameObject["TestFloat1"s].GetFloat());
				Assert::AreEqual(-5.0f, rootGameObject["TestFloat2"s].GetFloat());

				rootGameObject.Update(&gameState);
				Assert::AreEqual(0.0f, rootGameObject["TestFloat1"s].GetFloat());
				Assert::AreEqual(-10.0f, rootGameObject["TestFloat2"s].GetFloat());

				rootGameObject.Update(&gameState);
				Assert::AreEqual(0.0f, rootGameObject["TestFloat1"s].GetFloat());
				Assert::AreEqual(-15.0f, rootGameObject["TestFloat2"s].GetFloat());
			}

			//Test ActionList
			{
				const std::string filename = R"(Content\TestActionList.json)";

				GameObject rootGameObject;

				JsonParseCoordinator parseMaster(std::make_shared<JsonTableParseHelper::SharedData>(rootGameObject));
				parseMaster.AddHelper(std::make_shared<JsonTableParseHelper>());

				ScopeFactory scopeFactory;
				GameObjectFactory gameObjFactory;
				AvatarFactory avatarFactory;
				ActionIncrementFactory actionIncrementFactory;
				ActionListFactory actionListFactory;

				parseMaster.DeserializeObjectFromFile(filename);
				Assert::AreEqual(size_t(9), rootGameObject.Size());

				Assert::AreEqual(0.0f, rootGameObject["TestFloat1"s].GetFloat());
				Assert::AreEqual(0.0f, rootGameObject["TestFloat2"s].GetFloat());

				GameTime gametime;
				GameState gameState;
				gameState._wallTime = gametime;

				Scope& actions = rootGameObject["Actions"s].GetTable();
				Assert::AreEqual(size_t(4), actions.Size());
				Assert::AreEqual(0.0f, rootGameObject["TestFloat1"s].GetFloat());
				Assert::AreEqual(0.0f, rootGameObject["TestFloat2"s].GetFloat());

				rootGameObject.Update(&gameState);
				Assert::AreEqual(5.0f, rootGameObject["TestFloat1"s].GetFloat());
				Assert::AreEqual(-5.0f, rootGameObject["TestFloat2"s].GetFloat());

				rootGameObject.Update(&gameState);
				Assert::AreEqual(10.0f, rootGameObject["TestFloat1"s].GetFloat());
				Assert::AreEqual(-10.0f, rootGameObject["TestFloat2"s].GetFloat());

				rootGameObject.Update(&gameState);
				Assert::AreEqual(15.0f, rootGameObject["TestFloat1"s].GetFloat());
				Assert::AreEqual(-15.0f, rootGameObject["TestFloat2"s].GetFloat());
			}

			// Testing ActionListsIf
			{
				const std::string filename = R"(Content\TestActionListTrue.json)";

				GameObject rootGameObject;

				JsonParseCoordinator parseMaster(std::make_shared<JsonTableParseHelper::SharedData>(rootGameObject));
				parseMaster.AddHelper(std::make_shared<JsonTableParseHelper>());

				ScopeFactory scopeFactory;
				GameObjectFactory gameObjFactory;
				AvatarFactory avatarFactory;
				ActionIncrementFactory actionIncrementFactory;
				ActionListIfFactory actionListIfFactory;

				parseMaster.DeserializeObjectFromFile(filename);
				Assert::AreEqual(size_t(9), rootGameObject.Size());

				Assert::AreEqual(0.0f, rootGameObject["TestFloat1"s].GetFloat());
				Assert::AreEqual(0.0f, rootGameObject["TestFloat2"s].GetFloat());

				GameTime gametime;
				GameState gameState;
				gameState._wallTime = gametime;

				Scope& actions = rootGameObject["Actions"s].GetTable();
				Assert::AreEqual(size_t(7), actions.Size());
				Assert::AreEqual(0.0f, rootGameObject["TestFloat1"s].GetFloat());
				Assert::AreEqual(0.0f, rootGameObject["TestFloat2"s].GetFloat());

				rootGameObject.Update(&gameState);
				Assert::AreEqual(5.0f, rootGameObject["TestFloat1"s].GetFloat());
				Assert::AreEqual(0.0f, rootGameObject["TestFloat2"s].GetFloat());

				rootGameObject.Update(&gameState);
				Assert::AreEqual(10.0f, rootGameObject["TestFloat1"s].GetFloat());
				Assert::AreEqual(0.0f, rootGameObject["TestFloat2"s].GetFloat());

				rootGameObject.Update(&gameState);
				Assert::AreEqual(15.0f, rootGameObject["TestFloat1"s].GetFloat());
				Assert::AreEqual(0.0f, rootGameObject["TestFloat2"s].GetFloat());
			}

			//Testing ActionCreate & ActionDestroy 
			{
				const std::string filename = R"(Content\TestActionCreateAndDeleteAction.json)";

				GameObject rootGameObject;

				JsonParseCoordinator parseMaster(std::make_shared<JsonTableParseHelper::SharedData>(rootGameObject));
				parseMaster.AddHelper(std::make_shared<JsonTableParseHelper>());

				ScopeFactory scopeFactory;
				GameObjectFactory gameObjFactory;
				AvatarFactory avatarFactory;
				ActionIncrementFactory actionIncrementFactory;
				ActionCreateActionFactory createActionFactory;
				ActionDestroyActionFactory destroyActionFactory;

				parseMaster.DeserializeObjectFromFile(filename);
				Assert::AreEqual(size_t(9), rootGameObject.Size());

				GameTime gametime;
				GameState gameState;
				gameState._wallTime = gametime;

				Assert::AreEqual("Test Action Create And Delete Action"s, rootGameObject.GetName());
				Assert::AreEqual(0.0f, rootGameObject["A"s].GetFloat());
				Assert::AreEqual(0.0f, rootGameObject["B"s].GetFloat());

				auto& _actions = rootGameObject["Actions"s];
				_actions;
				Assert::AreEqual(size_t(3), rootGameObject["Actions"s].Size());

				auto actionDestroyAction = rootGameObject["Actions"s].GetTable(0).As<ActionDestroyAction>();
				Assert::IsNotNull(actionDestroyAction);
				Assert::AreEqual("DestroyIncrementA"s, actionDestroyAction->GetName());

				auto actionCreateAction = rootGameObject["Actions"s].GetTable(1).As<ActionCreateAction>();
				Assert::IsNotNull(actionCreateAction);
				Assert::AreEqual("CreateIncrementB"s, actionCreateAction->GetName());

				auto actionIncrement = rootGameObject["Actions"s].GetTable(2).As<ActionIncrement>();
				Assert::IsNotNull(actionIncrement);
				Assert::AreEqual("ActionIncrementToBeDeleted"s, actionIncrement->GetName());

				// First Update, should increment A, Create & Destroy added to queue and processed after update
				rootGameObject.Update(&gameState);
				gameState.UpdateQueues();

				Assert::AreEqual(size_t(3), rootGameObject["Actions"s].Size());
				auto newIncrementAction = rootGameObject["Actions"s].GetTable(2).As<ActionIncrement>();
				Assert::AreEqual("NewIncrementB"s, newIncrementAction->GetName());
				Assert::AreEqual(1.0f, rootGameObject["A"s].GetFloat());
				Assert::AreEqual(0.0f, rootGameObject["B"s].GetFloat());

				// Second Update, should increment B, no Create or Destroy call
				rootGameObject.Update(&gameState);
				gameState.UpdateQueues();
				Assert::AreEqual(size_t(3), rootGameObject["Actions"s].Size());
				auto anotherActionIncrement = rootGameObject["Actions"s].GetTable(2).As<ActionIncrement>();
				Assert::IsNotNull(anotherActionIncrement);
				Assert::AreEqual(newIncrementAction, anotherActionIncrement);
				Assert::AreEqual(1.0f, rootGameObject["A"s].GetFloat());
				Assert::AreEqual(1.0f, rootGameObject["B"s].GetFloat());


			}
		}


	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState ActionTests::_startMemState;
}