#include "pch.h"
#include "Foo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitsTestsLibraryDesktop
{
	TEST_CLASS(FooTests)
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

		TEST_METHOD(Constructor)
		{
			const int data = 10;
			Foo f{ data };
			Foo g{ 10 };
			Assert::AreEqual(f, g);
		}

		TEST_METHOD(Data)
		{
			int data = 10;
			Foo f{ data };
			Assert::AreEqual(data, f.Data());

			const int constData = 20;
			Foo g{ constData };
			Assert::AreEqual(constData, g.Data());
		}

		TEST_METHOD(CopySemantics)
		{
			Foo f{ 10 };
			Foo h{ f };
			Assert::AreEqual(f.Data(), h.Data());
			Assert::AreEqual(h.Data(), 10);

			Foo g{ 24 };
			g = f;
			Assert::AreEqual(g.Data(), f.Data());
			Assert::AreEqual(g.Data(), 10);
			
			Foo i{ 24 };
			Foo j{ 24 };
			i = j;
			Assert::AreEqual(i.Data(), j.Data());
		}

		TEST_METHOD(EqualitySemantics)
		{
			Foo f{ 40 };
			Foo g{ 30 };

			Assert::AreEqual(f != g, true);
			Assert::AreEqual(f == g, false);

			Foo h{ 20 };
			Foo i{ 20 };

			Assert::AreEqual(h == i, true);
			Assert::AreEqual(h != i, false);
		}


	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState FooTests::_startMemState;
}