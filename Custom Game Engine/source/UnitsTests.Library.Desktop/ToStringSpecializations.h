#pragma once

#include "CppUnitTest.h"
#include "Foo.h"
#include "SList.h"
#include "Vector.h"
#include "Datum.h"
#include "Scope.h"
#include "AttributedFoo.h"
#include "MonsterFoo.h"
#include "JsonTestParseHelper.h"
#include "JsonTableParseHelper.h"
#include "Factory.h"
#include "GameObject.h"
#include "Action.h"
#include "ActionIncrement.h"
#include "ActionList.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "EventQueue.h"

using namespace FieaGameEngine;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<FieaGameEngine::Event<UnitsTestsLibraryDesktop::Foo>>(FieaGameEngine::Event<UnitsTestsLibraryDesktop::Foo>* t)
	{
		t;
		try
		{
			RETURN_WIDE_STRING(L"Foo Event!");
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Event<UnitsTestsLibraryDesktop::Foo>>(const FieaGameEngine::Event<UnitsTestsLibraryDesktop::Foo>* t)
	{
		t;
		try
		{
			RETURN_WIDE_STRING(L"Foo Event!");
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::ActionDestroyAction>(const FieaGameEngine::ActionDestroyAction* t)
	{
		t;
		try
		{
			RETURN_WIDE_STRING(L"ActionDestroyAction");
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::ActionDestroyAction>(FieaGameEngine::ActionDestroyAction* t)
	{
		t;
		try
		{
			RETURN_WIDE_STRING(L"ActionDestroyAction");
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::ActionCreateAction>(const FieaGameEngine::ActionCreateAction* t)
	{
		t;
		try
		{
			RETURN_WIDE_STRING(L"ActionCreateAction");
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::ActionCreateAction>(FieaGameEngine::ActionCreateAction* t)
	{
		t;
		try
		{
			RETURN_WIDE_STRING(L"ActionCreateAction");
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::ActionList>(FieaGameEngine::ActionList* t)
	{
		t;
		try
		{
			RETURN_WIDE_STRING(L"ActionList");
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::ActionList>(const FieaGameEngine::ActionList* t)
	{
		t;
		try
		{
			RETURN_WIDE_STRING(L"ActionList");
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::ActionIncrement>(FieaGameEngine::ActionIncrement* t)
	{
		t;
		try
		{
			RETURN_WIDE_STRING(L"ActionIncrement");
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::ActionIncrement>(const FieaGameEngine::ActionIncrement* t)
	{
		t;
		try
		{
			RETURN_WIDE_STRING(L"ActionIncrement");
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::GameObject>(const FieaGameEngine::GameObject* t)
	{
		t;
		try
		{
			RETURN_WIDE_STRING(L"GameObject");
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::GameObject>(FieaGameEngine::GameObject* t)
	{
		t;
		try
		{
			RETURN_WIDE_STRING(L"GameObject");
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::GameObject>(const FieaGameEngine::GameObject& t)
	{
		t;
		try
		{
			RETURN_WIDE_STRING(L"GameObject");
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Factory<FieaGameEngine::RTTI>>(const FieaGameEngine::Factory<FieaGameEngine::RTTI>* t)
	{
		t;
		try
		{
			RETURN_WIDE_STRING(L"Factory<RTTI>");
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Factory<FieaGameEngine::RTTI>>(FieaGameEngine::Factory<FieaGameEngine::RTTI>* t)
	{
		t;
		try
		{
			RETURN_WIDE_STRING(L"Factory<RTTI>");
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::JsonTableParseHelper::SharedData>(const FieaGameEngine::JsonTableParseHelper::SharedData& t)
	{
		try
		{
			RETURN_WIDE_STRING(t.ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::JsonTableParseHelper::SharedData>(const FieaGameEngine::JsonTableParseHelper::SharedData* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::JsonTableParseHelper::SharedData>(FieaGameEngine::JsonTableParseHelper::SharedData* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::JsonTableParseHelper>(const FieaGameEngine::JsonTableParseHelper& t)
	{
		try
		{
			RETURN_WIDE_STRING(t.ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::JsonTableParseHelper>(const FieaGameEngine::JsonTableParseHelper* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::JsonTableParseHelper>(FieaGameEngine::JsonTableParseHelper* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<std::shared_ptr<FieaGameEngine::JsonParseHelper::SharedData>>(const std::shared_ptr<FieaGameEngine::JsonParseHelper::SharedData>& t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::JsonParseCoordinator>(const FieaGameEngine::JsonParseCoordinator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::JsonParseCoordinator>(FieaGameEngine::JsonParseCoordinator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<UnitsTestsLibraryDesktop::JsonTestParseHelper::SharedData>(const UnitsTestsLibraryDesktop::JsonTestParseHelper::SharedData& t)
	{
		try
		{
			RETURN_WIDE_STRING(t.ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<UnitsTestsLibraryDesktop::JsonTestParseHelper::SharedData>(const UnitsTestsLibraryDesktop::JsonTestParseHelper::SharedData* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<UnitsTestsLibraryDesktop::JsonTestParseHelper::SharedData>(UnitsTestsLibraryDesktop::JsonTestParseHelper::SharedData* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<UnitsTestsLibraryDesktop::JsonTestParseHelper>(const UnitsTestsLibraryDesktop::JsonTestParseHelper& t)
	{
		try
		{
			RETURN_WIDE_STRING(t.ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<UnitsTestsLibraryDesktop::JsonTestParseHelper>(const UnitsTestsLibraryDesktop::JsonTestParseHelper* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<UnitsTestsLibraryDesktop::JsonTestParseHelper>(UnitsTestsLibraryDesktop::JsonTestParseHelper* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<UnitsTestsLibraryDesktop::MonsterFoo>(const UnitsTestsLibraryDesktop::MonsterFoo& t)
	{
		try
		{
			RETURN_WIDE_STRING(t.ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<UnitsTestsLibraryDesktop::MonsterFoo>(const UnitsTestsLibraryDesktop::MonsterFoo* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<UnitsTestsLibraryDesktop::MonsterFoo>(UnitsTestsLibraryDesktop::MonsterFoo* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>	
	inline std::wstring ToString<UnitsTestsLibraryDesktop::AttributedFoo>(const UnitsTestsLibraryDesktop::AttributedFoo& t)
	{
		try
		{
			RETURN_WIDE_STRING(t.ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<UnitsTestsLibraryDesktop::AttributedFoo>(const UnitsTestsLibraryDesktop::AttributedFoo* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<UnitsTestsLibraryDesktop::AttributedFoo>(UnitsTestsLibraryDesktop::AttributedFoo* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Scope>(const Scope& t)
	{
		t;
		wstring returnString = L"Scope";
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<Scope>(const Scope* t)
	{
		t;
		wstring returnString = L"Scope";
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<Scope>(Scope* t)
	{
		t;
		wstring returnString = L"Scope";
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<Datum>(const Datum& t)
	{
		t;
		wstring returnString = L"Datum";
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<Datum>(const Datum* t)
	{
		t;
		wstring returnString = L"Datum";
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<Datum>(Datum* t)
	{
		t;
		wstring returnString = L"Datum";
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<glm::vec<4, float>>(const glm::vec<4, float>& t)
	{
		wstring returnString = std::to_wstring(t.length());
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<glm::vec<4, float>>(const glm::vec<4, float>* t)
	{
		wstring returnString = std::to_wstring(t->length());
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<glm::vec<4, float>>(glm::vec<4, float>* t)
	{
		wstring returnString = std::to_wstring(t->length());
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<glm::mat<4, 4, float>>(const glm::mat<4, 4, float>& t)
	{
		wstring returnString = std::to_wstring(t.length());
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<glm::mat<4, 4, float>>(const glm::mat<4, 4, float>* t)
	{
		wstring returnString = std::to_wstring(t->length());
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<glm::mat<4, 4, float>>(glm::mat<4, 4, float>* t)
	{
		wstring returnString = std::to_wstring(t->length());
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<RTTI>(const RTTI& t)
	{
		wstring returnString = std::to_wstring(t.TypeIdInstance());
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<RTTI>(const RTTI* t)
	{
		wstring returnString = std::to_wstring(t->TypeIdInstance());
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<RTTI>(RTTI* t)
	{
		wstring returnString = std::to_wstring(t->TypeIdInstance());
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<DatumTypes>(const DatumTypes& t)
	{
		switch (t)
		{
		case DatumTypes::Unknown:
		{
			RETURN_WIDE_STRING(L"Unknown");
			break;
		}

		case DatumTypes::Integer:
		{
			RETURN_WIDE_STRING(L"Integer");
			break;
		}

		case DatumTypes::Float:
		{
			RETURN_WIDE_STRING(L"Float");
			break;
		}

		case DatumTypes::Vector:
		{
			RETURN_WIDE_STRING(L"Vector");
			break;
		}

		case DatumTypes::Matrix:
		{
			RETURN_WIDE_STRING(L"Matrix");
			break;
		}

		case DatumTypes::Pointer:
		{
			RETURN_WIDE_STRING(L"Pointer");
			break;
		}

		default:
			RETURN_WIDE_STRING(L"Invalid!");
			break;
		}
	}

	template<>
	inline std::wstring ToString<DatumTypes>(const DatumTypes* t)
	{
		switch (*t)
		{
		case DatumTypes::Unknown:
		{
			RETURN_WIDE_STRING(L"Unknown");
			break;
		}

		case DatumTypes::Integer:
		{
			RETURN_WIDE_STRING(L"Integer");
			break;
		}

		case DatumTypes::Float:
		{
			RETURN_WIDE_STRING(L"Float");
			break;
		}

		case DatumTypes::Vector:
		{
			RETURN_WIDE_STRING(L"Vector");
			break;
		}

		case DatumTypes::Matrix:
		{
			RETURN_WIDE_STRING(L"Matrix");
			break;
		}

		case DatumTypes::Pointer:
		{
			RETURN_WIDE_STRING(L"Pointer");
			break;
		}

		default:
			RETURN_WIDE_STRING(L"Invalid!");
			break;
		}
	}

	template<>
	inline std::wstring ToString<DatumTypes>(DatumTypes* t)
	{
		switch (*t)
		{
		case DatumTypes::Unknown:
		{
			RETURN_WIDE_STRING(L"Unknown");
			break;
		}

		case DatumTypes::Integer:
		{
			RETURN_WIDE_STRING(L"Integer");
			break;
		}

		case DatumTypes::Float:
		{
			RETURN_WIDE_STRING(L"Float");
			break;
		}

		case DatumTypes::Vector:
		{
			RETURN_WIDE_STRING(L"Vector");
			break;
		}

		case DatumTypes::Matrix:
		{
			RETURN_WIDE_STRING(L"Matrix");
			break;
		}

		case DatumTypes::Pointer:
		{
			RETURN_WIDE_STRING(L"Pointer");
			break;
		}

		default:
			RETURN_WIDE_STRING(L"Invalid!");
			break;
		}
	}

	template<>
	inline std::wstring ToString<std::pair<const UnitsTestsLibraryDesktop::Foo, int>>(const std::pair<const UnitsTestsLibraryDesktop::Foo, int>& t)
	{
		std::wstring returnString = std::to_wstring(t.first.Data()) + std::to_wstring(t.second);
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<std::pair<const UnitsTestsLibraryDesktop::Foo, int>>(const std::pair<const UnitsTestsLibraryDesktop::Foo, int>* t)
	{
		std::wstring returnString = std::to_wstring(t->first.Data()) + std::to_wstring(t->second);
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<std::pair<const UnitsTestsLibraryDesktop::Foo, int>>(std::pair<const UnitsTestsLibraryDesktop::Foo, int>* t)
	{
		std::wstring returnString = std::to_wstring(t->first.Data()) + std::to_wstring(t->second);
		RETURN_WIDE_STRING(returnString);
	}

	template<>
	inline std::wstring ToString<UnitsTestsLibraryDesktop::Foo>(const UnitsTestsLibraryDesktop::Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<UnitsTestsLibraryDesktop::Foo>(const UnitsTestsLibraryDesktop::Foo* t)
	{
		RETURN_WIDE_STRING(t->Data());
	}

	template<>
	inline std::wstring ToString<UnitsTestsLibraryDesktop::Foo>(UnitsTestsLibraryDesktop::Foo* t)
	{
		RETURN_WIDE_STRING(t->Data());
	}
	
	template<>
	inline std::wstring ToString<SList<UnitsTestsLibraryDesktop::Foo>::Iterator>(const SList<UnitsTestsLibraryDesktop::Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<SList<UnitsTestsLibraryDesktop::Foo>::ConstIterator>(const SList<UnitsTestsLibraryDesktop::Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}

		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<UnitsTestsLibraryDesktop::Foo>::Iterator>(const Vector<UnitsTestsLibraryDesktop::Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<UnitsTestsLibraryDesktop::Foo>::ConstIterator>(const Vector<UnitsTestsLibraryDesktop::Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}

		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<HashMap<UnitsTestsLibraryDesktop::Foo, int>::Iterator>(const HashMap<UnitsTestsLibraryDesktop::Foo, int>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}

		catch (const std::exception)
		{
			return L"end()"s;
		}
	}
}