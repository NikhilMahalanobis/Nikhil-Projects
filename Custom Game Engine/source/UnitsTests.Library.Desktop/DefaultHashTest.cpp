#include "pch.h"
#include "HashMap.h"
#include "Foo.h"
#include <limits>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FieaGameEngine
{
	template<>
	struct DefaultHash<UnitsTestsLibraryDesktop::Foo>
	{
		std::size_t operator()(const UnitsTestsLibraryDesktop::Foo& value)
		{
			return abs(value.Data());
		}
	};
}

using namespace FieaGameEngine;

namespace UnitsTestsLibraryDesktop
{
	TEST_CLASS(DefaultHashTest)
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

		TEST_METHOD(IntegerHash)
		{
			int a = 10;
			int b = 20;
			int c = 10;

			DefaultHash<int> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}
		
		TEST_METHOD(StringHash)
		{
			string a = "Hello"s;
			string b = "Goodbye"s;
			string c(a);
			DefaultHash<string> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

		TEST_METHOD(CStringHash)
		{
			const char* a = "Hello";
			const char* b = "Goodbye";
			char c[6];
			strcpy_s(c, a);
			DefaultHash<char*> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

		TEST_METHOD(WideStringHash)
		{
			wstring a = L"Hello"s;
			wstring b = L"Goodbye"s;
			wstring c(a);
			DefaultHash<wstring> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

		TEST_METHOD(FooHash)
		{
			Foo a(10);
			Foo b(20);
			Foo c(10);
			DefaultHash<UnitsTestsLibraryDesktop::Foo> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState DefaultHashTest::_startMemState;
}