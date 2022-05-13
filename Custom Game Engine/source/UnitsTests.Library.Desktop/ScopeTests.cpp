#include "pch.h"
#include "Datum.h"
#include "Scope.h"
#include "Foo.h"
#include <limits>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitsTestsLibraryDesktop
{
	TEST_CLASS(ScopeTests)
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
			Scope scope;
			const string a = "A"s;
			const string b = "B"s;
			const string c = "C"s; 

			Datum& aDatum = scope.Append(a);
			aDatum;
		}

		TEST_METHOD(AppendScope)
		{
			Scope scope;
			const string a = "A"s;
			const string b = "B"s;
			const string c = "C"s;

			Datum& aDatum = scope.Append(a);
			aDatum = "Hello World!"s;
			Datum& bDatum = scope.Append(b);
			bDatum = glm::vec4(5);
			Scope& cScope = scope.AppendScope(c);
			Assert::AreEqual(size_t(3), scope.Size());

			auto [cDatum, datumIndex] = scope.FindContainedScope(cScope);
			Assert::IsNotNull(cDatum);
			datumIndex;
			
			auto [nullDatum, datumZeroIndex] = cScope.FindContainedScope(scope);
			Assert::IsNull(nullDatum);
			Assert::AreEqual(size_t(0), datumZeroIndex);

			const string d = "D"s;
			Datum& dDatum = cScope.Append(d);
			dDatum = 10;

			Scope* foundScope;
			Datum* foundDatum;
			foundDatum = cScope.Search(a, foundScope);
			Assert::AreEqual(aDatum, *foundDatum);

			cScope.Clear();
			Assert::IsTrue(cScope.IsEmpty());
		}

		TEST_METHOD(CopySemantics)
		{
			// Copy Constructor
			{
				Scope scope;
				const string a = "A"s;
				const string b = "B"s;
				const string c = "C"s;

				Datum& aDatum = scope.Append(a);
				aDatum = "Hello World"s;
				Datum& bDatum = scope.Append(b);
				bDatum = glm::vec4(5);
				Scope& cScope = scope.AppendScope(c);
				Assert::AreEqual(size_t(3), scope.Size());

				auto [cDatum, datumIndex] = scope.FindContainedScope(cScope);
				Assert::IsNotNull(cDatum);
				Assert::AreEqual(cScope, cDatum->GetTable());

 				Scope copiedScope = scope;
				Assert::IsFalse(scope.IsEmpty());
				Assert::AreEqual(size_t(3), scope.Size());

				Assert::AreEqual(size_t(3), copiedScope.Size());
				Datum* foundDatum = copiedScope.Find(a);
				Assert::AreEqual(aDatum.GetString(), foundDatum->GetString());
				Assert::AreEqual(scope.Find(a)->GetString(), foundDatum->GetString());

				foundDatum = copiedScope.Find(b);
				Assert::AreEqual(bDatum.GetVector(), foundDatum->GetVector());
				Assert::AreEqual(scope.Find(b)->GetVector(), foundDatum->GetVector());


				foundDatum = copiedScope.Find(c);
				Assert::AreEqual(foundDatum->GetTable().GetParent(), &copiedScope);
				Assert::AreEqual(cDatum->GetTable(), foundDatum->GetTable());
				Assert::AreEqual(cScope, foundDatum->GetTable());
			}

			// Copy assignment
			{
				Scope scope;
				const string a = "A"s;
				const string b = "B"s;
				const string c = "C"s;

				Datum& aDatum = scope.Append(a);
				aDatum = "Hello World"s;
				Datum& bDatum = scope.Append(b);
				bDatum = glm::vec4(5);
				Scope& cScope = scope.AppendScope(c);
				Assert::AreEqual(size_t(3), scope.Size());

				auto [cDatum, datumIndex] = scope.FindContainedScope(cScope);
				Assert::IsNotNull(cDatum);
				Assert::AreEqual(cScope, cDatum->GetTable());

				Scope copiedScope;
				copiedScope = scope;
				Assert::IsFalse(scope.IsEmpty());
				Assert::AreEqual(size_t(3), scope.Size());

				Assert::AreEqual(size_t(3), copiedScope.Size());
				Datum* foundDatum = copiedScope.Find(a);
				Assert::AreEqual(aDatum.GetString(), foundDatum->GetString());
				Assert::AreEqual(scope.Find(a)->GetString(), foundDatum->GetString());

				foundDatum = copiedScope.Find(b);
				Assert::AreEqual(bDatum.GetVector(), foundDatum->GetVector());
				Assert::AreEqual(scope.Find(b)->GetVector(), foundDatum->GetVector());


				foundDatum = copiedScope.Find(c);
				Assert::AreEqual(foundDatum->GetTable().GetParent(), &copiedScope);
				Assert::AreEqual(cDatum->GetTable(), foundDatum->GetTable());
				Assert::AreEqual(cScope, foundDatum->GetTable());
			}
		}
		TEST_METHOD(MoveConstructor)
		{
			// Move constructor
			{
				Scope scope;
				const string a = "A"s;
				const string b = "B"s;
				const string c = "C"s;

				Datum& aDatum = scope.Append(a);
				aDatum = "Hello World!"s;
				Datum& bDatum = scope.Append(b);
				bDatum = glm::vec4(5);
				Scope& cScope = scope.AppendScope(c);
				Assert::AreEqual(size_t(3), scope.Size());

				auto [cDatum, datumIndex] = scope.FindContainedScope(cScope);
				Assert::IsNotNull(cDatum);
				Assert::AreEqual(cScope, cDatum->GetTable());

				Scope movedScope = std::move(scope);
				Assert::IsTrue(scope.IsEmpty());

				Assert::AreEqual(size_t(3), movedScope.Size());
				Datum* foundDatum = movedScope.Find(a);
				Assert::AreEqual(&aDatum, foundDatum);

				foundDatum = movedScope.Find(b);
				Assert::AreEqual(&bDatum, foundDatum);

				foundDatum = movedScope.Find(c);
				Assert::AreEqual(cDatum, foundDatum);
				Assert::AreEqual(foundDatum->GetTable().GetParent(), &movedScope);

				Scope* reparentedScope = new Scope(std::move(cScope));
				reparentedScope;
				Assert::IsTrue(cScope.IsEmpty());
				Assert::IsNull(cScope.GetParent());
				Assert::AreEqual(&movedScope, reparentedScope->GetParent());
				foundDatum = movedScope.Find(c);
				Assert::AreEqual(cDatum, foundDatum);
				Assert::AreEqual(cDatum, movedScope.FindContainedScope(*reparentedScope).first);

				delete& cScope;
			}

			// Move assignment
			{
				Scope scope;
				const string a = "A"s;
				const string b = "B"s;
				const string c = "C"s;

				Datum& aDatum = scope.Append(a);
				aDatum = "Hello World!"s;
				Datum& bDatum = scope.Append(b);
				bDatum = glm::vec4(5);
				Scope& cScope = scope.AppendScope(c);
				Assert::AreEqual(size_t(3), scope.Size());

				auto [cDatum, datumIndex] = scope.FindContainedScope(cScope);
				Assert::IsNotNull(cDatum);
				Assert::AreEqual(cScope, cDatum->GetTable());
				Assert::AreEqual(scope.AppendScope(c), cDatum->GetTable());

				Scope movedScope;
				movedScope = std::move(scope);
				Assert::IsTrue(scope.IsEmpty());

				Assert::AreEqual(size_t(3), movedScope.Size());
				Datum* foundDatum = movedScope.Find(a);
				Assert::AreEqual(&aDatum, foundDatum);

				foundDatum = movedScope.Find(b);
				Assert::AreEqual(&bDatum, foundDatum);
				Assert::AreEqual(&bDatum, &movedScope.Append(b));

				foundDatum = movedScope.Find(c);
				Assert::AreEqual(cDatum, foundDatum);
				Assert::AreEqual(foundDatum->GetTable().GetParent(), &movedScope);

				Scope* reparentedScope = new Scope();
				*reparentedScope = std::move(cScope);
				Assert::IsTrue(cScope.IsEmpty());
				Assert::IsNull(cScope.GetParent());
				Assert::AreEqual(&movedScope, reparentedScope->GetParent());
				foundDatum = movedScope.Find(c);
				Assert::AreEqual(cDatum, foundDatum);
				Assert::AreEqual(cDatum, movedScope.FindContainedScope(*reparentedScope).first);

				delete& cScope;
			}
		}

		TEST_METHOD(EqualityOperator)
		{
			{
				const string a = "A"s;
				const string b = "B"s;
				const string c = "C"s;

				Scope scope1;
				Datum& aDatum1 = scope1.Append(a);
				aDatum1 = "Hello World!"s;
				Datum& bDatum1 = scope1.Append(b);
				bDatum1 = glm::vec4(5);

				Scope scope2;
				Datum& aDatum2 = scope2.Append(a);
				aDatum2 = "Hello World!"s;
				Datum& bDatum2 = scope2.Append(b);
				bDatum2 = glm::mat4(5);

				Assert::IsTrue(scope1 != scope2);
			}
			
			{
				const string a = "A"s;
				const string b = "B"s;
				const string c = "C"s;

				Scope scope1;
				Datum& aDatum1 = scope1.Append(a);
				aDatum1 = "Hello World!"s;
				Datum& bDatum1 = scope1.Append(b);
				bDatum1 = glm::vec4(5);
				Scope& cScope1 = scope1.AppendScope(c);
				Assert::AreEqual(size_t(3), scope1.Size());
				cScope1;

				Scope scope2;
				Datum& aDatum2 = scope2.Append(a);
				aDatum2 = "Hello World!"s;
				Datum& bDatum2 = scope2.Append(b);
				bDatum2 = glm::vec4(5);
				Scope& cScope2 = scope2.AppendScope(c);
				Assert::AreEqual(size_t(3), scope2.Size());
				cScope2;

				Assert::IsTrue(scope1 == scope2);
				Assert::IsFalse(scope1 != scope2);

				const string d = "D"s;
				Datum& newInsert = cScope1.Append(d);
				newInsert = "Unique!";
				Assert::IsTrue(scope1 != scope2);
				Assert::IsFalse(scope1 == scope2);

				Datum& newInsert2 = scope1.Append(d);
				newInsert2 = 10;
				Assert::IsTrue(scope1 != scope2);
				Assert::IsFalse(scope1 == scope2);
			}
		}

		TEST_METHOD(AdoptOrphanAndAncestory)
		{
			const string a = "A"s;
			const string b = "B"s;
			const string c = "C"s;

			Scope scope1;
			Datum& aDatum1 = scope1.Append(a);
			aDatum1 = "Hello World!"s;
			Datum& bDatum1 = scope1.Append(b);
			bDatum1 = glm::vec4(5);
			Scope& cScope1 = scope1.AppendScope(c);
			Assert::AreEqual(size_t(3), scope1.Size());
			cScope1;

			Scope scope2;
			Assert::IsNull(scope2.GetParent());
			const string d = "D"s;
			Datum& newInsert = scope2.Append(d);
			newInsert = 10;
			scope1.Adopt(scope2, d);
			Assert::AreEqual(size_t(4), scope1.Size());
			Assert::AreEqual(&scope1, scope2.GetParent());
			
			/*scope1.Adopt(scope2, d);
			Assert::AreEqual(size_t(4), scope1.Size());*/

			Assert::ExpectException<std::runtime_error>([&scope1, &scope2, &d] {scope2.Adopt(scope1, d); });


			scope2.Adopt(cScope1, c);
			Assert::AreEqual(size_t(4), scope1.Size());
		}

		TEST_METHOD(IndexOperator)
		{
			const string a = "A"s;
			const string b = "B"s;
			const string c = "C"s;

			Scope scope1;
			Datum& aDatum1 = scope1.Append(a);
			aDatum1 = "Hello World!"s;
			Datum& bDatum1 = scope1.Append(b);
			bDatum1 = glm::vec4(5);
			Scope& cScope1 = scope1.AppendScope(c);
			Assert::AreEqual(size_t(3), scope1.Size());
			cScope1;

			Assert::AreEqual(aDatum1, scope1[a]);
			Assert::AreEqual(bDatum1, scope1[1]);
		}
	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState ScopeTests::_startMemState;
}