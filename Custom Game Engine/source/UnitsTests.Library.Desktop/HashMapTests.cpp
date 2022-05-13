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
	TEST_CLASS(HashMapTests)
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
			HashMap<Foo, int> map(1);
			Assert::AreEqual(map.Size(), size_t(0));
			Assert::AreEqual(map.Capacity(), size_t(1));
		}

		TEST_METHOD(Insert)
		{
			std::function<size_t(const Foo&)> hashFunctor = DefaultHash<Foo>();
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			std::pair<const Foo, int> pairA = make_pair(a, 10);
			std::pair<const Foo, int> pairB = make_pair(b, 20);
			std::pair<const Foo, int> pairC = make_pair(c, 30);

			HashMap<const Foo, int> map(10, hashFunctor);
			map.Insert(pairA);
			map.Insert(pairB);
			map.Insert(pairC);

			auto it = map.Find(a);
			auto& entry = *it;
			Assert::AreEqual(entry, pairA);
			Assert::AreEqual(*map.Find(b), pairB);
			Assert::AreEqual(*map.Find(c), pairC);

			const Foo d{ -30 };
			std::pair<const Foo, int> pairD = make_pair(d, 30);
			map.Insert(pairD);
			Assert::AreEqual(*(map.Find(d)), pairD);
			Assert::AreNotEqual(*(map.Find(d)), pairC);
			Assert::AreNotEqual(*(map.Find(c)), pairD);

		}

		TEST_METHOD(Iterator)
		{
			{
				HashMap<Foo, int> hashMap(10);
				Assert::AreEqual(hashMap.begin(), hashMap.end());

				const Foo a{ 1 };
				std::pair pairA = make_pair(a, 1);
				hashMap.Insert(pairA);
				Assert::IsTrue(hashMap.begin() != hashMap.end());
			}

			{
				const Foo a{ 1 };
				const Foo b{ 2 };
				const Foo c{ 3 };
				std::pair pairA = make_pair(a, 10);
				std::pair pairB = make_pair(b, 20);
				std::pair pairC = make_pair(c, 30);
				HashMap<Foo, int> hashMap{ pairA, pairB, pairC };

				size_t count = 0;
				for (auto it = hashMap.begin(); it != hashMap.end(); it++, ++count)
				{
					;
				}
				Assert::AreEqual(hashMap.Size(), count);

				count = 0;
				for (const auto& value : hashMap)
				{
					value;
					count++;
				}
				Assert::AreEqual(hashMap.Size(), count);

			}

		}

		TEST_METHOD(ConstIterator)
		{
			const Foo a{ 1123 };
			const Foo b{ 1234 };
			const Foo c{ 42 };
			std::pair pairA = make_pair(a, 10);
			std::pair pairB = make_pair(b, 20);
			std::pair pairC = make_pair(c, 30);
			HashMap<Foo, int> mappy{ pairA, pairB, pairC };
			
			const HashMap<Foo, int> hashyMap(mappy);

			size_t count = 0;
			for (auto it = hashyMap.begin(); it != hashyMap.end(); it++, ++count)
			{
				;
			}
			Assert::AreEqual(hashyMap.Size(), count);

			count = 0;
			for (auto it = hashyMap.begin(); it != hashyMap.end(); ++it, ++count)
			{
				it->first;
			}
			Assert::AreEqual(hashyMap.Size(), count);

			count = 0;
			for (const std::pair<Foo, int>& value : hashyMap)
			{
				value;
				count++;
			}
			Assert::AreEqual(hashyMap.Size(), count);

			HashMap<Foo, int>::ConstIterator constCopy(mappy.begin());
			constCopy == mappy.cbegin();
		}

		TEST_METHOD(IteratorCBeingCEnd)
		{
			const Foo a{ 1 };
			const Foo b{ 2 };
			const Foo c{ 3 };
			std::pair pairA = make_pair(a, 10);
			std::pair pairB = make_pair(b, 20);
			std::pair pairC = make_pair(c, 30);
			HashMap<Foo, int> hashMap{ pairA, pairB, pairC };

			size_t count = 0;
			for (auto it = hashMap.cbegin(); it != hashMap.cend(); ++it, ++count)
			{
				;
			}
			Assert::AreEqual(hashMap.Size(), count);
		}
		TEST_METHOD(IndexOperator)
		{
			const Foo a{ 1 };
			const Foo b{ 2 };
			const Foo c{ 3 };
			std::pair pairA = make_pair(a, 10);
			std::pair pairB = make_pair(b, 20);
			std::pair pairC = make_pair(c, 30);
			HashMap<Foo, int> hashMap{ pairA, pairB, pairC };

			Assert::AreEqual(hashMap[a], 10);
			Assert::AreEqual(hashMap[b], 20);
			Assert::AreEqual(hashMap[c], 30);

			const Foo d{ 4 };
			hashMap[d];
			Assert::AreEqual(hashMap[d], 0);

			const Foo e{ 5 };
			hashMap[e] = 50;
			Assert::AreEqual(hashMap[e], 50);
		}

		TEST_METHOD(At)
		{
			const Foo a{ 1 };
			const Foo b{ 2 };
			const Foo c{ 3 };
			const Foo d{ 4 };
			std::pair pairA = make_pair(a, 10);
			std::pair pairB = make_pair(b, 20);
			std::pair pairC = make_pair(c, 30);
			HashMap<Foo, int> hashMap{ pairA, pairB, pairC };

			Assert::AreEqual(hashMap.At(a), 10);
			Assert::AreEqual(hashMap.At(b), 20);
			Assert::AreEqual(hashMap.At(c), 30);
			Assert::ExpectException<std::runtime_error>([&hashMap, &d] { hashMap.At(d); });

			const HashMap<Foo, int> constHashy(hashMap);
			Assert::AreEqual(constHashy.At(a), 10);
			Assert::AreEqual(constHashy.At(b), 20);
			Assert::AreEqual(constHashy.At(c), 30);
			Assert::ExpectException<std::runtime_error>([&constHashy, &d] { constHashy.At(d); });
		}
		TEST_METHOD(ContainsKey)
		{
			const Foo a{ 1 };
			const Foo b{ 2 };
			const Foo c{ 3 };
			const Foo d{ 4 };
			std::pair pairA = make_pair(a, 10);
			std::pair pairB = make_pair(b, 20);
			std::pair pairC = make_pair(c, 30);
			HashMap<Foo, int> hashMap{ pairA, pairB, pairC };

			Assert::AreEqual(hashMap.ContainsKey(a), true);
			Assert::AreEqual(hashMap.ContainsKey(b), true);
			Assert::AreEqual(hashMap.ContainsKey(c), true);
			Assert::AreEqual(hashMap.ContainsKey(d), false);

			const HashMap<Foo, int> constHashy(hashMap);
			Assert::AreEqual(constHashy.ContainsKey(a), true);
			Assert::AreEqual(constHashy.ContainsKey(b), true);
			Assert::AreEqual(constHashy.ContainsKey(c), true);
			Assert::AreEqual(constHashy.ContainsKey(d), false);
		}

		TEST_METHOD(Remove)
		{
			const Foo a{ 1 };
			const Foo b{ 2 };
			const Foo c{ 3 };
			std::pair pairA = make_pair(a, 10);
			std::pair pairB = make_pair(b, 20);
			std::pair pairC = make_pair(c, 30);
			HashMap<Foo, int> hashMap{ pairA, pairB, pairC };

			Assert::AreEqual(hashMap.ContainsKey(a), true);
			Assert::AreEqual(hashMap.ContainsKey(b), true);
			Assert::AreEqual(hashMap.ContainsKey(c), true);
			
			hashMap.Remove(c);
			Assert::AreEqual(hashMap.ContainsKey(a), true);
			Assert::AreEqual(hashMap.ContainsKey(b), true);
			Assert::AreEqual(hashMap.ContainsKey(c), false);

			hashMap.Remove(b);
			Assert::AreEqual(hashMap.ContainsKey(a), true);
			Assert::AreEqual(hashMap.ContainsKey(b), false);
			Assert::AreEqual(hashMap.ContainsKey(c), false);

			hashMap.Remove(a);
			Assert::AreEqual(hashMap.ContainsKey(a), false);
			Assert::AreEqual(hashMap.ContainsKey(b), false);
			Assert::AreEqual(hashMap.ContainsKey(c), false);

			hashMap.Remove(a);
		}

		TEST_METHOD(Clear)
		{
			const Foo a{ 1 };
			const Foo b{ 2 };
			const Foo c{ 3 };
			std::pair pairA = make_pair(a, 10);
			std::pair pairB = make_pair(b, 20);
			std::pair pairC = make_pair(c, 30);
			HashMap<Foo, int> hashMap{ pairA, pairB, pairC };

			Assert::AreEqual(hashMap.Size(), size_t(3));
			Assert::AreEqual(hashMap.Capacity(), size_t(3));
			Assert::AreEqual(hashMap.ContainsKey(a), true);
			Assert::AreEqual(hashMap.ContainsKey(b), true);
			Assert::AreEqual(hashMap.ContainsKey(c), true);

			hashMap.Clear();
			Assert::AreEqual(hashMap.Size(), size_t(0));
			Assert::AreEqual(hashMap.Capacity(), size_t(3));
			Assert::AreEqual(hashMap.ContainsKey(a), false);
			Assert::AreEqual(hashMap.ContainsKey(b), false);
			Assert::AreEqual(hashMap.ContainsKey(c), false);

		}

		TEST_METHOD(IteratorIncrementFail)
		{
			Assert::ExpectException<std::runtime_error>([] {HashMap<Foo, int>::Iterator it; ++it; });
			Assert::ExpectException<std::runtime_error>([] {HashMap<Foo, int>::Iterator it; it++; });
		}

		TEST_METHOD(IteratorDereferenceFail)
		{
			Assert::ExpectException<std::runtime_error>([] {HashMap<Foo, int>::Iterator it; *it; });
			Assert::ExpectException<std::runtime_error>([] {HashMap<Foo, int>::Iterator it; it->first; });

			HashMap<Foo, int> constMap(10);
			HashMap<Foo, int>::Iterator it = constMap.end();
			Assert::ExpectException<std::runtime_error>([&it] {*it; });
			Assert::ExpectException<std::runtime_error>([&it] {it->second; });
		}

		TEST_METHOD(ConstIteratorIncrementFail)
		{
			Assert::ExpectException<std::runtime_error>([] {HashMap<Foo, int>::ConstIterator it; ++it; });
			Assert::ExpectException<std::runtime_error>([] {HashMap<Foo, int>::ConstIterator it; it++; });
		}

		TEST_METHOD(ConstIteratorDereferenceFail)
		{
			Assert::ExpectException<std::runtime_error>([] {HashMap<Foo, int>::ConstIterator it; *it; });
			Assert::ExpectException<std::runtime_error>([] {HashMap<Foo, int>::ConstIterator it; it->first; });

			const HashMap<Foo, int> constMap(10);
			HashMap<Foo, int>::ConstIterator it = constMap.end();
			Assert::ExpectException<std::runtime_error>([&it] {*it; });
			Assert::ExpectException<std::runtime_error>([&it] {it->second; });
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState HashMapTests::_startMemState;
}