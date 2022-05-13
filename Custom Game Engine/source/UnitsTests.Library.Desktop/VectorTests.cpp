#include "pch.h"
#include "Vector.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include <limits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitsTestsLibraryDesktop
{
	TEST_CLASS(VectorTests)
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
			Vector<UnitsTestsLibraryDesktop::Foo> vector;
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(10), vector.Capacity());
		}
		
		TEST_METHOD(PushBack)
		{
			const Foo a{ 13 };
			const Foo b{ 2345 };
			Vector<Foo> vector;
			vector.PushBack(a);
			vector.PushBack(b);
			Assert::AreEqual(vector.Front(), a);
			Assert::AreEqual(vector.Back(), b);
			Assert::AreEqual(vector.Size(), size_t(2));

			vector.Clear();
		}

		TEST_METHOD(PopBack)
		{
			const Foo a{ 12 };
			const Foo b{ 134 };
			const Foo c{ 452 };
			Vector<Foo> vector;

			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			Assert::AreEqual(vector.Front(), a);
			Assert::AreEqual(vector.Back(), c);

			vector.PopBack();
			Assert::AreEqual(vector.Front(), a);
			Assert::AreEqual(vector.Back(), b);

			vector.PopBack();
			Assert::AreEqual(vector.Front(), a);
			Assert::AreEqual(vector.Back(), a);

			vector.PopBack();
			auto expression = [&vector]() {vector.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);
			
			auto expression2 = [&vector]() {vector.Back(); };
			vector.PopBack();
			Assert::ExpectException<std::runtime_error>(expression2);
		}

		TEST_METHOD(IndexingFail)
		{
			const Foo a{ 12 };
			const Foo b{ 31 };
			const Foo c{ 124 };
			Vector<Foo> vector{ a, b, c };
			Assert::ExpectException<std::runtime_error>([&vector] { vector[10]; });

			const Vector<Foo> constVector{ a, b, c };
			Assert::ExpectException<std::runtime_error>([&constVector] { constVector[10]; });
		}
		TEST_METHOD(Front)
		{
			const Foo a{ 10 };
			const Foo b{ 23 };
			Vector<Foo> vector;
			auto expression = [&vector]() {vector.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);
			vector.PushBack(a);
			vector.PushBack(b);
			Assert::AreEqual(size_t(2), vector.Size());
			Assert::AreEqual(vector.Front(), a);

			const Vector<Foo> constVect{ vector };
			Assert::AreEqual(size_t(2), constVect.Size());
			Assert::AreEqual(constVect.Front(), a);
			const Vector<Foo> emptyVect;
			Assert::ExpectException<std::runtime_error>([&emptyVect] { emptyVect.Front(); });
		}

		TEST_METHOD(Back)
		{
			const Foo a{ 10 };
			const Foo b{ 2344 };
			Vector<Foo> vector;
			auto expression = [&vector]() {vector.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);
			vector.PushBack(b);
			vector.PushBack(a);
			Assert::AreEqual(size_t(2), vector.Size());
			Assert::AreEqual(a, vector.Back());

			const Vector<Foo> constVect{ vector };
			Assert::AreEqual(size_t(2), constVect.Size());
			Assert::AreEqual(constVect.Back(), a);
			const Vector<Foo> emptyVect;
			Assert::ExpectException<std::runtime_error>([&emptyVect] { emptyVect.Back(); });
		}

		TEST_METHOD(IsEmpty)
		{
			const Foo a{ 10 };
			Vector<Foo> vector;
			Assert::AreEqual(vector.IsEmpty(), true);
			vector.PushBack(a);
			Assert::AreEqual(vector.IsEmpty(), false);
		}

		TEST_METHOD(Capacity)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			Vector<Foo> vector;
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(10), vector.Capacity());
			for (int i = 0; i < 10; i++)
			{
				vector.PushBack(a);
			}
			Assert::AreEqual(size_t(10), vector.Size());
			Assert::AreEqual(size_t(10), vector.Capacity());

			vector.PushBack(b);
			Assert::AreEqual(size_t(11), vector.Size());
			Assert::AreEqual(size_t(20), vector.Capacity());
		}

		TEST_METHOD(Clear)
		{
			const Foo a{ 134 };
			const Foo b{ 24 };
			const Foo c{ 234 };
			Vector<Foo> vector;

			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);
			Assert::AreEqual(vector.Front(), a);
			Assert::AreEqual(vector.Back(), c);
			Assert::AreEqual(vector.Size(), size_t(3));
			Assert::AreEqual(vector.Capacity(), size_t(10));

			vector.Clear();
			Assert::AreEqual(vector.Size(), size_t(0));
			Assert::AreEqual(vector.Capacity(), size_t(10));
			auto expression = [&vector]() {vector.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);
		}

		TEST_METHOD(Reserve)
		{
			Vector<Foo> vector;
			Assert::AreEqual(vector.Capacity(), size_t(10));
			Assert::ExpectException<std::runtime_error>([&vector] { vector.Reserve(std::numeric_limits<std::size_t>::max()); });

			vector.Reserve(size_t(34));
			Assert::AreEqual(vector.Capacity(), size_t(34));
			Assert::AreEqual(vector.Size(), size_t(0));

			auto expression = [&vector]() {vector.Reserve(3); };
			Assert::ExpectException<std::runtime_error>(expression);

		}

		TEST_METHOD(CopySemantics)
		{
			const Foo a{ 12 };
			const Foo b{ 234 };
			const Foo c{ 425 };
			Vector<Foo> vector;

			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			Vector<Foo> copyVector{ vector };
			Assert::AreEqual(vector.Front(), copyVector.Front());
			Assert::AreEqual(vector.Back(), copyVector.Back());
			Assert::AreEqual(a, copyVector.Front());
			Assert::AreEqual(b, copyVector.At(1));
			Assert::AreEqual(c, copyVector.At(2));
			Assert::AreEqual(vector.Size(), copyVector.Size());
			Assert::AreEqual(vector.Capacity(), copyVector.Capacity());

			Vector<Foo> copyAssignmentVector;
			copyAssignmentVector = vector;
			Assert::AreEqual(vector.Front(), copyAssignmentVector.Front());
			Assert::AreEqual(vector.Back(), copyAssignmentVector.Back());
			Assert::AreEqual(a, copyAssignmentVector.Front());
			Assert::AreEqual(b, copyAssignmentVector.At(1));
			Assert::AreEqual(c, copyAssignmentVector.At(2));
			Assert::AreEqual(vector.Size(), copyAssignmentVector.Size());
			Assert::AreEqual(vector.Capacity(), copyAssignmentVector.Capacity());

			vector.Clear();
			Assert::AreEqual(copyAssignmentVector.Size(), copyVector.Size());
			Assert::AreEqual(copyAssignmentVector.Capacity(), copyVector.Capacity());
			Assert::AreEqual(copyAssignmentVector.Front(), copyVector.Front());
			Assert::AreEqual(copyAssignmentVector.Back(), copyVector.Back());
		}

		TEST_METHOD(MoveSemantics)
		{
			const Foo a{ 12 };
			const Foo b{ 2354 };
			const Foo c{ 325 };
			Vector<Foo> vector;
			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			Vector<Foo> moveVector = std::move(vector);

			Assert::AreEqual(moveVector.Size(), size_t(3));
			Assert::AreEqual(moveVector.Capacity(), size_t(10));
			Assert::AreEqual(moveVector.Front(), a);
			Assert::AreEqual(moveVector.Back(), c);

			Assert::AreEqual(vector.Size(), size_t(0));
			Assert::AreEqual(vector.Capacity(), size_t(0));

			Vector<Foo> moveAssignment;
			moveAssignment = std::move(moveVector);

			Assert::AreEqual(moveAssignment.Size(), size_t(3));
			Assert::AreEqual(moveAssignment.Capacity(), size_t(10));
			Assert::AreEqual(moveAssignment.Front(), a);
			Assert::AreEqual(moveAssignment.Back(), c);

			Assert::AreEqual(moveVector.Size(), size_t(0));
			Assert::AreEqual(moveVector.Capacity(), size_t(0));
		}

		TEST_METHOD(Remove)
		{
			const Foo a{ 123 };
			const Foo b{ 34 };
			const Foo c{ 425 };
			Vector<Foo> vector;
			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			Assert::ExpectException<std::runtime_error>([&vector] { vector.Remove(10); });

			vector.Remove(0);
			Assert::AreEqual(vector.Front(), b);

			vector.Remove(vector.begin());
			Assert::AreEqual(vector.Front(), c);

			vector.Remove(0);
			Assert::AreEqual(vector.Size(), size_t(0));
		}

		TEST_METHOD(IteratorRemove)
		{
			const Foo a{ 123 };
			const Foo b{ 34 };
			const Foo c{ 425 };
			Vector<Foo> vector;
			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			Vector<Foo> otherVector;
			Assert::ExpectException<std::runtime_error>([&vector, &otherVector] { vector.Remove(otherVector.begin()); });

			Assert::ExpectException<std::runtime_error>([&vector] { vector.Remove(vector.end()); });

			vector.Remove(0);
			Assert::AreEqual(vector.Front(), b);

			vector.Remove(vector.begin());
			Assert::AreEqual(vector.Front(), c);

			vector.Remove(vector.begin());
			Assert::AreEqual(vector.Size(), size_t(0));
		}
		TEST_METHOD(Find)
		{
			const Foo a{ 1 };
			const Foo b{ 2 };
			const Foo c{ 3 };
			const Foo d{ 4 };
			Vector<Foo> vector{ a, b, c, d };

			size_t counter = 0;
			for (auto it = vector.begin(); it != vector.end(); ++it)
			{
				switch (counter)
				{
				case 0:
					Assert::AreEqual(it, vector.Find(a));
					break;

				case 1:
					Assert::AreEqual(it, vector.Find(b));
					break;

				case 2:
					Assert::AreEqual(it, vector.Find(c));
					break;

				case 3:
					Assert::AreEqual(it, vector.Find(d));
					break;
				}

				++counter;
			}

		}
		TEST_METHOD(Iterator)
		{
			{
				Vector<Foo> vector;
				Assert::AreEqual(vector.begin(), vector.end());

				const Foo a{ 1 };
				vector.PushBack(a);
				Assert::IsTrue(vector.begin() != vector.end());
			}

			{
				const Foo a{ 1 };
				const Foo b{ 2 };
				const Foo c{ 3 };
				Vector<Foo> vector{ a, b, c };

				size_t count = 0;
				for (auto it = vector.begin(); it != vector.end(); it++, ++count)
				{
					;
				}
				Assert::AreEqual(vector.Size(), count);

				count = 0;
				for (const Foo& value : vector)
				{
					value;
					count++;
				}
				Assert::AreEqual(vector.Size(), count);
			}
		}

		TEST_METHOD(ConstIterator)
		{
			const Foo a{ 1123 };
			const Foo b{ 1234 };
			const Foo c{ 42 };
			const Vector<Foo> vector{ a, b, c };

			size_t count = 0;
			for (auto it = vector.begin(); it != vector.end(); it++, ++count)
			{
				;
			}
			Assert::AreEqual(vector.Size(), count);

			count = 0;
			for (const Foo& value : vector)
			{
				value;
				count++;
			}
			Assert::AreEqual(vector.Size(), count);
		}

		TEST_METHOD(IteratorCbeginCend)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			Vector<Foo> vector{ a, b, c };

			Assert::AreEqual(a, *vector.cbegin());
			size_t count = 0;
			for (auto it = vector.cbegin(); it != vector.cend(); ++it, ++count)
			{
				;
			}
			Assert::AreEqual(vector.Size(), count);
		}

		TEST_METHOD(IteratorFind)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			Vector<Foo> vector{ a, b, c };

			auto foundIt = vector.Find(a);
			Assert::AreEqual(*foundIt, a);
			foundIt = vector.Find(b);
			Assert::AreEqual(*foundIt, b);
			foundIt = vector.Find(c);
			Assert::AreEqual(*foundIt, c);

			const Foo d(40);
			foundIt = vector.Find(d);
			auto beggIt = vector.begin();
			auto endIt = vector.end();
			Assert::AreNotEqual(foundIt, beggIt);
			Assert::AreEqual(foundIt, endIt);


			const Vector<Foo> constVector{ a, b, c };

			auto constFoundIt = constVector.Find(a);
			Assert::AreEqual(*constFoundIt, a);
			constFoundIt = constVector.Find(b);
			Assert::AreEqual(*constFoundIt, b);
			constFoundIt = constVector.Find(c);
			Assert::AreEqual(*constFoundIt, c);

			constFoundIt = constVector.Find(d);
			auto constbeggIt = constVector.begin();
			auto constendIt = constVector.end();
			Assert::AreNotEqual(constFoundIt, constbeggIt);
			Assert::AreEqual(constFoundIt, constendIt);
		}

		TEST_METHOD(IteratorIncrementFail)
		{
			Assert::ExpectException<std::runtime_error>([] {Vector<Foo>::Iterator it; ++it; });
			Assert::ExpectException<std::runtime_error>([] {Vector<Foo>::Iterator it; it++; });
		}

		TEST_METHOD(IteratorDereferenceFail)
		{
			Assert::ExpectException<std::runtime_error>([] {Vector<Foo>::Iterator it; *it; });
		}

		TEST_METHOD(ConstIteratorIncrementFail)
		{
			Assert::ExpectException<std::runtime_error>([] {Vector<Foo>::ConstIterator it; ++it; });
			Assert::ExpectException<std::runtime_error>([] {Vector<Foo>::ConstIterator it; it++; });
		}

		TEST_METHOD(ConstIteratorDereferenceFail)
		{
			Assert::ExpectException<std::runtime_error>([] {Vector<Foo>::ConstIterator it; *it; });
		}

		TEST_METHOD(Resize)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			Vector<Foo> vector{ a, b, c };
			Assert::AreEqual(vector.Size(), size_t(3));
			Assert::AreEqual(vector.Capacity(), size_t(3));
			
			vector.Resize(11);
			Assert::AreEqual(vector.Size(), size_t(11));
			Assert::AreEqual(vector.Capacity(), size_t(11));

			vector.Resize(2);
			Assert::AreEqual(vector.Size(), size_t(2));
			Assert::AreEqual(vector.Capacity(), size_t(2));
			Assert::AreEqual(vector.Front(), a);
			Assert::AreEqual(vector.Back(), b);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState VectorTests::_startMemState;
}