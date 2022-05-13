#include "pch.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitsTestsLibraryDesktop
{
	TEST_CLASS(FactoryTests)
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

		TEST_METHOD(ProductCreation)
		{
			RTTI* rtti = Factory<RTTI>::Create("Foo"s);
			Assert::IsNull(rtti);

			{
				FooFactory foofactory;

				rtti = Factory<RTTI>::Create("Foo"s);
				Assert::IsNotNull(rtti);
				
				const Factory<RTTI>* rttiFactoryptr = &foofactory;
				Assert::AreEqual(rttiFactoryptr, Factory<RTTI>::Find("Foo"s));
				const Factory<RTTI>* nullDummy = nullptr;
				Assert::AreEqual(nullDummy, Factory<RTTI>::Find("Bar"s));

				Foo* foo = rtti->As<Foo>();
				Assert::IsNotNull(foo);

				Assert::ExpectException<std::runtime_error>([] { FooFactory dupliacteFooFactory; });

				delete foo;
			}

			rtti = Factory<RTTI>::Create("Foo"s);
			Assert::IsNull(rtti);
		}


	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState FactoryTests::_startMemState;
}