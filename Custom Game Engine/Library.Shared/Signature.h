#pragma once

#include "Datum.h"
namespace FieaGameEngine
{
	struct Signature
	{
		/// <summary>
		/// Constructor for Signature
		/// </summary>
		/// <param name="name"> Takes a const reference to a std::string </param>
		/// <param name="type"> Takes a copy of a DatumTypes enum class </param>
		/// <param name="numElements"> Takes a copy of a size_t </param>
		/// <param name="offset"> Takes a copy of a size_t </param>
		Signature(const std::string& name, DatumTypes type, size_t numElements, size_t offset);

		/// <summary>
		/// Compiler supplied defaulted Copy Constructor for Signature 
		/// </summary>
		/// <param name="rhs"> Takes a const reference to a Signature </param>
		Signature(const Signature& rhs) = default;

		/// <summary>
		/// Compiler supplied defaulted Move Constructor for Signature 
		/// </summary>
		/// <param name="rhs"> Takes a rValue reference to a Signature </param>
		Signature(Signature&& rhs) = default;

		/// <summary>
		/// Compiler supplied defaulted Copy Assignment Operator for Signature 
		/// </summary>
		/// <param name="rhs"> Takes a const reference to a Signature </param>
		/// <returns> Returns a reference to a Signature </returns>
		Signature& operator=(const Signature& rhs) = default;

		/// <summary>
		/// Compiler supplied defaulted Move Assignment Operator for Signature
		/// </summary>
		/// <param name="rhs"> Takes a rValue reference to a Signature </param>
		/// <returns> Returns a reference to a Signature </returns>
		Signature& operator=(Signature&& rhs) = default;

		/// <summary>
		/// Compiler supplied default Destructor for Signature
		/// </summary>
		~Signature() = default;

		std::string _name;
		DatumTypes _type{ DatumTypes::Unknown };
		size_t _numElements{ 0 };
		size_t _offset{ 0 };
	};
}