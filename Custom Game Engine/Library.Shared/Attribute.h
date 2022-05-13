#pragma once
#include "Scope.h"
#include "TypeRegistry.h"

namespace FieaGameEngine
{
	class Attribute : public Scope
	{
		RTTI_DECLARATIONS(Attribute, Scope)

	protected:
		/// <summary>
		/// Attribute Default Constructor, calls Populate Method
		/// </summary>
		Attribute();

		/// <summary>
		/// Attribute TypeCast Constructor, used to call from Inherited children
		/// </summary>
		/// <param name="typeId"> Takes a copy of a size_t </param>
		Attribute(RTTI::IdType typeId);
		
		/// <summary>
		/// Copy Constructor for Attribute
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an Attribute </param>
		Attribute(const Attribute& rhs);

		/// <summary>
		/// Copy Assignment Operator for Attribute
		/// </summary>
		/// <param name="rhs"> Takes a const reference to an Attribute </param>
		/// <returns> Returns a reference to an Attribute </returns>
		Attribute& operator=(const Attribute& rhs);

		/// <summary>
		/// Move Constructor for Attribute
		/// </summary>
		/// <param name="rhs"> Takes a rValue reference to an Attribute </param>
		Attribute(Attribute&& rhs) noexcept;

		/// <summary>
		/// Move Assignment Operator for Attribute 
		/// </summary>
		/// <param name="rhs"> Takes a rValue reference to an Attribute </param>
		/// <returns> Returns a reference to an Attribute </returns>
		Attribute& operator=(Attribute&& rhs) noexcept;

		/// <summary>
		/// Compiler Supplied Defaulted Destructor for Attribute
		/// </summary>
		~Attribute() = default;

	public:
		/// <summary>
		/// IsAttritbute Method for Attribute
		/// </summary>
		/// <param name="name"> Takes a const reference to a std::string </param>
		/// <returns> Returns a copy of a bool </returns>
		bool IsAttribute(const std::string& name);

		/// <summary>
		/// IsPrescribedAttribute method for Attribute
		/// </summary>
		/// <param name="name"> Takes a const reference to a std::string </param>
		/// <returns> Returns a copy of a bool </returns>
		bool IsPrescribedAttribute(const std::string& name);

		/// <summary>
		/// IsAuxiliaryAttribute method for Attribute
		/// </summary>
		/// <param name="name"> Takes a const reference to a std::string </param>
		/// <returns> Returns a copy of a bool </returns>
		bool IsAuxiliaryAttribute(const std::string& name);

		/// <summary>
		/// AppendAuxiliaryAttribute method for Attribute, adds Auxiliary attributes 
		/// </summary>
		/// <param name="name"> Takes a const reference to a std::string </param>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& AppendAuxiliaryAttribute(const std::string& name);

		/// <summary>
		/// UpdateExternalStorate method for Attribute, used to update external storage pointers for copy/move semantics
		/// </summary>
		/// <param name="typeInfo"> Takes a reference to a HashMap PairType of <size_t, Vector<Signature>> </param>
		void UpdateExternalStorage(HashMap<RTTI::IdType, Vector<Signature>>::PairType& typeInfo);

		/// <summary>
		/// At method for Attribute, wraps Scope's HashMap At
		/// </summary>
		/// <param name="name"> Takes a const reference to a std::string </param>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& At(const std::string name);

	private:
		/// <summary>
		/// Populate method for Attributes, populates the Signatures from TypeRegistry
		/// </summary>
		/// <param name="typeInfo"> Takes a reference to a HashMap PairType of <size_t, Vector<Signature>> </param>
		void Populate(HashMap<RTTI::IdType, Vector<Signature>>::PairType& typeInfo);

		size_t numPrescribedAttributes{ 0 };
	};
}