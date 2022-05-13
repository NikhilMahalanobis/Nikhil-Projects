#include "pch.h"
#include "TypeRegistry.h"
#include "AttributedFoo.h"
#include "MonsterFoo.h"
#include <limits>
#include "ToStringSpecializations.h"
#include "GameObject.h"
#include "Avatar.h"
#include "Action.h"
#include "ActionIncrement.h"
#include "ActionList.h"
#include "ActionListIf.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "EventQueue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitsTestsLibraryDesktop
{
	TEST_MODULE_INITIALIZE(TestModuleInitialize)
	{
		TypeRegistry::getInstance()->registerType<AttributedFoo>();
		TypeRegistry::getInstance()->registerParentedType<MonsterFoo, AttributedFoo>();
		TypeRegistry::getInstance()->registerType<GameObject>();
		TypeRegistry::getInstance()->registerParentedType<Avatar, GameObject>();
		TypeRegistry::getInstance()->registerType<Action>();
		TypeRegistry::getInstance()->registerParentedType<ActionList, Action>();
		TypeRegistry::getInstance()->registerParentedType<ActionIncrement, Action>();
		TypeRegistry::getInstance()->registerParentedType<ActionListIf, ActionList>();
		TypeRegistry::getInstance()->registerParentedType<ActionCreateAction, Action>();
		TypeRegistry::getInstance()->registerParentedType<ActionDestroyAction, Action>();
	}

	TEST_MODULE_CLEANUP(TestModuleCleanup)
	{
		TypeRegistry::getInstance()->Clear();
	}
}