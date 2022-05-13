#pragma once
#include <cstdint>
#include <string>
#include <cassert>
#include <stdexcept>
#include <stdio.h>
#include "RTTI.h"
#pragma warning (push)
#pragma warning (disable : 4201)
#include "glm/gtx/string_cast.hpp"
#pragma warning (pop)
#include "glm\glm.hpp"

namespace FieaGameEngine
{
	class Scope;

	enum class DatumTypes
	{
		Unknown,
		Integer,
		Float,
		Vector,
		Matrix,
		String,
		Table,
		Pointer,

		Begin = Integer,
		End = Pointer
	};

	class Datum final
	{
	public:

		friend Scope;

		/// <summary>
		/// Default compiler supplied Constructor for Datum
		/// </summary>
		Datum() = default;

		/// <summary>
		/// Copy Constructor for Datum
		/// </summary>
		/// <param name="other"> Takes a const reference to a Datum </param>
		Datum(const Datum& other);

		/// <summary>
		/// Copy Assignment Operator for Datum
		/// </summary>
		/// <param name="other"> Takes a const reference to a Datum </param>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& operator=(const Datum & other);

		/// <summary>
		/// Move Constructor for Datum
		/// </summary>
		/// <param name="other"> Takes a rvalue reference to a Datum </param>
		Datum(Datum&& other) noexcept;

		/// <summary>
		/// Move Assignment Operator for Datum
		/// </summary>
		/// <param name="other"> Takes a rvalue reference to a Datum </param>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& operator=(Datum&& other) noexcept;

		/// <summary>
		/// Copy Assignment Operator Integer overload for Datum
		/// </summary>
		/// <param name="i"> Takes a copy of an int </param>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& operator=(int i);

		/// <summary>
		/// Copy Assignment Operator Float overload for Datum 
		/// </summary>
		/// <param name="f"> Takes a copy of a float </param>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& operator=(float f);

		/// <summary>
		/// Copy Assignment Operator GLM Vector overload for Datum 
		/// </summary>
		/// <param name="f"> Takes a const reference to a glm::vec4 </param>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& operator=(const glm::vec4& v);

		/// <summary>
		/// Copy Assignment Operator GLM Matrix overload for Datum 
		/// </summary>
		/// <param name="f"> Takes a const reference to a glm::mat4 </param>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& operator=(const glm::mat4& m);

		/// <summary>
		/// Copy Assignment Operator String overload for Datum
		/// </summary>
		/// <param name="s"> Takes a const reference to a std::string </param>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& operator=(const std::string& s);

		/// <summary>
		/// Copy Assignment Operator RTTI Pointer overload for Datum
		/// </summary>
		/// <param name="r"> Takes a pointer to an RTTI </param>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& operator=(RTTI* r);
		
		/// <summary>
		/// Destructor for Datum
		/// </summary>
		~Datum();

		/// <summary>
		/// Typecast Constructor Integer overload for Datum 
		/// </summary>
		/// <param name="i"> Takes a copy of an int </param>
		Datum(int i);

		/// <summary>
		/// Typecast Constructor Float overload for Datum
		/// </summary>
		/// <param name="f"> Takes a copy of a float </param>
		Datum(float f);

		/// <summary>
		/// Typecast Constructor GLM Vector overload for Datum
		/// </summary>
		/// <param name="f"> Takes a const reference to a glm::vec4 </param>
		Datum(const glm::vec4& v);

		/// <summary>
		/// Typecast Constructor GLM Matrix overload for Datum
		/// </summary>
		/// <param name="f"> Takes a const reference to a glm::mat4 </param>
		Datum(const glm::mat4& m);

		/// <summary>
		/// Typecast Constructor String overload for Datum
		/// </summary>
		/// <param name="f"> Takes a const reference to a glm::vec4 </param>
		Datum(const std::string& s);

		/// <summary>
		/// Typecast Constructor Scope Table overload for Datum
		/// </summary>
		/// <param name="t"> Takes a pointer to a Scope </param>
		Datum(Scope& t);

