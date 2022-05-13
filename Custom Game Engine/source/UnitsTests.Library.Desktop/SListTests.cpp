#include "pch.h"
#include "SList.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitsTestsLibraryDesktop
{
	TEST_CLASS(SListTests)
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
			SList<Foo> list;
			Assert::AreEqual((size_t)0, list.Size());
		}

		TEST_METHOD(PushFront)
		{
			Foo a{ 13 };
			Foo b{ 27 };
			SList<Foo> list;
			list.PushFront(a);
			list.PushFront(b);
			Assert::AreEqual((size_t)2, list.Size());
			Assert::AreEqual(b, list.Front());
			Assert::AreEqual(a, list.Back());
		}

		TEST_METHOD(Front)
		{
			Foo a{ 13 };
			Foo b{ 23 };
			SList<Foo> list;
			auto expression = [&list]() {list.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);
			list.PushFront(a);
			list.PushFront(b);
			Assert::AreEqual((size_t)2, list.Size());
			Assert::AreEqual(b, list.Front());

			const SList<Foo> otherList;
			auto expression2 = [&otherList]() {otherList.Front(); };
			Assert::ExpectException<std::runtime_error>(expression2);		
			Assert::AreEqual(otherList.IsEmpty(), true);
			Assert::AreEqual((size_t)0, otherList.Size());
		}

		TEST_METHOD(PushBack)
		{
			Foo a{ 13 };
			Foo b{ 27 };
			SList<Foo> list;
			list.PushBack(a);
			list.PushBack(b);
			Assert::AreEqual((size_t)2, list.Size());
			Assert::AreEqual(b, list.Back());
			Assert::AreEqual(a, list.Front());
		}

		TEST_METHOD(Back)
		{
			Foo a{ 243 };
			Foo b{ 142 };
			SList<Foo> list;
			auto expression = [&list]() {list.Back(); };
			Assert::ExpectException<std::runtime_error>(expression);
			list.PushBack(a);
			list.PushBack(b);
			Assert::AreEqual((size_t)2, list.Size());
			Assert::AreEqual(b, list.Back());

			const SList<Foo> otherList;
			auto expression2 = [&otherList]() {otherList.Back(); };
			Assert::ExpectException<std::runtime_error>(expression2);
			Assert::AreEqual(otherList.IsEmpty(), true);
			Assert::AreEqual((size_t)0, otherList.Size());
		}

		TEST_METHOD(Size)
		{
			Foo a{ 2345 };
			Foo b{ 42 };
			SList<Foo> list;
			list.PushFront(a);
			list.PushBack(b);
			Assert::AreEqual((size_t)2, list.Size());
		}

		TEST_METHOD(IsEmpty)
		{
			Foo a{ 13 };
			SList<Foo> list;
			Assert::AreEqual(true, list.IsEmpty());
			list.PushBack(a);
			Assert::AreEqual(false, list.IsEmpty());
		}

		TEST_METHOD(PopFront)
		{
			Foo a{ 234 };
			Foo b{ 45 };
			Foo c{ 23 };
			SList<Foo> list;

			list.PushBack(a);
			list.PushFront(b);
			list.PushFront(c);
			Assert::AreEqual(c, list.Front());
			
			list.PopFront();
			Assert::AreEqual(b, list.Front());
			Assert::AreEqual(a, list.Back());
			
			list.PopFront();
			Assert::AreEqual(a, list.Front());

			list.PopFront();
			Assert::AreEqual((size_t)0, list.Size());

			auto expression = [&list]() {list.PopFront(); };
			Assert::ExpectException<std::runtime_error>(expression);

			Foo d{ 74 };
			list.PushFront(d);
			Assert::AreEqual(d, list.Front());
			Assert::AreEqual(d, list.Back());
			Assert::AreEqual((size_t)1, list.Size());
		}

		TEST_METHOD(PopBack)
		{
			Foo a{ 234 };
			Foo b{ 45 };
			Foo c{ 23 };
			SList<Foo> list;

			list.PushBack(a);
			list.PushFront(b);
			list.PushFront(c);
			Assert::AreEqual(a, list.Back());

			list.PopBack();
			Assert::AreEqual(b, list.Back());
			Assert::AreEqual(c, list.Front());

			list.PopBack();
			Assert::AreEqual(c, list.Front());

			list.PopBack();
			Assert::AreEqual((size_t)0, list.Size());

			auto expression = [&list]() {list.PopBack(); };
			Assert::ExpectException<std::runtime_error>(expression);

			Foo d{ 74 };
			list.PushFront(d);
			Assert::AreEqual(d, list.Front());
			Assert::AreEqual(d, list.Back());
			Assert::AreEqual((size_t)1, list.Size());
		}

		TEST_METHOD(Clear)
		{
			Foo a{ 234 };
			Foo b{ 45 };
			Foo c{ 23 };
			SList<Foo> list;
			list.PushFront(c);
			list.PushBack(b);
			list.PushBack(a);

			Assert::AreEqual((size_t)3, list.Size());
			list.Clear();
			Assert::AreEqual((size_t)0, list.Size());

			list.PushFront(a);
			Assert::AreEqual(a, list.Front());
			list.PushBack(c);
			Assert::AreEqual(c, list.Back());
			Assert::AreEqual((size_t)2, list.Size());
		}

		TEST_METHOD(CopySemantics)
		{
			Foo a{ 234 };
			Foo b{ 45 };
			Foo c{ 23 };
			SList<Foo> list;
			list.PushFront(a);
			list.PushFront(b);
			list.PushFront(c);

			SList<Foo> listConstructorCopy{ list };
			Assert::AreEqual(listConstructorCopy.Front(), list.Front());
			Assert::AreEqual(listConstructorCopy.Back(), list.Back());
			Assert::AreEqual(listConstructorCopy.Front(), c);
			Assert::AreEqual(listConstructorCopy.Back(), a);

			SList<Foo> listAssignmentCopy;
			listAssignmentCopy = list;
			Assert::AreEqual(listAssignmentCopy.Front(), list.Front());
			Assert::AreEqual(listAssignmentCopy.Back(), list.Back());
			Assert::AreEqual(listAssignmentCopy.Front(), c);
			Assert::AreEqual(listAssignmentCopy.Back(), a);
			Assert::AreEqual(listAssignmentCopy.Size(), list.Size());

			listConstructorCopy.PopBack();
			listAssignmentCopy.PopBack();
			listConstructorCopy.PopFront();
			listAssignmentCopy.PopFront();
			Assert::AreEqual(listAssignmentCopy.Front(), listConstructorCopy.Front());
			Assert::AreEqual(listAssignmentCopy.Back(), listConstructorCopy.Back());

			const SList<Foo> constListCopy{ list };
			Assert::AreEqual(c, constListCopy.Front());
			Assert::AreEqual(a, constListCopy.Back());
			Assert::AreEqual(list.Size(), constListCopy.Size());
		}

		TEST_METHOD(Iterator)
		{
			{
				SList<Foo> list;
				Assert::AreEqual(list.end(), list.begin());

				Foo a(10);
				list.PushBack(a);
				Assert::IsTrue(list.end() != list.begin());
			}

			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);
				SList<Foo> list{ a, b, c };

				size_t count = 0;
				for (auto it = list.begin(); it != list.end(); ++it, ++count)
				{
					;
				}
				Assert::AreEqual(list.Size(), count);

				count = 0;
				for (const Foo& value : list)
				{
					value;
					count++;
				}
				Assert::AreEqual(list.Size(), count);
			}
		}

		TEST_METHOD(ConstIterator)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			const SList<Foo> list{ a, b, c };

			size_t count = 0;
			for (auto it = list.begin(); it != list.end(); it++, ++count)
			{
				;
			}
			Assert::AreEqual(list.Size(), count);

			count = 0;
			for (const Foo& value : list)
			{
				value;
				count++;
			}
			Assert::AreEqual(list.Size(), count);
		}

		TEST_METHOD(IteratorCBeginCEnd)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			SList<Foo> list{ a, b, c };

			Assert::AreEqual(a, *list.cbegin());
			size_t count = 0;
			for (auto it = list.cbegin(); it != list.cend(); ++it, ++count)
			{
				;
			}
			Assert::AreEqual(list.Size(), count);
		}

		TEST_METHOD(IteratorFind)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			SList<Foo> list{ a, b, c };

			auto foundIt = list.Find(a);
			Assert::AreEqual(*foundIt, a);
			foundIt = list.Find(b);
			Assert::AreEqual(*foundIt, b);
			foundIt = list.Find(c);
			Assert::AreEqual(*foundIt, c);

			const Foo d(40);
			foundIt = list.Find(d);
			auto beggIt = list.begin();
			auto endIt = list.end();
			Assert::AreNotEqual(foundIt, beggIt);
			Assert::AreEqual(foundIt, endIt);
			

			const SList<Foo> constList{ a, b, c };

			auto constFoundIt = constList.Find(a);
			Assert::AreEqual(*constFoundIt, a);
			constFoundIt = constList.Find(b);
			Assert::AreEqual(*constFoundIt, b);
			constFoundIt = constList.Find(c);
			Assert::AreEqual(*constFoundIt, c);

			constFoundIt = constList.Find(d);
			auto constbeggIt = constList.begin();
			auto constendIt = constList.end();
			Assert::AreNotEqual(constFoundIt, constbeggIt);
			Assert::AreEqual(constFoundIt, constendIt);
		}

		TEST_METHOD(IteratorIncrementFail)
		{
			Assert::ExpectException<std::runtime_error>([] {SList<Foo>::Iterator it; ++it; });
		}

		TEST_METHOD(IteratorDereferenceFail)
		{
			Assert::ExpectException<std::runtime_error>([] {SList<Foo>::Iterator it; *it; });
		}

		TEST_METHOD(ConstIteratorIncrementFail)
		{
			Assert::ExpectException<std::runtime_error>([] {SList<Foo>::ConstIterator it; ++it; });
		}

		TEST_METHOD(InsertAfter)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			SList<Foo> list{ a, b, c };
			Assert::AreEqual(a, *list.begin());

			const Foo d(40);
			auto it = list.begin();
			list.InsertAfter(d, it);
			
			int counter = 0;
			for (const Foo& value : list)
			{
				switch (counter)
				{
				case 0:
					Assert::AreEqual(value, a);
					break;
				case 1:
					Assert::AreEqual(value, d);
					break;
				case 2:
					Assert::AreEqual(value, b);
					break;
				case 3:
					Assert::AreEqual(value, c);
					break;
				default:
					break;
				}

				++counter;
			}

			SList<Foo> randomList;
			auto randomIt = randomList.begin();
			Assert::ExpectException<std::runtime_error>([&list, &d, &randomIt] { list.InsertAfter(d, randomIt); });

			SList<Foo> singleList{ d };
			singleList.InsertAfter(b, singleList.begin());
			Assert::AreEqual(singleList.Back(), b);
		}

		TEST_METHOD(Remove)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			SList<Foo> list{ a, b, c };

			Assert::AreEqual(size_t(3), list.Size());
			Assert::IsTrue(list.Remove(a));
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(list.end(), list.Find(a));
			Assert::IsFalse(list.Remove(a));

			Assert::IsTrue(list.Remove(b));
			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(list.end(), list.Find(b));
			Assert::IsFalse(list.Remove(b));

			Assert::IsTrue(list.Remove(c));
			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual(list.end(), list.Find(c));
			Assert::IsFalse(list.Remove(c));
		}

		private:
			static _CrtMemState _startMemState;
	};

	_CrtMemState SListTests::_startMemState;

}
