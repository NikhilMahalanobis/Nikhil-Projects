#include "pch.h"
#include "ToStringSpecializations.h"
#include "AttributedFoo.h"
#include "JsonTestParseHelper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitsTestsLibraryDesktop
{
	TEST_CLASS(JsonParseCoordinatorTests)
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
			JsonTestParseHelper helper;

			RTTI* rtti = &helper;
			Assert::IsFalse(rtti->Is("AttributedFoo"s));
			Assert::IsTrue(rtti->Is("JsonParseHelper"s));
			Assert::IsTrue(rtti->Is("JsonTestParseHelper"s));
			Assert::IsFalse(rtti->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(JsonTestParseHelper::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(JsonTestParseHelper::TypeIdClass(), rtti->TypeIdInstance());

			AttributedFoo* f = rtti->As<AttributedFoo>();
			Assert::IsNull(f);

			JsonTestParseHelper* h = rtti->As<JsonTestParseHelper>();
			Assert::IsNotNull(h);
			Assert::AreEqual(&helper, h);
			RTTI* r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(JsonTestParseHelper::TypeIdClass());
			Assert::IsNotNull(r);

			r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(JsonTestParseHelper::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(JsonParseTestHelperSharedDataRTTI)
		{
			JsonTestParseHelper::SharedData sharedData;

			RTTI* rtti = &sharedData;
			Assert::IsFalse(rtti->Is("AttributedFoo"s));
			Assert::IsTrue(rtti->Is("SharedData"s));
			Assert::IsFalse(rtti->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(JsonTestParseHelper::SharedData::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(JsonTestParseHelper::SharedData::TypeIdClass(), rtti->TypeIdInstance());

			AttributedFoo* f = rtti->As<AttributedFoo>();
			Assert::IsNull(f);

			JsonTestParseHelper::SharedData* h = rtti->As<JsonTestParseHelper::SharedData>();
			Assert::IsNotNull(h);
			Assert::AreEqual(&sharedData, h);
			RTTI* r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(JsonTestParseHelper::SharedData::TypeIdClass());
			Assert::IsNotNull(r);

			r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(JsonTestParseHelper::SharedData::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(Constructor)
		{
			std::shared_ptr<JsonTestParseHelper::SharedData> sharedData_ptr = std::make_shared<JsonTestParseHelper::SharedData>();
			JsonParseCoordinator parseMaster(sharedData_ptr);
			Assert::AreEqual(sharedData_ptr.get(), parseMaster.GetSharedData()->As<JsonTestParseHelper::SharedData>());

			std::shared_ptr<JsonTestParseHelper> testParseHelper = std::make_shared<JsonTestParseHelper>();
			parseMaster.AddHelper(testParseHelper);
			Assert::AreEqual(size_t(1), parseMaster.Helpers().Size());

			Assert::AreEqual(sharedData_ptr.get(), parseMaster.GetSharedData()->As<JsonTestParseHelper::SharedData>());
			Assert::AreEqual(&parseMaster, sharedData_ptr->GetJsonParseCoordinator());

			std::shared_ptr<JsonTestParseHelper::SharedData> anotherSharedData = std::make_shared<JsonTestParseHelper::SharedData>();
			parseMaster.SetSharedData(anotherSharedData);
			Assert::AreEqual(anotherSharedData.get(), parseMaster.GetSharedData()->As<JsonTestParseHelper::SharedData>());
		}

		TEST_METHOD(MoveSemantics)
		{
			std::shared_ptr<JsonTestParseHelper::SharedData> sharedData = std::make_shared<JsonTestParseHelper::SharedData>();
			std::shared_ptr<JsonTestParseHelper> testParseHelper = std::make_shared<JsonTestParseHelper>();

			{
				JsonParseCoordinator parseMaster(sharedData);
				parseMaster.AddHelper(testParseHelper);

				JsonParseCoordinator otherParseMaster = std::move(parseMaster);
				Assert::AreEqual(size_t(1), otherParseMaster.Helpers().Size());
				Assert::AreEqual(sharedData.get(), otherParseMaster.GetSharedData()->As<JsonTestParseHelper::SharedData>());
				Assert::AreEqual(&otherParseMaster, sharedData->GetJsonParseCoordinator());
			}

			{
				JsonParseCoordinator parseMaster(sharedData);
				parseMaster.AddHelper(testParseHelper);

				std::shared_ptr<JsonTestParseHelper::SharedData> otherSharedData = std::make_shared<JsonTestParseHelper::SharedData>();
				JsonParseCoordinator otherParseMaster(otherSharedData);
				otherParseMaster = std::move(parseMaster);
				Assert::AreEqual(size_t(1), otherParseMaster.Helpers().Size());
				Assert::AreEqual(sharedData.get(), otherParseMaster.GetSharedData()->As<JsonTestParseHelper::SharedData>());
				Assert::AreEqual(&otherParseMaster, sharedData->GetJsonParseCoordinator());
			}
		}

		TEST_METHOD(IntegerParsing)
		{
			string inputString = R"({ "integer" : 100 })";

			std::shared_ptr<JsonTestParseHelper::SharedData> sharedData = std::make_shared<JsonTestParseHelper::SharedData>();
			std::shared_ptr<JsonTestParseHelper> integerParseHelper = std::make_shared<JsonTestParseHelper>();
			JsonParseCoordinator parseMaster(sharedData);
			parseMaster.AddHelper(integerParseHelper);

			parseMaster.DeserializeObject(inputString);
			Assert::AreEqual(size_t(1), sharedData->Data.Size());
			Assert::AreEqual(100, sharedData->Data.Front());
			Assert::AreEqual(size_t(0), sharedData->Depth());
			Assert::AreEqual(size_t(1), sharedData->MaxDepth);
		}

		TEST_METHOD(IntegerArrayParsing)
		{
			std::string inputString = R"({ "integer" : [10, 20, 30, 40] })";

			std::shared_ptr<JsonTestParseHelper::SharedData> sharedData = std::make_shared<JsonTestParseHelper::SharedData>();
			std::shared_ptr<JsonTestParseHelper> integerParseHelper = std::make_shared<JsonTestParseHelper>();
			JsonParseCoordinator parseMaster(sharedData);
			parseMaster.AddHelper(integerParseHelper);

			parseMaster.DeserializeObject(inputString);
			Assert::AreEqual(size_t(4), sharedData->Data.Size());
			Assert::AreEqual(10, sharedData->Data[0]);
			Assert::AreEqual(20, sharedData->Data[1]);
			Assert::AreEqual(30, sharedData->Data[2]);
			Assert::AreEqual(40, sharedData->Data[3]);
			Assert::AreEqual(size_t(0), sharedData->Depth());
			Assert::AreEqual(size_t(1), sharedData->MaxDepth);
		}

		TEST_METHOD(IntegerObjectParsing)
		{
			std::string inputString = R"({ "Object": 
											{
												"integer" : 100
											}
		
										})";

			std::shared_ptr<JsonTestParseHelper::SharedData> sharedData = std::make_shared<JsonTestParseHelper::SharedData>();
			std::shared_ptr<JsonTestParseHelper> integerParseHelper = std::make_shared<JsonTestParseHelper>();
			JsonParseCoordinator parseMaster(sharedData);
			parseMaster.AddHelper(integerParseHelper);

			/*parseMaster.DeserializeObject(inputString);
			Assert::AreEqual(size_t(1), sharedData->Data.Size());
			Assert::AreEqual(100, sharedData->Data.Front());
			Assert::AreEqual(size_t(0), sharedData->Depth());
			Assert::AreEqual(size_t(2), sharedData->MaxDepth);*/
		}

		TEST_METHOD(AddRemoveHelper)
		{
			std::shared_ptr<JsonTestParseHelper::SharedData> sharedData = std::make_shared<JsonTestParseHelper::SharedData>();
			JsonParseCoordinator parseMaster(sharedData);

			std::shared_ptr<JsonTestParseHelper> testParseHelper = std::make_shared<JsonTestParseHelper>();
			Assert::AreEqual(size_t(0), parseMaster.Helpers().Size());
			parseMaster.AddHelper(testParseHelper);
			Assert::AreEqual(size_t(1), parseMaster.Helpers().Size());

			Assert::ExpectException<exception>([&parseMaster, &testParseHelper] { parseMaster.AddHelper(testParseHelper); });

			parseMaster.RemoveHelper(testParseHelper);
			Assert::AreEqual(size_t(0), parseMaster.Helpers().Size());
		}

		TEST_METHOD(FallingThroughScene)
		{
			const string inputString = R"(
			{
				"Test" : 100
			})"s;

			std::shared_ptr<JsonTestParseHelper::SharedData> sharedData = std::make_shared<JsonTestParseHelper::SharedData>();
			std::shared_ptr<JsonTestParseHelper> integerParseHelper = std::make_shared<JsonTestParseHelper>();
			JsonParseCoordinator parseMaster(sharedData);
			parseMaster.AddHelper(integerParseHelper);

			parseMaster.DeserializeObject(inputString);

			Assert::AreEqual(size_t(0), sharedData->Depth());
			Assert::AreEqual(size_t(0), sharedData->MaxDepth);
		}

		TEST_METHOD(Clone)
		{
			std::shared_ptr<JsonTestParseHelper::SharedData> sharedData = std::make_shared<JsonTestParseHelper::SharedData>();
			std::shared_ptr<JsonTestParseHelper> integerParseHelper = std::make_shared<JsonTestParseHelper>();
			JsonParseCoordinator parseMaster(sharedData);
			parseMaster.AddHelper(integerParseHelper);

			auto clone = parseMaster.Clone();
			Assert::AreNotEqual(&*clone, &parseMaster);
			Assert::AreEqual(size_t(1), clone->Helpers().Size());

			auto clonedHelper = clone->Helpers()[0]->As<JsonTestParseHelper>();
			Assert::IsNotNull(clonedHelper);
			Assert::AreNotEqual(clonedHelper, parseMaster.Helpers()[0]->As<JsonTestParseHelper>());
			Assert::AreNotSame(clone->GetSharedData(), parseMaster.GetSharedData());


			clone->RemoveHelper(clone->Helpers()[0]);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState JsonParseCoordinatorTests::_startMemState;
}