		/// <summary>
		/// Typecast Constructor RTTI Pointer overload for Datum
		/// </summary>
		/// <param name="r"> Takes a pointer to an RTTI </param>
		Datum(RTTI* r);

		/// <summary>
		/// Operator [] overload for Datum
		/// </summary>
		/// <param name="index"> Takes a copy of a uint32_t </param>
		/// <returns> Returns a reference to a Scope </returns>
		Scope& operator[](size_t index);

		/// <summary>
		/// Equality Operator for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a Datum </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator==(const Datum& value) const;

		/// <summary>
		/// Equality Operator Integer Overload for Datum
		/// </summary>
		/// <param name="value"> Takes a copy of an int </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator==(int value) const;

		/// <summary>
		/// Equality Operator Float Overload for Datum
		/// </summary>
		/// <param name="value"> Takes a copy of a float </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator==(float value) const;

		/// <summary>
		/// Equality Operator GLM Vector Overload for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a glm::vec4 </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator==(const glm::vec4& value) const;

		/// <summary>
		/// Equality Operator GLM Matrix Overload for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a glm::mat4 </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator==(const glm::mat4& value) const;

		/// <summary>
		/// Equality Operator String Overload for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a Datum </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator==(const std::string& value) const;

		/// <summary>
		/// Equality Operator RTTI Pointer Overload for Datum
		/// </summary>
		/// <param name="value"> Takes a pointer to a RTTI </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator==(RTTI* value) const;

		/// <summary>
		/// In-equality operator for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a Datum </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator!=(const Datum& value) const;

		/// <summary>
		/// In-equality operator Integer Overload for Datum
		/// </summary>
		/// <param name="value"> Takes a copy of an int </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator!=(int value) const;

		/// <summary>
		/// In-equality operator Float Overload for Datum
		/// </summary>
		/// <param name="value"> Takes a copy of a float </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator!=(float value) const;

		/// <summary>
		/// In-equality operator GLM Vector Overload for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a glm::vec4 </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator!=(const glm::vec4& value) const;

		/// <summary>
		/// In-equality operator GLM Matrix Overload for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a glm::mat4 </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator!=(const glm::mat4& value) const;

		/// <summary>
		/// In-equality operator String Overload for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a std::string </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator!=(const std::string& value) const;

		/// <summary>
		/// In-equality operator RTTI Pointer Overload for Datum
		/// </summary>
		/// <param name="value"> Takes a pointer to a RTTI </param>
		/// <returns> Returns a copy of a bool </returns>
		bool operator!=(RTTI* value) const;

		/// <summary>
		/// Set Integer overload for Datum
		/// </summary>
		/// <param name="value"> Takes a copy of an int </param>
		/// <param name="index"> Takes a copy of a size_t </param>
		void Set(int value, size_t index = 0);

		/// <summary>
		/// Set Float overload for Datum
		/// </summary>
		/// <param name="value"> Takes a copy of an float </param>
		/// <param name="index"> Takes a copy of a size_t </param>
		void Set(float value, size_t index = 0);

		/// <summary>
		/// Set GLM Vector overload for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a glm::vec4 </param>
		/// <param name="index"> Takes a copy of a size_t </param>
		void Set(const glm::vec4& value, size_t index = 0);

		/// <summary>
		/// Set GLM Matrix overload for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a glm::mat4 </param>
		/// <param name="index"> Takes a copy of a size_t </param>
		void Set(const glm::mat4& value, size_t index = 0);

		/// <summary>
		/// Set String overload for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a std::string </param>
		/// <param name="index"> Takes a copy of a size_t </param>
		void Set(const std::string& value, size_t index = 0);

		/// <summary>
		/// Set Scope Table overload for Datum
		/// </summary>
		/// <param name="value"> Takes a pointer to a Scope </param>
		/// <param name="index"> Takes a copy of a size_t </param>
		void Set(Scope& value, size_t index = 0);

