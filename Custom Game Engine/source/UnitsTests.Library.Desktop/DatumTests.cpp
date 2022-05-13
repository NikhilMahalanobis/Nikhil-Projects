#include "pch.h"
#include "Datum.h"
#include "Foo.h"
#include <limits>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitsTestsLibraryDesktop
{
	TEST_CLASS(DatumTests)
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

		TEST_METHOD(DefaultConstructor)
		{
			Datum d;
			Assert::AreEqual(d.Type(), DatumTypes::Unknown);
			Assert::AreEqual(d.Size(), size_t(0));
			Assert::AreEqual(d.Capacity(), size_t(0));
		}
		TEST_METHOD(TypeCastConstructors)
		{
			{
				const int a = 1;
				Datum datum = a;
				Assert::AreEqual(datum.Size(), size_t(1));
				Assert::AreEqual(datum.Capacity(), size_t(3));
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
				Assert::AreEqual(datum.GetInt(), a);
			}

			{
				const float a = 1.0f;
				Datum datum = a;
				Assert::AreEqual(datum.Size(), size_t(1));
				Assert::AreEqual(datum.Capacity(), size_t(3));
				Assert::AreEqual(DatumTypes::Float, datum.Type());
				Assert::AreEqual(datum.GetFloat(), a);
			}

			{
				const glm::vec4 a(1);
				Datum datum = a;
				Assert::AreEqual(datum.Size(), size_t(1));
				Assert::AreEqual(datum.Capacity(), size_t(3));
				Assert::AreEqual(DatumTypes::Vector, datum.Type());
				Assert::AreEqual(datum.GetVector(), a);
			}

			{
				const glm::mat4 a(1);
				Datum datum = a;
				Assert::AreEqual(datum.Size(), size_t(1));
				Assert::AreEqual(datum.Capacity(), size_t(3));
				Assert::AreEqual(DatumTypes::Matrix, datum.Type());
				Assert::AreEqual(datum.GetMatrix(), a);
			}

			{
				const std::string a = "Hello";
				Datum datum = a;
				Assert::AreEqual(datum.Size(), size_t(1));
				Assert::AreEqual(datum.Capacity(), size_t(3));
				Assert::AreEqual(DatumTypes::String, datum.Type());
				Assert::AreEqual(datum.GetString(), a);
			}

			{
				Foo a{ 1 };
				RTTI* r = &a;
				Datum datum = r;
				Assert::AreEqual(datum.Size(), size_t(1));
				Assert::AreEqual(datum.Capacity(), size_t(3));
				Assert::AreEqual(DatumTypes::Pointer, datum.Type());
				Assert::AreEqual(datum.GetPointer(), r);
			}
		}

		TEST_METHOD(CopyConstructor)
		{
			Datum d1;
			d1.PushBack(10);
			d1.PushBack(10);
			Assert::AreEqual(d1.Size(), (size_t)2);
			Assert::AreEqual(d1.Capacity(), (size_t)3);
			Datum d2 = d1;
			Assert::AreEqual(d2.Size(), (size_t)2);
			Assert::AreEqual(d2.Capacity(), (size_t)3);
			Datum d3;
			d3.SetType(DatumTypes::String);
			Assert::AreEqual(d3.Size(), (size_t)0);
			Assert::AreEqual(d3.Capacity(), (size_t)0);
			std::string s1 = "";
			d3.PushBack(s1);
			Datum d4 = d3;
			Assert::AreEqual(d4.Size(), (size_t)1);
			Assert::AreEqual(d4.Capacity(), (size_t)1);
			int a[] = { 1,2 };
			Datum d8;
			d8.SetStorage(a, 2);
			d8.Set(10);
			Datum d7 = d8;
			Assert::AreEqual(d7.Size(), (size_t)2);
			Assert::AreEqual(d7.Capacity(), (size_t)2);
			Datum d9;
			d9.SetType(DatumTypes::Float);
			Assert::ExpectException<std::runtime_error>([&d9, &a] {d9.SetStorage(a, 10); });
		}

		TEST_METHOD(CopyAssignmentOperator)
		{
			{
				Datum d1;
				d1.PushBack(10);
				d1.PushBack(10);
				Assert::AreEqual(d1.Size(), (size_t)2);
				Assert::AreEqual(d1.Capacity(), (size_t)3);
				Datum d2;
				d2 = d1;
				Assert::AreEqual(d2.Size(), (size_t)2);
				Assert::AreEqual(d2.Capacity(), (size_t)3);
				Datum d3;
				d3.SetType(DatumTypes::String);
				Assert::AreEqual(d3.Size(), (size_t)0);
				Assert::AreEqual(d3.Capacity(), (size_t)0);
				std::string s1 = "";
				d3.PushBack(s1);
				Datum d4;
				d4 = d3;
				Assert::AreEqual(d4.Size(), (size_t)1);
				Assert::AreEqual(d4.Capacity(), (size_t)1);
				float a[] = { 1,2 };
				Datum d8;
				d8.SetStorage(a, 2);
				d8.Set(10.f);
				Datum d7;
				d7 = d8;
				Assert::AreEqual(d7.Size(), (size_t)2);
				Assert::AreEqual(d7.Capacity(), (size_t)2);
				Datum d9;
				d9.SetType(DatumTypes::Integer);
				Assert::ExpectException<std::runtime_error>([&d9, &a] {d9.SetStorage(a, 10); });
			}

			{
				Datum d1;
				d1 = 5;
				Assert::AreEqual(d1.Size(), (size_t)1);
				Assert::AreEqual(d1.Capacity(), (size_t)1);
				Assert::AreEqual(d1.Type(), DatumTypes::Integer);
				Datum d2;
				int a[] = { 10,20,30 };
				d2.SetStorage(a, 3);
				Datum d3;
				d3.PushBack(10);
				Assert::AreEqual(10, d3.GetInt());
				d3 = 5;
				Assert::AreEqual(5, d3.GetInt());

			}

			{
				Datum d1;
				d1 = 5.f;
				Assert::AreEqual(d1.Size(), (size_t)1);
				Assert::AreEqual(d1.Capacity(), (size_t)1);
				Assert::AreEqual(d1.Type(), DatumTypes::Float);
				Datum d2;
				float a[] = { 10,20,30 };
				d2.SetStorage(a, 3);
				Datum d3;
				d3.PushBack(10.f);
				Assert::AreEqual(10.f, d3.GetFloat());
				d3 = 5.f;
				Assert::AreEqual(5.f, d3.GetFloat());
			}

			{
				Datum d1;
				d1 = "Abc";
				Assert::AreEqual(d1.Size(), (size_t)1);
				Assert::AreEqual(d1.Capacity(), (size_t)1);
				Assert::AreEqual(d1.Type(), DatumTypes::String);
				Datum d2;
				std::string a[] = { "","","" };
				d2.SetStorage(a, 3);
				Datum d3;
				d3.PushBack(glm::vec4(1.0f));
				Assert::AreEqual(glm::vec4(1.0f), d3.GetVector());
				d3 = glm::vec4(5.0f);
				Assert::AreEqual(glm::vec4(5.0f), d3.GetVector());
			}

			{
				Datum d1;
				d1 = glm::vec4();
				Assert::AreEqual(d1.Size(), (size_t)1);
				Assert::AreEqual(d1.Capacity(), (size_t)1);
				Assert::AreEqual(d1.Type(), DatumTypes::Vector);
				Datum d2;
				glm::vec4 a[] = { {1.f,2.f,3.f,4.f }, {1.f, 2.f, 3.f, 4.f} };
				d2.SetStorage(a, 2);
				Datum d3;
				d3.PushBack(glm::mat4(1.0f));
				Assert::AreEqual(glm::mat4(1.0f), d3.GetMatrix());
				d3 = glm::mat4(5.0f);
				Assert::AreEqual(glm::mat4(5.0f), d3.GetMatrix());
			}

			{
				Datum d1;
				d1 = glm::mat4();
				Assert::AreEqual(d1.Size(), (size_t)1);
				Assert::AreEqual(d1.Capacity(), (size_t)1);
				Assert::AreEqual(d1.Type(), DatumTypes::Matrix);
				Datum d2;
				glm::mat4 a[1];
				d2.SetStorage(a, 1);
				Datum d3;
				d3.PushBack("Hello");
				Assert::AreEqual("Hello"s, d3.GetString());
				d3 = "Goodbye"s;
				Assert::AreEqual("Goodbye"s, d3.GetString());
			}
			{
				Datum d1;
				Assert::AreEqual(d1.Size(), (size_t)0);
				Assert::AreEqual(d1.Capacity(), (size_t)0);
				Foo f1(10);
				RTTI* r = &f1;
				d1 = r;
				Assert::AreEqual(d1.Size(), (size_t)1);
				Assert::AreEqual(d1.Capacity(), (size_t)1);
				Assert::AreEqual(d1.Type(), DatumTypes::Pointer);
				Datum d2;
				RTTI* a = &f1;
				d2.SetStorage(&a, 10);
				Foo f2(20);
				r = &f2;
				Assert::AreEqual(a, d2.GetPointer());
				d2 = r;
				Assert::AreEqual(r, d2.GetPointer());

			}
			{
				Datum d1;
				std::int32_t a[3] = { 0,1,2 };
				d1.SetStorage(a, 3);
				Datum d2;
				d2.PushBack(10);
				d2.PushBack(20);
				d1 = d2;
				Assert::AreEqual(d1.Size(), (size_t)2);
				Assert::AreEqual(d1.Capacity(), (size_t)3);
				Assert::AreEqual(d1.IntFront(), 10);
				Assert::AreEqual(d1.IntBack(), 20);
			}
			{
				Datum d1;
				std::int32_t a[3] = { 0,1,2 };
				d1.SetStorage(a, 3);
				Datum d2;
				std::string s1 = "HYE";
				std::string s2 = "BYE";
				d2.PushBack(s1);
				d2.PushBack(s2);
				d1 = d2;
				Assert::AreEqual(d1.Size(), (size_t)2);
				Assert::AreEqual(d1.Capacity(), (size_t)3);
				Assert::AreEqual(d1.StringFront(), s1);
				Assert::AreEqual(d1.StringBack(), s2);
			}
			{
				Datum d1;
				std::int32_t a[3] = { 0,1,2 };
				d1.SetStorage(a, 3);

				Datum d2;
				std::int32_t b[4] = { 4,5,6,7 };
				d2.SetStorage(b, 4);

				d1 = d2;
				Assert::AreEqual(d1.Size(), (size_t)4);
				Assert::AreEqual(d1.Capacity(), (size_t)4);
				Assert::AreEqual(d1.IntFront(), b[0]);
				Assert::AreEqual(d1.IntBack(), b[3]);
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			Datum d1;
			d1.PushBack(10);
			d1.PushBack(20);
			Assert::AreEqual(d1.Size(), (size_t)2);
			Datum d2 = std::move(d1);
			Assert::AreEqual(d2.Size(), (size_t)2);
			Assert::AreEqual(d2.IntFront(), 10);
			Datum d3;
			d3 = std::move(d2);
			Assert::AreEqual(d3.IntFront(), 10);
			Assert::AreEqual(d3.Size(), (size_t)2);
			Assert::AreEqual(d3.Type(), DatumTypes::Integer);
		}
		TEST_METHOD(Front)
		{
			{
				Datum d1;
				d1.PushBack(10);
				Assert::ExpectException<std::runtime_error>([&d1] {d1.FloatFront(); });
				const Datum d2 = d1;
				Assert::AreEqual(d2.IntFront(), 10);
				Assert::AreEqual(d2.IntBack(), 10);
				Assert::AreEqual(d1.IntBack(), 10);
				Assert::ExpectException<std::runtime_error>([&d2] {d2.FloatFront(); });
			}

			{
				Datum d1;
				d1.PushBack(10.f);
				Assert::ExpectException<std::runtime_error>([&d1] {d1.IntFront(); });
				const Datum d2 = d1;
				Assert::AreEqual(d2.FloatFront(), 10.f);
				Assert::AreEqual(d2.FloatBack(), 10.f);
				Assert::AreEqual(d1.FloatBack(), 10.f);
				Assert::ExpectException<std::runtime_error>([&d2] {d2.IntFront(); });
			}

			{
				Datum d1;
				glm::vec4 v1(1.f);
				d1.PushBack(v1);
				Assert::ExpectException<std::runtime_error>([&d1] {d1.MatrixFront(); });
				const Datum d2 = d1;
				Assert::AreEqual(d2.VectorFront(), v1);
				Assert::AreEqual(d2.VectorBack(), v1);
				Assert::AreEqual(d1.VectorBack(), v1);
				Assert::ExpectException<std::runtime_error>([&d2] {d2.MatrixFront(); });
			}

			{
				Datum d1;
				glm::mat4 m1(1.f);
				d1.PushBack(m1);
				Assert::ExpectException<std::runtime_error>([&d1] {d1.VectorFront(); });
				const Datum d2 = d1;
				Assert::AreEqual(d2.MatrixFront(), m1);
				Assert::AreEqual(d2.MatrixBack(), m1);
				Assert::AreEqual(d1.MatrixBack(), m1);
				Assert::ExpectException<std::runtime_error>([&d2] {d2.VectorFront(); });
			}

			{
				Datum d1;
				Foo f1(10);
				RTTI* r;
				r = &f1;
				d1.PushBack(r);
				Assert::ExpectException<std::runtime_error>([&d1] {d1.StringFront(); });
				const Datum d2 = d1;
				Assert::AreEqual(d2.PointerFront(), r);
				Assert::AreEqual(d2.PointerBack(), r);
				Assert::AreEqual(d1.PointerFront(), r);
				Assert::AreEqual(d1.PointerBack(), r);
				Assert::ExpectException<std::runtime_error>([&d2] {d2.StringFront(); });
				
				Foo f2{ 20 };
				RTTI* g = &f2;
				d1.Set(g);
			}
			{
				Datum d1;
				std::string a = "abc";
				d1.PushBack(a);
				const Datum d2 = d1;
				Assert::AreEqual(d2.StringFront(), a);
				Assert::AreEqual(d2.StringBack(), a);
				Assert::AreEqual(d1.StringBack(), a);
				Assert::ExpectException<std::runtime_error>([&d1] {d1.PointerFront(); });
				
			}
		}

		TEST_METHOD(EqualityOperators)
		{
			{
				Datum d, d1, d2, d3, d4, d5;
				d.SetType(DatumTypes::Integer);
				d1.SetType(DatumTypes::Integer);
				d.PushBack(10);
				d.PushBack(20);
				d.PushBack(30);
				d1.PushBack(10);
				d1.PushBack(20);
				d1.PushBack(30);
				Assert::IsTrue(d == d1);
				Assert::IsTrue(d != d2);
				Datum s1, s2, s3;
				s1.SetType(DatumTypes::String);
				s2.SetType(DatumTypes::String);
				s1.PushBack("abc");
				s2.PushBack("abc");
				s3.PushBack("asdasdasd");
				Assert::IsTrue(s1 == s2);
				Assert::IsFalse(s2 == s3);
				Foo f1(10);
				Foo f2(10);
				RTTI* r1;
				RTTI* r2;
				RTTI* r3;
				r1 = &f1;
				r2 = &f1;
				r3 = &f2;
				d3.PushBack(r1);
				d4.PushBack(r2);
				d5.PushBack(r3);
				Assert::IsTrue(d3 == d4);
				Assert::IsFalse(d3 == d5);
			}

			{
				Datum d, d1, d2;
				d.SetType(DatumTypes::Integer);
				d.PushBack(5);
				Assert::IsTrue(d == 5);
				Assert::IsFalse(d == 6);
				d.PushBack(6);
				Assert::IsFalse(d == 6);
				Assert::IsTrue(d != 8);
			}
			{
				Datum d, d1, d2;
				d.SetType(DatumTypes::Float);
				d.PushBack(5.f);
				Assert::IsTrue(d == 5.f);
				Assert::IsFalse(d == 6.f);
				d.PushBack(6.f);
				Assert::IsFalse(d == 6.f);
				Assert::IsTrue(d != 8.f);
			}
			{
				Datum d, d1, d2;
				d.SetType(DatumTypes::Vector);
				glm::vec4 v(1.f);
				glm::vec4 v1(2.f);
				d.PushBack(v);
				Assert::IsTrue(d == v);
				Assert::IsFalse(d == v1);
				d.PushBack(v1);
				Assert::IsFalse(d == v1);
				Assert::IsTrue(d != v1);
			}
			{
				Datum d, d1, d2;
				d.SetType(DatumTypes::Matrix);
				glm::mat4 m(1.f);
				glm::mat4 m1(2.f);
				d.PushBack(m);
				Assert::IsTrue(d == m);
				Assert::IsFalse(d == m1);
				d.PushBack(m1);
				Assert::IsFalse(d == m1);
				Assert::IsTrue(d != m1);
			}
			{
				Datum d, d1, d2;
				d.SetType(DatumTypes::String);
				std::string a = "abc";
				std::string b = "abc123";
				d.PushBack(a);
				Assert::IsTrue(d == a);
				Assert::IsFalse(d == b);
				d.PushBack(b);
				Assert::IsFalse(d == b);
				Assert::IsTrue(d != b);
			}
			{
				Datum d, d1, d2;
				d.SetType(DatumTypes::Pointer);
				Foo f1(10);
				Foo f2(20);
				RTTI* r1 = &f1;
				RTTI* r2 = &f2;
				d.PushBack(r1);
				Assert::IsTrue(d == r1);
				Assert::IsFalse(d == r2);
				d.PushBack(r2);
				Assert::IsFalse(d == r2);
				Assert::IsTrue(d != r2);
			}
		}

		TEST_METHOD(Resize)
		{
			Datum d;
			Assert::AreEqual(d.Type(), DatumTypes::Unknown);
			Assert::AreEqual(d.Size(), (size_t)0);
			Assert::AreEqual(d.Capacity(), (size_t)0);
			d.SetType(DatumTypes::Integer);
			Assert::AreEqual(d.Type(), DatumTypes::Integer);
			d.Reserve(10);
			Assert::AreEqual(d.Size(), (size_t)0);
			Assert::AreEqual(d.Capacity(), (size_t)10);
			d.Resize(5);
			Assert::AreEqual(d.Size(), (size_t)5);
			Assert::AreEqual(d.Capacity(), (size_t)5);
			d.Resize(15);
			Assert::AreEqual(d.Size(), (size_t)15);
			Assert::AreEqual(d.Capacity(), (size_t)15);
			d.PushBack(10);
			d.PushBack(10);
			d.PushBack(10);
			d.PushBack(10);
			d.PushBack(10);
			d.Resize(2);
			Assert::AreEqual(d.Size(), (size_t)2);
			Assert::AreEqual(d.Capacity(), (size_t)2);
			Datum d1;
			Assert::AreEqual(d1.Type(), DatumTypes::Unknown);
			Assert::AreEqual(d1.Size(), (size_t)0);
			Assert::AreEqual(d1.Capacity(), (size_t)0);
			d1.SetType(DatumTypes::String);
			d1.Resize(5);
			Assert::AreEqual(d1.Size(), (size_t)5);
			Assert::AreEqual(d1.Capacity(), (size_t)5);
			Datum d2;
			d2.PushBack("Hello");
			d2.PushBack("Hello");
			d2.PushBack("Hello");
			d2.PushBack("Hello");
			d2.PushBack("Hello");
			d2.PushBack("Hello");
			d2.PushBack("Hello");
			d2.Resize(3);
			Assert::AreEqual(d2.Size(), (size_t)3);
			Assert::AreEqual(d2.Capacity(), (size_t)3);
		}

		TEST_METHOD(PushBack)
		{
			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				d.PushBack(10);
				Assert::AreEqual(d.Type(), DatumTypes::Integer);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)1);
				d.PushBack(20);
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::ExpectException<std::runtime_error>([&d] {d.PushBack(10.f); });
			}

			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				d.PushBack(10.f);
				Assert::AreEqual(d.Type(), DatumTypes::Float);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)1);
				d.PushBack(20.f);
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::ExpectException<std::runtime_error>([&d] {d.PushBack(10); });
			}

			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				glm::vec4 v1;
				d.PushBack(v1);
				Assert::AreEqual(d.Type(), DatumTypes::Vector);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)1);
				glm::vec4 v2;
				d.PushBack(v2);
				Assert::AreEqual(d.Size(), (size_t)2);
				glm::mat4 m;
				Assert::ExpectException<std::runtime_error>([&d, &m] {d.PushBack(m); });
			}

			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				glm::mat4 m1;
				d.PushBack(m1);
				Assert::AreEqual(d.Type(), DatumTypes::Matrix);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)1);
				glm::mat4 m2;
				d.PushBack(m2);
				Assert::AreEqual(d.Size(), (size_t)2);
				glm::vec4 v;
				Assert::ExpectException<std::runtime_error>([&d, &v] {d.PushBack(v); });
			}

			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				std::string s1 = "dashk";
				d.PushBack(s1);
				Assert::AreEqual(d.Type(), DatumTypes::String);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)1);
				std::string s2 = "dashk";
				d.PushBack(s2);
				Assert::AreEqual(d.Size(), (size_t)2);
				Foo f2(10);
				RTTI* r = &f2;
				Assert::ExpectException<std::runtime_error>([&d, &r] {d.PushBack(r); });
			}

			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				Foo f1(10);
				RTTI* r;
				r = &f1;
				d.PushBack(r);
				Assert::AreEqual(d.Type(), DatumTypes::Pointer);
				Assert::AreEqual(d.Size(), (size_t)1);
				Assert::AreEqual(d.Capacity(), (size_t)1);
				std::string s = "";
				Assert::ExpectException<std::runtime_error>([&d, &s] {d.PushBack(s); });
			}
		}

		TEST_METHOD(Set)
		{
			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				Assert::ExpectException<std::runtime_error>([&d] {d.Set(10, 20); });
				Assert::ExpectException<std::runtime_error>([&d] {d.Set(10); });
			}
			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				Assert::ExpectException<std::runtime_error>([&d] {d.Set(10.f, 20); });
				Assert::ExpectException<std::runtime_error>([&d] {d.Set(10.f); });
			}
			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				std::string a = "";
				Assert::ExpectException<std::runtime_error>([&d, &a] {d.Set(a, 20); });
				Assert::ExpectException<std::runtime_error>([&d, &a] {d.Set(a); });
			}
			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				glm::vec4 v;
				Assert::ExpectException<std::runtime_error>([&d, &v] {d.Set(v, 20); });
				Assert::ExpectException<std::runtime_error>([&d, &v] {d.Set(v); });
			}
			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				glm::mat4 m;
				Assert::ExpectException<std::runtime_error>([&d, &m] {d.Set(m, 20); });
				Assert::ExpectException<std::runtime_error>([&d, &m] {d.Set(m); });
			}
			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				RTTI* r;
				Foo f1(10);
				r = &f1;
				Assert::ExpectException<std::runtime_error>([&d, &r] {d.Set(r, 20); });
				Assert::ExpectException<std::runtime_error>([&d, &r] {d.Set(r); });
			}
		}

		TEST_METHOD(Get)
		{
			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				d.PushBack(10);
				d.PushBack(20);
				Assert::AreEqual(d.Type(), DatumTypes::Integer);
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetInt(0), 10);
				Assert::AreEqual(d.GetInt(1), 20);
				Assert::ExpectException<std::runtime_error>([&d] {d.GetInt(20); });
				Assert::ExpectException<std::runtime_error>([&d] {d.GetFloat(0); });
				const Datum d1 = d;
				Assert::AreEqual(d1.Type(), DatumTypes::Integer);
				Assert::AreEqual(d1.Size(), (size_t)2);
				Assert::AreEqual(d1.Capacity(), (size_t)3);
				Assert::AreEqual(d1.GetInt(0), 10);
				Assert::AreEqual(d1.GetInt(1), 20);
				Assert::ExpectException<std::runtime_error>([&d1] {d1.GetInt(20); });
				Assert::ExpectException<std::runtime_error>([&d1] {d1.GetFloat(0); });

			}
			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				d.PushBack(10.f);
				d.PushBack(20.f);
				Assert::AreEqual(d.Type(), DatumTypes::Float);
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetFloat(0), 10.f);
				Assert::AreEqual(d.GetFloat(1), 20.f);
				Assert::ExpectException<std::runtime_error>([&d] {d.GetFloat(20); });
				Assert::ExpectException<std::runtime_error>([&d] {d.GetInt(0); });
				const Datum d1 = d;
				Assert::AreEqual(d1.Type(), DatumTypes::Float);
				Assert::AreEqual(d1.Size(), (size_t)2);
				Assert::AreEqual(d1.Capacity(), (size_t)3);
				Assert::AreEqual(d1.GetFloat(0), 10.f);
				Assert::AreEqual(d1.GetFloat(1), 20.f);
				Assert::ExpectException<std::runtime_error>([&d1] {d1.GetFloat(20); });
				Assert::ExpectException<std::runtime_error>([&d1] {d1.GetInt(0); });
			}
			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				glm::vec4 v;
				glm::vec4 v1;
				d.PushBack(v);
				d.PushBack(v1);
				Assert::AreEqual(d.Type(), DatumTypes::Vector);
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetVector(0), v);
				Assert::AreEqual(d.GetVector(1), v1);
				Assert::ExpectException<std::runtime_error>([&d] {d.GetVector(20); });
				Assert::ExpectException<std::runtime_error>([&d] { d.GetMatrix(0); });
				const Datum d1 = d;
				Assert::AreEqual(d1.Type(), DatumTypes::Vector);
				Assert::AreEqual(d1.Size(), (size_t)2);
				Assert::AreEqual(d1.Capacity(), (size_t)3);
				Assert::AreEqual(d1.GetVector(0), v);
				Assert::AreEqual(d1.GetVector(1), v1);
				Assert::ExpectException<std::runtime_error>([&d1] {d1.GetVector(20); });
				Assert::ExpectException<std::runtime_error>([&d1] {d1.GetMatrix(0); });
			}
			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				glm::mat4 m;
				glm::mat4 m1;
				d.PushBack(m);
				d.PushBack(m1);
				Assert::AreEqual(d.Type(), DatumTypes::Matrix);
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Assert::AreEqual(d.GetMatrix(0), m);
				Assert::AreEqual(d.GetMatrix(1), m1);
				Assert::ExpectException<std::runtime_error>([&d] {d.GetMatrix(20); });
				Assert::ExpectException<std::runtime_error>([&d] { d.GetVector(0); });
				const Datum d1 = d;
				Assert::AreEqual(d1.Type(), DatumTypes::Matrix);
				Assert::AreEqual(d1.Size(), (size_t)2);
				Assert::AreEqual(d1.Capacity(), (size_t)3);
				Assert::AreEqual(d1.GetMatrix(0), m);
				Assert::AreEqual(d1.GetMatrix(1), m1);
				Assert::ExpectException<std::runtime_error>([&d1] {d1.GetMatrix(20); });
				Assert::ExpectException<std::runtime_error>([&d1] {d1.GetVector(0); });
			}
			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				std::string a;
				std::string a1 = "abc";
				d.PushBack(a);
				d.PushBack(a1);
				Assert::AreEqual(d.Type(), DatumTypes::String);
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				Foo f1(20);
				RTTI* r = &f1;
				Assert::AreEqual(d.GetString(0), a);
				Assert::AreEqual(d.GetString(1), a1);
				Assert::ExpectException<std::runtime_error>([&d] {d.GetString(20); });
				Assert::ExpectException<std::runtime_error>([&d, &r] { d.GetPointer(0); });
				const Datum d1 = d;
				Assert::AreEqual(d1.Type(), DatumTypes::String);
				Assert::AreEqual(d1.Size(), (size_t)2);
				Assert::AreEqual(d1.Capacity(), (size_t)3);
				Assert::AreEqual(d1.GetString(0), a);
				Assert::AreEqual(d1.GetString(1), a1);
				Assert::ExpectException<std::runtime_error>([&d1] {d1.GetString(20); });
				Assert::ExpectException<std::runtime_error>([&d1, &r] { d1.GetPointer(0); });
			}
			{
				Datum d;
				Assert::AreEqual(d.Type(), DatumTypes::Unknown);
				Assert::AreEqual(d.Size(), (size_t)0);
				Assert::AreEqual(d.Capacity(), (size_t)0);
				Foo f1(20);
				RTTI* r = &f1;
				d.PushBack(r);
				d.PushBack(r);
				Assert::AreEqual(d.Type(), DatumTypes::Pointer);
				Assert::AreEqual(d.Size(), (size_t)2);
				Assert::AreEqual(d.Capacity(), (size_t)3);
				std::string a = "";
				Assert::AreEqual(d.GetPointer(0), r);
				Assert::ExpectException<std::runtime_error>([&d] {d.GetPointer(20); });
				Assert::ExpectException<std::runtime_error>([&d, &a] { d.GetString(0); });
				const Datum d1 = d;
				Assert::AreEqual(d1.Type(), DatumTypes::Pointer);
				Assert::AreEqual(d1.Size(), (size_t)2);
				Assert::AreEqual(d1.Capacity(), (size_t)3);
				Assert::AreEqual(d1.GetPointer(0), r);
				Assert::ExpectException<std::runtime_error>([&d1] {d1.GetPointer(20); });
				Assert::ExpectException<std::runtime_error>([&d1, &a] { d1.GetString(0); });
			}
		}

		TEST_METHOD(ToString)
		{
			{
				Datum d;
				d.SetType(DatumTypes::Integer);
				d.PushBack(10);
				std::string a = "10";
				Assert::AreEqual(a, d.ToString(0));
			}
			{
				Datum d;
				d.SetType(DatumTypes::Float);
				d.PushBack(10.f);
				std::string a = "10.000000";
				Assert::AreEqual(a, d.ToString(0));
			}

			{
				Datum d;
				d.SetType(DatumTypes::String);
				std::string a = "10.000000";
				d.PushBack(a);
				Assert::AreEqual(a, d.ToString(0));
			}

			{
				Datum d;
				d.SetType(DatumTypes::Vector);
				glm::vec4 v(1.f);
				d.PushBack(v);
				std::string a = "vec4(1.000000, 1.000000, 1.000000, 1.000000)";
				Assert::AreEqual(a, d.ToString(0));
			}

			{
				Datum d;
				d.SetType(DatumTypes::Matrix);
				glm::mat4 m(1.f);
				d.PushBack(m);
				std::string a = "mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))";
				Assert::AreEqual(a, d.ToString(0));
			}

			{
				Datum d;
				d.SetType(DatumTypes::Pointer);
				Foo f{ 10 };
				RTTI* r = &f;
				d.PushBack(r);
				std::string a = "10";
				Assert::AreEqual(a, d.ToString(0));
			}

			{
				Datum d;
				Assert::ExpectException<std::runtime_error>([&d] {d.ToString(); });
			}
		}

		TEST_METHOD(SetFromString)
		{
			{
				Datum d;
				d.SetType(DatumTypes::Integer);
				d.PushBack(10);
				d.SetFromString("5", 0);
				Assert::AreEqual(d.IntFront(), 5);
			}
			{
				Datum d;
				d.SetType(DatumTypes::Float);
				d.PushBack(10.f);
				d.SetFromString("5.f", 0);
				Assert::AreEqual(d.FloatFront(), 5.f);
			}
			{
				Datum d;
				d.SetType(DatumTypes::Vector);
				glm::vec4 init(20.f);
				d.PushBack(init);
				glm::vec4 v(1.0f);
				d.SetFromString("1.0 1.0 1.0 1.0", 0);
				Assert::AreEqual(d.VectorFront(), v);
			}
			{
				Datum d;
				d.SetType(DatumTypes::Matrix);
				glm::mat4 init(20.f);
				d.PushBack(init);
				glm::mat4 m(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
				d.SetFromString("1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0", 0);
				Assert::AreEqual(d.MatrixFront(), m);
			}

			{
				Datum d;
				d.SetType(DatumTypes::String);
				d.PushBack("xyz");
				std::string a = "abc";
				d.SetFromString(a, 0);
				Assert::AreEqual(d.StringFront(), a);
			}
			{
				Datum d;
				std::string a = ";";
				Assert::ExpectException<std::runtime_error>([&d, &a] {d.SetFromString(a); });
			}
		}

		TEST_METHOD(SetStorage)
		{
			Datum d;
			d.SetType(DatumTypes::Integer);
			d.Reserve(10);
			Assert::AreEqual(d.Size(), (std::size_t)0);
			Assert::AreEqual(d.Capacity(), (std::size_t)10);
			std::int32_t a[3] = { 0,1,2 };
			d.SetStorage(a, 3);
			Assert::AreEqual(d.Size(), (std::size_t)3);
			Assert::AreEqual(d.Capacity(), (std::size_t)3);
			Assert::AreEqual(d.IntFront(), 0);
			std::string s[3];
			Assert::ExpectException<std::runtime_error>([&d, &s] {d.SetStorage(s, 3); });
			
			{
				Datum d1;
				d1.SetType(DatumTypes::String);
				std::string c = "";
				d1.PushBack(c);
				d1.SetStorage(s, 3);
				Assert::AreEqual(d1.Size(), (std::size_t)3);
				Assert::AreEqual(d1.Capacity(), (std::size_t)3);
			}

			{
				Datum d1;
				d1.SetType(DatumTypes::Vector);
				glm::vec4 v(1.0f);
				glm::vec4 v1[3];
				d1.PushBack(v);
				d1.SetStorage(v1, 3);
				Assert::AreEqual(d1.Size(), (std::size_t)3);
				Assert::AreEqual(d1.Capacity(), (std::size_t)3);
			}
			{
				Datum d1;
				d1.SetType(DatumTypes::Matrix);
				glm::mat4 v(1.0f);
				glm::mat4 v1[3];
				d1.PushBack(v);
				d1.SetStorage(v1, 3);
				Assert::AreEqual(d1.Size(), (std::size_t)3);
				Assert::AreEqual(d1.Capacity(), (std::size_t)3);
			}
			{
				Datum d1;
				d1.SetType(DatumTypes::Pointer);
				Foo f1{ 10 };
				RTTI* r1[3];
				RTTI* r = &f1;
				d1.PushBack(r);
				d1.SetStorage(r1, 3);
				Assert::AreEqual(d1.Size(), (std::size_t)3);
				Assert::AreEqual(d1.Capacity(), (std::size_t)3);
			}
		}

		TEST_METHOD(Remove)
		{
			{
				Datum d1;
				d1.SetType(DatumTypes::Integer);
				d1.Reserve(10);
				Assert::AreEqual(d1.Size(), (std::size_t)0);
				Assert::AreEqual(d1.Capacity(), (std::size_t)10);
				d1.PushBack(10);
				d1.PushBack(20);
				Assert::AreEqual(d1.Size(), (std::size_t)2);
				Assert::AreEqual(d1.Remove(20), true);
				Assert::AreEqual(d1.Size(), (std::size_t)1);
				Assert::AreEqual(d1.Remove(30), false);
				Assert::AreEqual(d1.Size(), (std::size_t)1);
			}


			{
				Datum d1;
				d1.SetType(DatumTypes::Float);
				d1.Reserve(10);
				Assert::AreEqual(d1.Size(), (std::size_t)0);
				Assert::AreEqual(d1.Capacity(), (std::size_t)10);
				d1.PushBack(10.f);
				d1.PushBack(20.f);
				Assert::AreEqual(d1.Size(), (std::size_t)2);
				Assert::AreEqual(d1.Remove(20.f), true);
				Assert::AreEqual(d1.Size(), (std::size_t)1);
				Assert::AreEqual(d1.Remove(30.f), false);
				Assert::AreEqual(d1.Size(), (std::size_t)1);
			}

			{
				Datum d1;
				d1.SetType(DatumTypes::String);
				d1.Reserve(10);
				Assert::AreEqual(d1.Size(), (std::size_t)0);
				Assert::AreEqual(d1.Capacity(), (std::size_t)10);
				std::string a = "abc";
				d1.PushBack(a);
				d1.PushBack("abc1");
				Assert::AreEqual(d1.Size(), (std::size_t)2);
				Assert::AreEqual(d1.Remove("abc1"), true);
				Assert::AreEqual(d1.Size(), (std::size_t)1);
				Assert::AreEqual(d1.Remove("dasdsa"), false);
				Assert::AreEqual(d1.Size(), (std::size_t)1);
			}

			{
				Datum d1;
				d1.SetType(DatumTypes::Matrix);
				d1.Reserve(10);
				Assert::AreEqual(d1.Size(), (std::size_t)0);
				Assert::AreEqual(d1.Capacity(), (std::size_t)10);
				glm::mat4 a(1);
				glm::mat4 b(2);
				glm::mat4 b1(22);
				d1.PushBack(a);
				d1.PushBack(b);
				Assert::AreEqual(d1.Size(), (std::size_t)2);
				Assert::AreEqual(d1.Remove(a), true);
				Assert::AreEqual(d1.Size(), (std::size_t)1);
				Assert::AreEqual(d1.Remove(b1), false);
				Assert::AreEqual(d1.Size(), (std::size_t)1);
			}
			{
				Datum d1;
				d1.SetType(DatumTypes::Vector);
				d1.Reserve(10);
				Assert::AreEqual(d1.Size(), (std::size_t)0);
				Assert::AreEqual(d1.Capacity(), (std::size_t)10);
				glm::vec4 a(1);
				glm::vec4 b(2);
				glm::vec4 b1(22);
				d1.PushBack(a);
				d1.PushBack(b);
				Assert::AreEqual(d1.Size(), (std::size_t)2);
				Assert::AreEqual(d1.Remove(a), true);
				Assert::AreEqual(d1.Size(), (std::size_t)1);
				Assert::AreEqual(d1.Remove(b1), false);
				Assert::AreEqual(d1.Size(), (std::size_t)1);
			}

			{
				Datum d1;
				d1.SetType(DatumTypes::Pointer);
				d1.Reserve(10);
				Assert::AreEqual(d1.Size(), (std::size_t)0);
				Assert::AreEqual(d1.Capacity(), (std::size_t)10);
				Foo f1(10);
				Foo f2(100);
				Foo f3(1000);
				RTTI* r = &f1;
				RTTI* r1 = &f2;
				RTTI* r2 = &f3;
				d1.PushBack(r);
				d1.PushBack(r1);
				Assert::AreEqual(d1.Size(), (std::size_t)2);
				Assert::AreEqual(d1.Remove(r), true);
				Assert::AreEqual(d1.Size(), (std::size_t)1);
				Assert::AreEqual(d1.Remove(r2), false);
				Assert::AreEqual(d1.Size(), (std::size_t)1);
				Assert::ExpectException<std::runtime_error>([&d1] {d1.RemoveAt(3); });
			}
		}

		TEST_METHOD(Find)
		{
			{
				Datum d1;
				d1.SetType(DatumTypes::Integer);
				d1.Reserve(10);
				Assert::AreEqual(d1.Size(), (std::size_t)0);
				Assert::AreEqual(d1.Capacity(), (std::size_t)10);
				d1.PushBack(10);
				d1.PushBack(20);
				Assert::AreEqual(d1.Size(), (std::size_t)2);
				Assert::AreEqual(d1.Find(20).second, true);
				Assert::AreEqual(d1.Find(30).second, false);
			}


			{
				Datum d1;
				d1.SetType(DatumTypes::Float);
				d1.Reserve(10);
				Assert::AreEqual(d1.Size(), (std::size_t)0);
				Assert::AreEqual(d1.Capacity(), (std::size_t)10);
				d1.PushBack(10.f);
				d1.PushBack(20.f);
				Assert::AreEqual(d1.Size(), (std::size_t)2);
				Assert::AreEqual(d1.Find(20.f).second, true);
				Assert::AreEqual(d1.Find(30.f).second, false);

			}

			{
				Datum d1;
				d1.SetType(DatumTypes::String);
				d1.Reserve(10);
				Assert::AreEqual(d1.Size(), (std::size_t)0);
				Assert::AreEqual(d1.Capacity(), (std::size_t)10);
				std::string a = "abc";
				d1.PushBack(a);
				d1.PushBack("abc1");
				Assert::AreEqual(d1.Size(), (std::size_t)2);
				Assert::AreEqual(d1.Find("abc1").second, true);
				Assert::AreEqual(d1.Find("dasdsa").second, false);

			}

			{
				Datum d1;
				d1.SetType(DatumTypes::Matrix);
				d1.Reserve(10);
				Assert::AreEqual(d1.Size(), (std::size_t)0);
				Assert::AreEqual(d1.Capacity(), (std::size_t)10);
				glm::mat4 a(1);
				glm::mat4 b(2);
				glm::mat4 b1(22);
				d1.PushBack(a);
				d1.PushBack(b);
				Assert::AreEqual(d1.Size(), (std::size_t)2);
				Assert::AreEqual(d1.Find(a).second, true);
				Assert::AreEqual(d1.Find(b1).second, false);

			}
			{
				Datum d1;
				d1.SetType(DatumTypes::Vector);
				d1.Reserve(10);
				Assert::AreEqual(d1.Size(), (std::size_t)0);
				Assert::AreEqual(d1.Capacity(), (std::size_t)10);
				glm::vec4 a(1);
				glm::vec4 b(2);
				glm::vec4 b1(22);
				d1.PushBack(a);
				d1.PushBack(b);
				Assert::AreEqual(d1.Size(), (std::size_t)2);
				Assert::AreEqual(d1.Find(a).second, true);
				Assert::AreEqual(d1.Find(b1).second, false);
			}

			{
				Datum d1;
				d1.SetType(DatumTypes::Pointer);
				d1.Reserve(10);
				Assert::AreEqual(d1.Size(), (std::size_t)0);
				Assert::AreEqual(d1.Capacity(), (std::size_t)10);
				Foo f1(10);
				Foo f2(100);
				Foo f3(1000);
				RTTI* r = &f1;
				RTTI* r1 = &f2;
				RTTI* r2 = &f3;
				d1.PushBack(r);
				d1.PushBack(r1);
				Assert::AreEqual(d1.Size(), (std::size_t)2);
				Assert::AreEqual(d1.Find(r).second, true);
				Assert::AreEqual(d1.Find(r2).second, false);
			}
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState DatumTests::_startMemState;
}