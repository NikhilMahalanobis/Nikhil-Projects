#include "pch.h"
#include "ToStringSpecializations.h"
#include "AttributedFoo.h"
#include "JsonTableParseHelper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitsTestsLibraryDesktop
{
	TEST_CLASS(JsonTableParseHelperTests)
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

		TEST_METHOD(JsonParseTestHelperRTTI)
		{
			JsonTableParseHelper helper;

			RTTI* rtti = &helper;
			Assert::IsFalse(rtti->Is("AttributedFoo"s));
			Assert::IsTrue(rtti->Is("JsonParseHelper"s));
			Assert::IsTrue(rtti->Is("JsonTableParseHelper"s));
			Assert::IsFalse(rtti->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(JsonTableParseHelper::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(JsonTableParseHelper::TypeIdClass(), rtti->TypeIdInstance());

			AttributedFoo* f = rtti->As<AttributedFoo>();
			Assert::IsNull(f);

			JsonTableParseHelper* h = rtti->As<JsonTableParseHelper>();
			Assert::IsNotNull(h);
			Assert::AreEqual(&helper, h);
			RTTI* r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(JsonTableParseHelper::TypeIdClass());
			Assert::IsNotNull(r);

			r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(JsonTableParseHelper::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(JsonParseTestHelperSharedDataRTTI)
		{
			JsonTableParseHelper::SharedData sharedData;

			RTTI* rtti = &sharedData;
			Assert::IsFalse(rtti->Is("AttributedFoo"s));
			Assert::IsTrue(rtti->Is("SharedData"s));
			Assert::IsFalse(rtti->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(JsonTableParseHelper::SharedData::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(JsonTableParseHelper::SharedData::TypeIdClass(), rtti->TypeIdInstance());

			AttributedFoo* f = rtti->As<AttributedFoo>();
			Assert::IsNull(f);

			JsonTableParseHelper::SharedData* h = rtti->As<JsonTableParseHelper::SharedData>();
			Assert::IsNotNull(h);
			Assert::AreEqual(&sharedData, h);
			RTTI* r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(JsonTableParseHelper::SharedData::TypeIdClass());
			Assert::IsNotNull(r);

			r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(JsonTableParseHelper::SharedData::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(Constructor)
		{
			std::shared_ptr<JsonTableParseHelper::SharedData> sharedData_ptr = std::make_shared<JsonTableParseHelper::SharedData>();
			JsonParseCoordinator parseMaster(sharedData_ptr);
			Assert::AreEqual(sharedData_ptr.get(), parseMaster.GetSharedData()->As<JsonTableParseHelper::SharedData>());

			std::shared_ptr<JsonTableParseHelper> testParseHelper = std::make_shared<JsonTableParseHelper>();
			parseMaster.AddHelper(testParseHelper);
			Assert::AreEqual(size_t(1), parseMaster.Helpers().Size());

			Assert::AreEqual(sharedData_ptr.get(), parseMaster.GetSharedData()->As<JsonTableParseHelper::SharedData>());
			Assert::AreEqual(&parseMaster, sharedData_ptr->GetJsonParseCoordinator());

			std::shared_ptr<JsonTableParseHelper::SharedData> anotherSharedData = std::make_shared<JsonTableParseHelper::SharedData>();
			parseMaster.SetSharedData(anotherSharedData);
			Assert::AreEqual(anotherSharedData.get(), parseMaster.GetSharedData()->As<JsonTableParseHelper::SharedData>());
		}

		TEST_METHOD(ScopeParsing)
		{
			const string inputString = R"(
            {
                "Name":
                {
                    "type": "string",
                    "value": "Test Name"
                }
            })"s;

			Scope root;
			std::shared_ptr<JsonTableParseHelper::SharedData> sharedData = std::make_shared<JsonTableParseHelper::SharedData>(root);
			std::shared_ptr<JsonTableParseHelper> testParseHelper = std::make_shared<JsonTableParseHelper>();
			JsonParseCoordinator parseMaster(sharedData);
			parseMaster.AddHelper(testParseHelper);

			parseMaster.DeserializeObject(inputString);

			Assert::AreEqual(size_t(1), sharedData->_parentScope->Size());
			Assert::AreEqual("Test Name"s, root["Name"s].GetString());
			Assert::AreEqual(size_t(0), sharedData->Depth());
			Assert::AreEqual(size_t(2), sharedData->MaxDepth);
		}

		TEST_METHOD(ScopeArrayParsing)
		{
			const string inputString = R"(
			{
				"Aliases": 
				{
					"type": "string",
					"value": [ "Bob", "Jim", "Fred" ]
				}
			})";


			Scope root;
			std::shared_ptr<JsonTableParseHelper::SharedData> sharedData = std::make_shared<JsonTableParseHelper::SharedData>(root);
			std::shared_ptr<JsonTableParseHelper> testParseHelper = std::make_shared<JsonTableParseHelper>();
			JsonParseCoordinator parseMaster(sharedData);
			parseMaster.AddHelper(testParseHelper);

			parseMaster.DeserializeObject(inputString);

			Assert::AreEqual(size_t(1), sharedData->_parentScope->Size());
			Assert::AreEqual(size_t(3), (*(sharedData->_parentScope))["Aliases"s].Size());
			Assert::AreEqual("Bob"s, (*(sharedData->_parentScope))["Aliases"s].GetString(0));
			Assert::AreEqual("Jim"s, (*(sharedData->_parentScope))["Aliases"s].GetString(1));
			Assert::AreEqual("Fred"s, (*(sharedData->_parentScope))["Aliases"s].GetString(2));
			Assert::AreEqual(size_t(0), sharedData->Depth());
			Assert::AreEqual(size_t(2), sharedData->MaxDepth);
		}

		TEST_METHOD(ScopeFileParsing)
		{
			const std::string filename = R"(Content\TestScope.json)";

			Scope root;

			JsonParseCoordinator parseMaster(std::make_shared<JsonTableParseHelper::SharedData>(root));
			parseMaster.AddHelper(std::make_shared<JsonTableParseHelper>());

			AttributedFooFactory attributedFooFactory;

			ScopeFactory scopeFactory;
			size_t attributedCount = 13;

			parseMaster.DeserializeObjectFromFile(filename);
			Assert::AreEqual(size_t(8), root.Size());
			Assert::AreEqual("Bob"s, root["Aliases"s].GetString(0));
			Assert::AreEqual("Jim"s, root["Aliases"s].GetString(1));
			Assert::AreEqual("Fred"s, root["Aliases"s].GetString(2));
			Assert::AreEqual(glm::vec4(1, 0, 0, 1), root["Direction"s].GetVector());
			Assert::AreEqual(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), root["Transform"s].GetMatrix());
			Assert::AreEqual(100, root["Health"s].GetInt());
			Assert::AreEqual("Test Name"s, root["Name"s].GetString());
			Assert::AreEqual(DatumTypes::Table, root["EmptyNestedScopeArray"s].Type());
			Assert::AreEqual(size_t(0), root["EmptyNestedScopeArray"s].Size());

			/*Scope testingRoot;
			Scope& address = testingRoot.AppendScope("Address");

			Datum& city = address.Append("City");
			city.SetType(DatumTypes::String);
			city.PushBack("Orlando");

			Scope& postal = address.AppendScope("Postal Code");

			Datum& zip = postal.Append("Zip Code");
			zip.SetType(DatumTypes::Integer);
			zip.PushBack(12345);

			Datum& zip4 = postal.Append("Zip Code+4");
			zip4.SetType(DatumTypes::Integer);
			zip4.PushBack(6789);

			Datum& street = address.Append("Street");
			street.SetType(DatumTypes::String);
			street.PushBack("123 Anystreet St.");
			Assert::AreEqual(address, root["Address"s].GetTable());
			*/

			Assert::AreEqual(size_t(1), root["Address"s].Size());
			Assert::AreEqual(size_t(3 + attributedCount), root["Address"s].GetTable().Size());

			Assert::AreEqual(size_t(3), root["Powers"s].Size());
			Assert::AreEqual(size_t(2 + attributedCount), root["Powers"s].GetTable(0).Size());
			Assert::AreEqual(size_t(2 + attributedCount), root["Powers"s].GetTable(1).Size());
			Assert::AreEqual(size_t(2 + attributedCount), root["Powers"s].GetTable(2).Size());

		}

		TEST_METHOD(Clone)
		{
			std::shared_ptr<JsonTableParseHelper::SharedData> sharedData = std::make_shared<JsonTableParseHelper::SharedData>();
			std::shared_ptr<JsonTableParseHelper> testParseHelper = std::make_shared<JsonTableParseHelper>();
			JsonParseCoordinator parseMaster(sharedData);
			parseMaster.AddHelper(testParseHelper);

			auto clone = parseMaster.Clone();
			Assert::AreNotEqual(&*clone, &parseMaster);
			Assert::AreEqual(size_t(1), clone->Helpers().Size());

			auto clonedHelper = clone->Helpers()[0]->As<JsonTableParseHelper>();
			Assert::IsNotNull(clonedHelper);
			Assert::AreNotEqual(clonedHelper, parseMaster.Helpers()[0]->As<JsonTableParseHelper>());
			Assert::AreNotSame(clone->GetSharedData(), parseMaster.GetSharedData());


			clone->RemoveHelper(clone->Helpers()[0]);
		}
	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState JsonTableParseHelperTests::_startMemState;
}