		/// <summary>
		/// Set RTII Pointer overload for Datum
		/// </summary>
		/// <param name="value"> Takes a pointer to a RTTI </param>
		/// <param name="index"> Takes a copy of a size_t </param>
		void Set(RTTI* value, size_t index = 0);

		/// <summary>
		/// Set Storage Integer overload for Datum
		/// </summary>
		/// <param name="intArray"> Takes a pointer to an int </param>
		/// <param name="size"> Takes a copy of a size_t </param>
		void SetStorage(int* intArray, size_t size);

		/// <summary>
		/// Set Storage Float overload for Datum
		/// </summary>
		/// <param name="floatArray"> Takes a pointer to an float </param>
		/// <param name="size"> Takes a copy of a size_t </param>
		void SetStorage(float* floatArray, size_t size);

		/// <summary>
		/// Set Storage GLM Vector overload for Datum
		/// </summary>
		/// <param name="vecArray"> Takes a pointer to an glm::vec4 </param>
		/// <param name="size"> Takes a copy of a size_t </param>
		void SetStorage(glm::vec4* vecArray, size_t size);

		/// <summary>
		/// Set Storage GLM Matrix overload for Datum
		/// </summary>
		/// <param name="matArray"> Takes a pointer to an glm::mat4 </param>
		/// <param name="size"> Takes a copy of a size_t </param>
		void SetStorage(glm::mat4* matArray, size_t size);

		/// <summary>
		/// Set Storage String overload for Datum
		/// </summary>
		/// <param name="stringArray"> Takes a pointer to an std::string </param>
		/// <param name="size"> Takes a copy of a size_t </param>
		void SetStorage(std::string* stringArray, size_t size);

		/// <summary>
		/// Set Storage RTTI Pointer overload for Datum
		/// </summary>
		/// <param name="pointerArray"> Takes a double pointer to a RTTI </param>
		/// <param name="size"> Takes a copy of a size_t </param>
		void SetStorage(RTTI** pointerArray, size_t size);

		/// <summary>
		/// Set Storage Scope Table overload for Datum
		/// </summary>
		/// <param name="pointerArray"> Takes a double pointer to a Scope </param>
		/// <param name="size"> Takes a copy of a size_t </param>
		void SetStorage(Scope** pointerArray, size_t size);

		/// <summary>
		/// Set From String for Datum 
		/// </summary>
		/// <param name="inputString"> Takes a const reference to a std::string </param>
		/// <param name="index"> Takes a copy of a size_t </param>
		void SetFromString(const std::string& inputString, size_t index = 0);

		/// <summary>
		/// PushBackFromString method for Datum
		/// </summary>
		/// <param name="inputString"> Takes a const reference to a std::string </param>
		void PushBackFromString(const std::string& inputString);

		/// <summary>
		/// Get Integer const overload for Datum
		/// </summary>
		/// <param name="index"> Takes a copy of a size_t </param>
		/// <returns> Returns a copy of an int </returns>
		const int& GetInt(size_t index = 0) const;

		/// <summary>
		/// Get Float const overload for Datum 
		/// </summary>
		/// <param name="index"> Takes a copy of a size_t </param>
		/// <returns> Return a copy of a float </returns>
		const float& GetFloat(size_t index = 0) const;

		/// <summary>
		/// Get GLM Vector const overload for Datum
		/// </summary>
		/// <param name="index"> Takes a copy of a size_t </param>
		/// <returns> returns a const reference to a glm::vec4 </returns>
		const glm::vec4& GetVector(size_t index = 0) const;

		/// <summary>
		/// Get GLM Matrix const overload for Datum
		/// </summary>
		/// <param name="index"> Takes a copy of a size_t </param>
		/// <returns> returns a const reference to a glm::mat4 </returns>
		const glm::mat4& GetMatrix(size_t index = 0) const;

		/// <summary>
		/// Get String const overload for Datum
		/// </summary>
		/// <param name="index"> Takes a copy of a size_t </param>
		/// <returns> returns a const reference to a std::string </returns>
		const std::string& GetString(size_t index = 0) const;

