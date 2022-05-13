#include "pch.h"
#include "Attribute.h"
#include "AttributedFoo.h"
#include "MonsterFoo.h"
#include <limits>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitsTestsLibraryDesktop
{
	TEST_CLASS(AttributeTests)
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

		TEST_METHOD(RTTITest)
		{
			AttributedFoo a;
			RTTI* rtti = &a;

			Assert::IsTrue(rtti->Is("AttributedFoo"s));
			Assert::IsTrue(rtti->Is("Attribute"s));
			Assert::IsTrue(rtti->Is("AttributedFoo"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsTrue(rtti->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attribute::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(AttributedFoo::TypeIdClass(), rtti->TypeIdInstance());

			AttributedFoo* f = rtti->As<AttributedFoo>();
			Assert::IsNotNull(f);
			Assert::AreEqual(&a, f);
		}

		TEST_METHOD(Attributes)
		{
			AttributedFoo foo;

			Assert::IsTrue(foo.IsAttribute("this"));
			Assert::IsTrue(foo.IsAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsAttribute("ExternalString"));
			Assert::IsTrue(foo.IsAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsAttribute("NestedScope"));
			Assert::IsTrue(foo.IsAttribute("NestedScopeArray"));

			Assert::IsTrue(foo.IsPrescribedAttribute("this"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScopeArray"));

			Assert::IsFalse(foo.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScope"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScopeArray"));

			Assert::AreEqual(&foo, foo["this"].GetPointer()->As<AttributedFoo>());
		}

		TEST_METHOD(InheritedAttributes)
		{
			MonsterFoo foo;

			Assert::IsTrue(foo.IsAttribute("this"));
			Assert::IsTrue(foo.IsAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsAttribute("ExternalString"));
			Assert::IsTrue(foo.IsAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsAttribute("NestedScope"));
			Assert::IsTrue(foo.IsAttribute("NestedScopeArray"));
			Assert::IsTrue(foo.IsAttribute("MonsterExternalInteger"));

			Assert::IsTrue(foo.IsPrescribedAttribute("this"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScopeArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("MonsterExternalInteger"));

			Assert::IsFalse(foo.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScope"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScopeArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("MonsterExternalInteger"));

			Assert::AreEqual(&foo, foo["this"].GetPointer()->As<MonsterFoo>());
		}

		TEST_METHOD(AuxiliaryAttributes)
		{
			AttributedFoo foo;
			
			foo.AppendAuxiliaryAttribute("AuxiliaryInteger");
			Assert::IsTrue(foo.IsAttribute("AuxiliaryInteger"));
			Assert::IsTrue(foo.IsAuxiliaryAttribute("AuxiliaryInteger"));
			Assert::IsFalse(foo.IsPrescribedAttribute("AuxiliaryInteger"));

			Assert::ExpectException<std::runtime_error>([&foo] {foo.AppendAuxiliaryAttribute("ExternalInteger"); });
		}
		
		TEST_METHOD(ExternalAssignment)
		{
			AttributedFoo foo;

			Datum* datum = foo.Find("ExternalInteger");
			Assert::AreEqual(size_t(1), datum->Size());
			Assert::AreSame(datum->GetInt(), foo.ExternalInteger);

			int a = 5;
			datum->Set(a);
			Assert::AreEqual(a, datum->GetInt());
			Assert::AreEqual(a, foo.ExternalInteger);

			float b = 6.0f;
			foo["ExternalFloat"] = b;
			Assert::AreEqual(b, foo.ExternalFloat);
		}

		TEST_METHOD(CopyConstructor)
		{
			AttributedFoo foo;
			AttributedFoo anotherFoo = foo;

			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("this"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("NestedScopeArray"));

			Assert::AreEqual(&anotherFoo, anotherFoo["this"].GetPointer()->As<AttributedFoo>());

			const int& fooData = foo["ExternalInteger"].GetInt();
			const int& anotherFooData = anotherFoo["ExternalInteger"].GetInt();
			Assert::AreNotSame(anotherFooData, fooData);
		}

		TEST_METHOD(CopyAssignmentOperator)
		{
			AttributedFoo foo;
			AttributedFoo anotherFoo;
			anotherFoo = foo;

			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("this"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(anotherFoo.IsPrescribedAttribute("NestedScopeArray"));

			Assert::AreEqual(&anotherFoo, anotherFoo["this"].GetPointer()->As<AttributedFoo>());

			const int& fooData = foo["ExternalInteger"].GetInt();
			const int& anotherFooData = anotherFoo["ExternalInteger"].GetInt();
			Assert::AreNotSame(anotherFooData, fooData);
		}

		TEST_METHOD(MoveConstructor)
		{
			AttributedFoo foo;

			size_t ogSize = foo.Size();
			Assert::AreNotEqual(size_t(0), ogSize);
			Assert::AreNotEqual(size_t(1), ogSize);

			foo.ExternalInteger = 1;
			foo.ExternalFloat = 2.0f;
			foo.ExternalString = "Hello, World!"s;
			foo.ExternalVector = glm::vec4(1, 2, 3, 4);
			foo.ExternalMatrix = glm::mat4(-1);

			char helloWorld[14];
			strcpy_s(helloWorld, foo.ExternalString.c_str());
			string helloWorldString = helloWorld;
			Assert::AreEqual(helloWorldString, foo.ExternalString);

			int integerArray[] = { 1, 2, 3, 4, 5 };
			std::copy(std::begin(integerArray), std::end(integerArray), foo.ExternalIntegerArray);

			AttributedFoo anotherFoo = std::move(foo);
			Assert::AreEqual(ogSize, anotherFoo.Size());
			Assert::AreEqual(size_t(0), foo.Size());
			Assert::AreEqual(1, anotherFoo.ExternalInteger);
			Assert::AreEqual(2.0f, anotherFoo.ExternalFloat);
			Assert::AreEqual(helloWorldString, anotherFoo.ExternalString);
			Assert::AreNotSame(helloWorldString, anotherFoo.ExternalString);
			Assert::IsTrue(foo.ExternalString.empty());
			Assert::AreEqual(anotherFoo.ExternalVector, glm::vec4(1, 2, 3, 4));
			Assert::AreEqual(anotherFoo.ExternalMatrix, glm::mat4(-1));
			Assert::AreEqual(&anotherFoo, anotherFoo["this"].GetPointer()->As<AttributedFoo>());
		}

		TEST_METHOD(MoveAssignmentOperator)
		{
			AttributedFoo foo;

			size_t ogSize = foo.Size();
			Assert::AreNotEqual(size_t(0), ogSize);
			Assert::AreNotEqual(size_t(1), ogSize);

			foo.ExternalInteger = 1;
			foo.ExternalFloat = 2.0f;
			foo.ExternalString = "Hello, World!"s;
			foo.ExternalVector = glm::vec4(1, 2, 3, 4);
			foo.ExternalMatrix = glm::mat4(-1);

			char helloWorld[14];
			strcpy_s(helloWorld, foo.ExternalString.c_str());
			string helloWorldString = helloWorld;
			Assert::AreEqual(helloWorldString, foo.ExternalString);

			int integerArray[] = { 1, 2, 3, 4, 5 };
			std::copy(std::begin(integerArray), std::end(integerArray), foo.ExternalIntegerArray);

			AttributedFoo anotherFoo;
			anotherFoo = std::move(foo);
			Assert::AreEqual(ogSize, anotherFoo.Size());
			Assert::AreEqual(size_t(0), foo.Size());
			Assert::AreEqual(1, anotherFoo.ExternalInteger);
			Assert::AreEqual(2.0f, anotherFoo.ExternalFloat);
			Assert::AreEqual(helloWorldString, anotherFoo.ExternalString);
			Assert::AreNotSame(helloWorldString, anotherFoo.ExternalString);
			Assert::IsTrue(foo.ExternalString.empty());
			Assert::AreEqual(anotherFoo.ExternalVector, glm::vec4(1, 2, 3, 4));
			Assert::AreEqual(anotherFoo.ExternalMatrix, glm::mat4(-1));
			Assert::AreEqual(&anotherFoo, anotherFoo["this"].GetPointer()->As<AttributedFoo>());
		}

		TEST_METHOD(NestedScopeAttributes)
		{
			AttributedFoo foo;
			Datum& nestedScopeDatum = foo.At("NestedScope"s);
			Assert::AreEqual(size_t(0), nestedScopeDatum.Size());

			Datum& nestedScopeArrayDatum = foo.At("NestedScopeArray"s);
			Assert::AreEqual(AttributedFoo::ArraySize, nestedScopeArrayDatum.Size());
		}

		TEST_METHOD(Clone)
		{
			AttributedFoo foo;
			auto clone = foo.Clone();
			auto anotherFoo = clone->As<AttributedFoo>();
			Assert::IsNotNull(anotherFoo);

			Assert::IsTrue(anotherFoo->IsPrescribedAttribute("this"));
			Assert::IsTrue(anotherFoo->IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(anotherFoo->IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(anotherFoo->IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(anotherFoo->IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(anotherFoo->IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(anotherFoo->IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(anotherFoo->IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(anotherFoo->IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(anotherFoo->IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(anotherFoo->IsPrescribedAttribute("ExternalMatrixArray"));

			Assert::AreEqual(anotherFoo, (*anotherFoo)["this"s].GetPointer()->As<AttributedFoo>());

			const int& fooData = foo["ExternalInteger"].GetInt();
			const int& anotherFooData = (*anotherFoo)["ExternalInteger"s].GetInt();
			Assert::AreNotSame(anotherFooData, fooData);

			Assert::AreEqual(foo, *anotherFoo);

			delete clone;
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState AttributeTests::_startMemState;
}