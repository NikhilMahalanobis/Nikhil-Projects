#pragma once
#include <cstddef>
#include "Attribute.h"
#include "TypeRegistry.h"
#include "Scope.h"
#include "Factory.h"

namespace UnitsTestsLibraryDesktop
{
	class AttributedFoo : public FieaGameEngine::Attribute
	{
		RTTI_DECLARATIONS(AttributedFoo, FieaGameEngine::Attribute)

	public:
		static const std::size_t ArraySize = 5;

		AttributedFoo();
		AttributedFoo(const AttributedFoo&) = default;
		AttributedFoo(AttributedFoo&&) = default;
		AttributedFoo& operator=(const AttributedFoo&) = default;
		AttributedFoo& operator=(AttributedFoo&&) = default;
		~AttributedFoo() = default;

		int ExternalInteger = 0;
		float ExternalFloat = 0;
		std::string ExternalString;
		glm::vec4 ExternalVector;
		glm::mat4 ExternalMatrix;

		int ExternalIntegerArray[ArraySize];
		float ExternalFloatArray[ArraySize];
		std::string ExternalStringArray[ArraySize];
		glm::vec4 ExternalVectorArray[ArraySize];
		glm::mat4 ExternalMatrixArray[ArraySize];
	
		AttributedFoo* Clone() const override;
		bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override;
		
		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	protected:
		AttributedFoo(RTTI::IdType type);
	};

	ConcreteFactory(AttributedFoo, FieaGameEngine::Scope)
}