#include "pch.h"
#include "AttributedFoo.h"

namespace UnitsTestsLibraryDesktop
{
	RTTI_DEFINITIONS(AttributedFoo)

	AttributedFoo::AttributedFoo() :
		Attribute(AttributedFoo::TypeIdClass())
	{
	}
	AttributedFoo::AttributedFoo(RTTI::IdType type) :
		Attribute(type)
	{
	}

	const FieaGameEngine::Vector<FieaGameEngine::Signature> UnitsTestsLibraryDesktop::AttributedFoo::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{"ExternalInteger", FieaGameEngine::DatumTypes::Integer, 1, offsetof(AttributedFoo, ExternalInteger)},
			{ "ExternalFloat", FieaGameEngine::DatumTypes::Float, 1, offsetof(AttributedFoo, ExternalFloat) },
			{ "ExternalString", FieaGameEngine::DatumTypes::String, 1, offsetof(AttributedFoo, ExternalString) },
			{ "ExternalVector", FieaGameEngine::DatumTypes::Vector, 1, offsetof(AttributedFoo, ExternalVector) },
			{ "ExternalMatrix", FieaGameEngine::DatumTypes::Matrix, 1, offsetof(AttributedFoo, ExternalMatrix) },
			{ "ExternalIntegerArray", FieaGameEngine::DatumTypes::Integer, ArraySize, offsetof(AttributedFoo, ExternalIntegerArray) },
			{ "ExternalFloatArray", FieaGameEngine::DatumTypes::Float, ArraySize, offsetof(AttributedFoo, ExternalFloatArray) },
			{ "ExternalStringArray", FieaGameEngine::DatumTypes::String, ArraySize, offsetof(AttributedFoo, ExternalStringArray) },
			{ "ExternalVectorArray", FieaGameEngine::DatumTypes::Vector, ArraySize, offsetof(AttributedFoo, ExternalVectorArray) },
			{ "ExternalMatrixArray", FieaGameEngine::DatumTypes::Matrix, ArraySize, offsetof(AttributedFoo, ExternalMatrixArray) },
			{ "NestedScope", FieaGameEngine::DatumTypes::Table, 0, 0 },
			{ "NestedScopeArray", FieaGameEngine::DatumTypes::Table, ArraySize, 0 }
		};
	}

	AttributedFoo* AttributedFoo::Clone() const
	{
		return new AttributedFoo(*this);
	}

	bool AttributedFoo::Equals(const RTTI* rhs) const
	{
		const auto rhsFoo = rhs->As<AttributedFoo>();
		if (rhsFoo == nullptr)
		{
			return false;
		}

		return ExternalInteger == rhsFoo->ExternalInteger &&
			ExternalFloat == rhsFoo->ExternalFloat &&
			ExternalString == rhsFoo->ExternalString &&
			ExternalVector == rhsFoo->ExternalVector &&
			ExternalMatrix == rhsFoo->ExternalMatrix &&
			ExternalFloat == rhsFoo->ExternalFloat &&
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsFoo->ExternalIntegerArray)) &&
			std::equal(std::begin(ExternalFloatArray), std::end(ExternalFloatArray), std::begin(rhsFoo->ExternalFloatArray)) &&
			std::equal(std::begin(ExternalStringArray), std::end(ExternalStringArray), std::begin(rhsFoo->ExternalStringArray)) &&
			std::equal(std::begin(ExternalVectorArray), std::end(ExternalVectorArray), std::begin(rhsFoo->ExternalVectorArray)) &&
			std::equal(std::begin(ExternalMatrixArray), std::end(ExternalMatrixArray), std::begin(rhsFoo->ExternalMatrixArray));
	}

	std::string AttributedFoo::ToString() const
	{
		return "AttributedFood"s;
	}

}