		/// <summary>
		/// Get Scope Table const overload for Datum
		/// </summary>
		/// <param name="index"> Takes a copy of a size_t </param>
		/// <returns> Returns a pointer to a Scope </returns>
		Scope& GetTable(size_t index = 0) const;

		/// <summary>
		/// Get RTTI Pointer const overload for Datum
		/// </summary>
		/// <param name="index"> Takes a copy of a size_t </param>
		/// <returns> returns a pointer to a RTTI </returns>
		RTTI* GetPointer(size_t index = 0) const;

		/// <summary>
		/// PopBack method for Datum 
		/// </summary>
		void PopBack();

		/// <summary>
		/// PushBack Integer overload for Datum 
		/// </summary>
		/// <param name="i"> Takes a copy of an int </param>
		void PushBack(int i);

		/// <summary>
		/// PushBack Float overload for Datum
		/// </summary>
		/// <param name="f"> Takes a copy of a float </param>
		void PushBack(float f);

		/// <summary>
		/// PushBack GLM Vector overload for Datum
		/// </summary>
		/// <param name="v"> Takes a const reference to a glm::vec4 </param>
		void PushBack(const glm::vec4& v);

		/// <summary>
		/// PushBack GLM Matrix overload for Datum
		/// </summary>
		/// <param name="m"> Takes a const reference to a glm::mat4 </param>
		void PushBack(const glm::mat4& m);

		/// <summary>
		/// PushBack String overload for Datum
		/// </summary>
		/// <param name="s"> Takes a const reference to a std::string </param>
		void PushBack(const std::string& s);

		/// <summary>
		/// PushBack Scope Pointer overload for Datum
		/// </summary>
		/// <param name="t"> Takes a pointer to a scope </param>
		void PushBack(Scope& t);

		/// <summary>
		/// PushBack RTTI Pointer overload for Datum
		/// </summary>
		/// <param name="r"> Takes a copy of a RTTI pointer </param>
		void PushBack(RTTI* r);

		/// <summary>
		/// Front Integer overload for Datum
		/// </summary>
		/// <returns> Returns a reference to an int </returns>
		int& IntFront();

		/// <summary>
		/// Front Integer const overload for Datum
		/// </summary>
		/// <returns> Returns a copy of an int </returns>
		int IntFront() const;

		/// <summary>
		/// Back Integer overload for Datum
		/// </summary>
		/// <returns> Returns a reference to an int </returns>
		int& IntBack();

		/// <summary>
		/// Back Integer const overload for Datum
		/// </summary>
		/// <returns> Returns a copy of an int </returns>
		int IntBack() const;

		/// <summary>
		/// Front Float overload for Datum 
		/// </summary>
		/// <returns> Returns a reference to a float </returns>
		float& FloatFront();

		/// <summary>
		/// Front Float const overload for Datum 
		/// </summary>
		/// <returns> Returns a copy of a float </returns>
		float FloatFront() const;

		/// <summary>
		/// Back Float overload for Datum 
		/// </summary>
		/// <returns> Returns a reference to a float </returns>
		float& FloatBack();
		
		/// <summary>
		/// Back Float const overload for Datum
		/// </summary>
		/// <returns> Returns a copy of a float </returns>
		float FloatBack() const;

		/// <summary>
		/// Front GLM Vector overload for Datum 
		/// </summary>
		/// <returns> Returns a reference to a glm::vec4 </returns>
		glm::vec4& VectorFront();

		/// <summary>
		/// Front GLM Vector const overload for Datum 
		/// </summary>
		/// <returns> Returns a const reference to a glm::vec4 </returns>
		const glm::vec4& VectorFront() const;

		/// <summary>
		/// Back GLM Vector overload for Datum
		/// </summary>
		/// <returns> Returns a reference to a glm::vec4 </returns>
		glm::vec4& VectorBack();

		/// <summary>
		/// Back GLM Vector const overload for Datum
		/// </summary>
		/// <returns> Returns a const reference to a glm::vec4 </returns>
		const glm::vec4& VectorBack() const;

		/// <summary>
		/// Front GLM Matrix overload for Datum
		/// </summary>
		/// <returns> Returns a reference to a glm::vec4 </returns>
		glm::mat4& MatrixFront();

		/// <summary>
		/// Front GLM Matrix const overload for Datum
		/// </summary>
		/// <returns> Returns a const reference to a glm::vec4 </returns>
		const glm::mat4& MatrixFront() const;

		/// <summary>
		/// Back GLM Matrix overload for Datum
		/// </summary>
		/// <returns> Returns a reference to a glm::vec4 </returns>
		glm::mat4& MatrixBack();

		/// <summary>
		/// Back GLM Matrix const overload for Datum
		/// </summary>
		/// <returns> Returns a const reference to a glm::vec4 </returns>
		const glm::mat4& MatrixBack() const;

		/// <summary>
		/// Front String overload for Datum
		/// </summary>
		/// <returns> Returns a reference to a std::string </returns>
		std::string& StringFront();

		/// <summary>
		/// Front String const overload for Datum
		/// </summary>
		/// <returns> Returns a const reference to a std::string </returns>
		const std::string& StringFront() const;

		/// <summary>
		/// Back String overload for Datum
		/// </summary>
		/// <returns> Returns a reference to a std::string </returns>
		std::string& StringBack();

		/// <summary>
		/// Back String const overload for Datum
		/// </summary>
		/// <returns> Returns a const reference to a std::string </returns>
		const std::string& StringBack() const;

		/// <summary>
		/// Front RTTI Pointer overload for Datum
		/// </summary>
		/// <returns> Returns a reference to a RTTI Pointer </returns>
		RTTI*& PointerFront();

		/// <summary>
		/// Front RTTI Pointer const overload for Datum
		/// </summary>
		/// <returns> Returns a copy of a RTTI Pointer </returns>
		RTTI* PointerFront() const;

		/// <summary>
		/// Back RTTI Pointer overload for Datum
		/// </summary>
		/// <returns> Returns a reference to a RTTI Pointer </returns>
		RTTI*& PointerBack();

		/// <summary>
		/// Back RTTI Pointer const overload for Datum
		/// </summary>
		/// <returns> Returns a copy of a RTTI Pointer </returns>
		RTTI* PointerBack() const;

		/// <summary>
		/// RemoveAt method for Datum
		/// </summary>
		/// <param name="index"> Takes a copy of a size_t </param>
		void RemoveAt(size_t index);

		/// <summary>
		/// Remove Integer overload for Datum
		/// </summary>
		/// <param name="value"> Takes a copy of an int </param>
		/// <returns> Returns a copy of a bool </returns>
		bool Remove(int value);

		/// <summary>
		/// Remove Float overload for Datum
		/// </summary>
		/// <param name="value"> Takes a copy of an float </param>
		/// <returns> Returns a copy of a bool </returns>
		bool Remove(float value);

		/// <summary>
		/// Remove GLM Vector overload for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a glm::vec4 </param>
		/// <returns> Returns a copy of a bool </returns>
		bool Remove(const glm::vec4& value);

		/// <summary>
		/// Remove GLM Matrix overload for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a glm::mat4 </param>
		/// <returns> Returns a copy of a bool </returns>
		bool Remove(const glm::mat4& value);

		/// <summary>
		/// Remove String overload for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a std::string </param>
		/// <returns> Returns a copy of a bool </returns>
		bool Remove(const std::string& value);

		/// <summary>
		/// Remove RTTI Pointer overload for Datum
		/// </summary>
		/// <param name="value"> Takes a pointer to a RTTI </param>
		/// <returns> Returns a copy of a bool </returns>
		bool Remove(RTTI* value);

		bool Remove(Scope& value);

		/// <summary>
		/// Find Integer overload for Datum
		/// </summary>
		/// <param name="value"> Takes a copy of an int </param>
		/// <returns> Returns a copy of a std::pair<size_t, bool> </returns>
		std::pair<size_t, bool> Find(int value) const;

		/// <summary>
		/// Find Float overload for Datum
		/// </summary>
		/// <param name="value"> Takes a copy of an float </param>
		/// <returns> Returns a copy of a std::pair<size_t, bool> </returns>
		std::pair<size_t, bool> Find(float value) const;

		/// <summary>
		/// Find GLM Vector overload for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a glm::vec4 </param>
		/// <returns> Returns a copy of a std::pair<size_t, bool> </returns>
		std::pair<size_t, bool> Find(const glm::vec4& value) const;

		/// <summary>
		/// Find GLM Matrix overload for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a glm::mat4 </param>
		/// <returns> Returns a copy of a std::pair<size_t, bool> </returns>
		std::pair<size_t, bool> Find(const glm::mat4& value) const;

		/// <summary>
		/// Find String overload for Datum
		/// </summary>
		/// <param name="value"> Takes a const reference to a std::string </param>
		/// <returns> Returns a copy of a std::pair<size_t, bool> </returns>
		std::pair<size_t, bool> Find(const std::string& value) const;

		/// <summary>
		/// Find RTTI Pointer overload for Datum
		/// </summary>
		/// <param name="value"> Takes a pointer to a RTTI </param>
		/// <returns> Returns a copy of a std::pair<size_t, bool> </returns>
		std::pair<size_t, bool> Find(RTTI* value) const;

		/// <summary>
		/// Find Scope Table overload for Datum
		/// </summary>
		/// <param name="value"> Takes a pointer to a Scope </param>
		/// <returns> Returns a copy of a std::par<size_t, bool> </returns>
		std::pair<size_t, bool> Find(const Scope& value) const;

		/// <summary>
		/// Reserve method for Datum
		/// </summary>
		/// <param name="capacity"> Takes a copy of a size_t </param>
		void Reserve(size_t capacity);

		/// <summary>
		/// Resize method for Datum
		/// </summary>
		/// <param name="size"> Takes a copy of a size_t </param>
		void Resize(size_t size);

		/// <summary>
		/// ShrinkToFit method for Datum 
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Clear method for Datum 
		/// </summary>
		void Clear();

		/// <summary>
		/// SetType method for Datum 
		/// </summary>
		/// <param name="type"> Takes a copy of a DatumType enum class </param>
		void SetType(DatumTypes type);

		/// <summary>
		/// Type method for Datum
		/// </summary>
		/// <returns> Returns a copy of a DatumType enum class </returns>
		DatumTypes Type() const;

		/// <summary>
		/// Size method for Datum
		/// </summary>
		/// <returns> Returns a copy of a size_t </returns>
		size_t Size() const;

		/// <summary>
		/// Capacity method for Datum
		/// </summary>
		/// <returns> Returns a copy of a size_t </returns>
		size_t Capacity() const;
		 
		/// <summary>
		/// ToString method for Datum
		/// </summary>
		/// <param name="index"> Takes a copy of a size_t </param>
		/// <returns> Returns a copy of a std::string </returns>
		std::string ToString(size_t index = 0) const;

		/// <summary>
		/// IsExternal method for Datum
		/// </summary>
		/// <returns> Returns a copy of a bool </returns>
		bool IsExternal() const;
	private:

		/// <summary>
		/// Copy Assignment Operator Scope Table overload for Datum
		/// </summary>
		/// <param name="t"> Takes a pointer to a Scope </param>
		/// <returns> Returns a reference to a Datum </returns>
		Datum& operator=(Scope& t);

		friend class Attribute;
		void SetStorage(DatumTypes type, void* data, size_t size);

		union DatumValues final
		{
			int32_t* i;
			float* f;
			glm::vec4* v;
			glm::mat4* m;
			std::string* s;
			Scope** t;
			RTTI** r;
			void* vp{ nullptr };
		};

		bool isExternal{ false };
		DatumValues _data;
		size_t _capacity{ 0 };
		size_t _size{ 0 };
		DatumTypes _type{ DatumTypes::Unknown };
		static const size_t _typeSizes[static_cast<size_t>(DatumTypes::End) + 1];
	};
}

