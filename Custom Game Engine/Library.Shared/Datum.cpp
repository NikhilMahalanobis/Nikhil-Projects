#include "Datum.h"

namespace FieaGameEngine
{
	const size_t Datum::_typeSizes[] =
	{
		0,					 //Unknown
		sizeof(int32_t),	 //Integer
		sizeof(float),		 //Float
		sizeof(glm::vec4),	 //Vec4
		sizeof(glm::mat4),	 //Mat4
		sizeof(std::string), //String
		sizeof(Scope*),		 //Scope*
		sizeof(RTTI*)		 //RTTI*
	};

	Datum& Datum::operator=(int i)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Integer)
		{
			throw std::runtime_error("Error, cannot scalar assign to different types!");
		}

		SetType(DatumTypes::Integer);

		if (_size == 0)
		{
			PushBack(i);
			
		}
		else
		{
			Set(i);
		}


		return *this;
	}

	Datum& Datum::operator=(float f)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Float)
		{
			throw std::runtime_error("Error, cannot scalar assign to different types!");
		}

		SetType(DatumTypes::Float);

		if (_size == 0)
		{
			PushBack(f);

		}
		else
		{
			Set(f);
		}


		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& v)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Vector)
		{
			throw std::runtime_error("Error, cannot scalar assign to different types!");
		}

		SetType(DatumTypes::Vector);

		if (_size == 0)
		{
			PushBack(v);

		}
		else
		{
			Set(v);
		}


		return *this;
	}

	Datum& Datum::operator=(const glm::mat4& m)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Error, cannot scalar assign to different types!");
		}

		SetType(DatumTypes::Matrix);

		if (_size == 0)
		{
			PushBack(m);

		}
		else
		{
			Set(m);
		}


		return *this;
	}

	Datum& Datum::operator=(const std::string& s)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::String)
		{
			throw std::runtime_error("Error, cannot scalar assign to different types!");
		}

		SetType(DatumTypes::String);

		if (_size == 0)
		{
			PushBack(s);

		}
		else
		{
			Set(s);
		}


		return *this;
	}

	Datum& Datum::operator=(Scope& t)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Table)
		{
			throw std::runtime_error("Error, cannot scalar assign to different types!");
		}

		SetType(DatumTypes::Table);

		if (_size == 0)
		{
			PushBack(t);

		}
		else
		{
			Set(t);
		}
		return *this;
	}

	Datum& Datum::operator=(RTTI* r)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Error, cannot scalar assign to different types!");
		}

		SetType(DatumTypes::Pointer);

		if (_size == 0)
		{
			PushBack(r);

		}
		else
		{
			Set(r);
		}


		return *this;
	}

	Datum::Datum(const Datum& other):
		_capacity(other._capacity), isExternal(other.isExternal), _type(other._type)
	{
		if (isExternal)
		{
			_data.vp = other._data.vp;
			_size = other._size;
		}

		else
		{
			if (_type == DatumTypes::Unknown)
			{
				_size = other._size;
			}
			else if (_type == DatumTypes::String)
			{
				_data.s = reinterpret_cast<std::string*>(malloc(other._capacity * _typeSizes[static_cast<size_t>(_type)]));
				for (size_t i = 0; i < other._size; ++i)
				{
					PushBack(other.GetString(i));
				}
			}

			else
			{
				size_t size = _typeSizes[static_cast<size_t>(_type)];
				auto temp = malloc(_capacity * size);
				assert(temp != nullptr);
				_data.vp = temp;

				auto lhsData = reinterpret_cast<uint8_t*>(_data.vp);
				auto rhsData = reinterpret_cast<uint8_t*>(other._data.vp);
				std::memcpy(lhsData, rhsData, size * other._size);
				_size = other._size;
			}
		}
	}

	Datum& Datum::operator=(const Datum& other)
	{	
		// EXTERNAL = EXTERNAL
		if (isExternal && other.isExternal)
		{
			_data.vp = other._data.vp;
			_size = other._size;
			_capacity = other._capacity;
			_type = other._type;
		}

		// EXTERNAL = INTERNAL
		if (isExternal && !other.isExternal)
		{
			_data.vp = nullptr;
			_capacity = other._capacity;
			_type = other._type;

			size_t size = _typeSizes[static_cast<size_t>(other._type)];
			auto temp = realloc(_data.vp, size * other._capacity);
			assert(temp != nullptr);
			_data.vp = temp;

			if (_type == DatumTypes::String)
			{
				_size = 0;
				isExternal = false;
				for (size_t i = 0; i < other._size; ++i)
				{
					PushBack(other.GetString(i));
				}
				isExternal = true;
			}

			else
			{
				auto lhsData = reinterpret_cast<uint8_t*>(_data.vp);
				auto rhsData = reinterpret_cast<uint8_t*>(other._data.vp);
				std::memcpy(lhsData, rhsData, size * other._size);
				_size = other._size;
			}

			isExternal = other.isExternal;
		}

		// INTERNAL = INTERNAL
		if (!isExternal && !other.isExternal)
		{ 
			Clear();

			_capacity = other._capacity;
			_type = other._type;

			size_t size = _typeSizes[static_cast<size_t>(other._type)];
			auto temp = realloc(_data.vp, size * other._capacity);
			assert(temp != nullptr);
			_data.vp = temp;

			if (_type == DatumTypes::String)
			{
				for (size_t i = 0; i < other._size; ++i)
				{
					PushBack(other.GetString(i));
				}
			}

			else
			{
				auto lhsData = reinterpret_cast<uint8_t*>(_data.vp);
				auto rhsData = reinterpret_cast<uint8_t*>(other._data.vp);
				std::memcpy(lhsData, rhsData, size * other._size);
				_size = other._size;
			}
		}

		// INTERAL = EXTERNAL
		if (!isExternal && other.isExternal)
		{	
			Clear();
			free(_data.vp);

			_size = other._size;
			_capacity = other._capacity;
			_type = other._type;
			_data.vp = other._data.vp;
			isExternal = other.isExternal;
		}

		return *this;
	}

	Datum::Datum(Datum&& other) noexcept :
		_size(other._size), _capacity(other._capacity), _type(other._type), isExternal(other.isExternal), _data(other._data)
	{
		other._data.vp = nullptr;
		other._size = other._capacity = 0;
		other._type = DatumTypes::Unknown;
		other.isExternal = false;
	}

	Datum& Datum::operator=(Datum&& other) noexcept
	{
		this->~Datum();

		_size = other._size;
		_capacity = other._capacity;
		_type = other._type;
		isExternal = other.isExternal;
		_data.vp = other._data.vp;

		other._data.vp = nullptr;
		other._size = other._capacity = 0;
		other._type = DatumTypes::Unknown;
		other.isExternal = false;

		return *this;
	}

	Datum::~Datum()
	{
		Clear();
		free(_data.vp);
	}

	Datum::Datum(int i) :
		_size(0), _capacity(3), _type(DatumTypes::Integer)
	{
		_data.i = reinterpret_cast<int*>(malloc(_capacity * _typeSizes[static_cast<size_t>(_type)]));
		PushBack(i);
	}

	Datum::Datum(float f) :
		_size(0), _capacity(3), _type(DatumTypes::Float)
	{
		_data.f = reinterpret_cast<float*>(malloc(_capacity * _typeSizes[static_cast<size_t>(_type)]));
		PushBack(f);
	}

	Datum::Datum(const glm::vec4& v) :
		_size(0), _capacity(3), _type(DatumTypes::Vector)
	{
		_data.v = reinterpret_cast<glm::vec4*>(malloc(_capacity * _typeSizes[static_cast<size_t>(_type)]));
		PushBack(v);
	}

	Datum::Datum(const glm::mat4& m) :
		_size(0), _capacity(3), _type(DatumTypes::Matrix)
	{
		_data.m = reinterpret_cast<glm::mat4*>(malloc(_capacity * _typeSizes[static_cast<size_t>(_type)]));
		PushBack(m);
	}

	Datum::Datum(const std::string& s) :
		_size(0), _capacity(3), _type(DatumTypes::String)
	{
		_data.s = reinterpret_cast<std::string*>(malloc(_capacity * _typeSizes[static_cast<size_t>(_type)]));
		PushBack(s);
	}

	Datum::Datum(Scope& t) :
		_size(0), _capacity(3), _type(DatumTypes::Table)
	{ 
		_data.t = reinterpret_cast<Scope**>(malloc(_capacity * _typeSizes[static_cast<size_t>(_type)]));
		PushBack(t);
	}

	Datum::Datum(RTTI* r) :
		_size(0), _capacity(3), _type(DatumTypes::Pointer)
	{
		_data.r = reinterpret_cast<RTTI**>(malloc(_capacity * _typeSizes[static_cast<size_t>(_type)]));
		PushBack(r);
	}

	Scope& Datum::operator[](size_t index)
	{
		return GetTable(index);
	}

	bool Datum::operator==(const Datum& value) const
	{
		if (_type != value._type || _size != value._size)
		{
			return false;
		}

		if (_type == DatumTypes::String)
		{
			bool allMatch = true;
			for (size_t i = 0; i < _size; ++i)
			{
				if (_data.s[i].compare(value._data.s[i]) != 0)
				{
					allMatch = false;
					break;
				}
			}

			return allMatch;
		}

		else if (_type == DatumTypes::Pointer)
		{
			bool allMatch = true;
			for (size_t i = 0; i < _size; ++i)
			{
				if (!(_data.r[i]->Equals(value._data.r[i])))
				{
					allMatch = false;
					break;
				}
			}
			return allMatch;
		}

		else
		{
			int compareReturnVal = std::memcmp(_data.vp, value._data.vp, _size * _typeSizes[static_cast<size_t>(_type)]);
			return compareReturnVal == 0;
		}
	}

	bool Datum::operator==(int value) const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Error, datum is empty!");
		}

		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Error, type argument is incorrect, does not match!");
		}

		return (_data.i[0] == value);
	}

	bool Datum::operator==(float value) const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Error, datum is empty!");
		}

		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Error, type argument is incorrect, does not match!");
		}

		return (_data.f[0] == value);
	}

	bool Datum::operator==(const glm::vec4& value) const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Error, datum is empty!");
		}

		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Error, type argument is incorrect, does not match!");
		}

		return (_data.v[0] == value);
	}

	bool Datum::operator==(const glm::mat4& value) const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Error, datum is empty!");
		}

		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Error, type argument is incorrect, does not match!");
		}

		return (_data.m[0] == value);
	}

	bool Datum::operator==(const std::string& value) const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Error, datum is empty!");
		}

		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Error, type argument is incorrect, does not match!");
		}

		return (_data.s[0] == value);
	}

	bool Datum::operator==(RTTI* value) const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Error, datum is empty!");
		}

		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Error, type argument is incorrect, does not match!");
		}

		return (_data.r[0] == value);
	}

	bool Datum::operator!=(const Datum& value) const
	{
		return !(operator==(value));
	}

	bool Datum::operator!=(int value) const
	{
		return !(operator==(value));
	}

	bool Datum::operator!=(float value) const
	{
		return !(operator==(value));
	}

	bool Datum::operator!=(const glm::vec4& value) const
	{
		return !(operator==(value));
	}

	bool Datum::operator!=(const glm::mat4& value) const
	{
		return !(operator==(value));
	}

	bool Datum::operator!=(const std::string& value) const
	{
		return !(operator==(value));
	}

	bool Datum::operator!=(RTTI* value) const
	{
		return !(operator==(value));
	}

	void Datum::Set(int value, size_t index)
	{
		if (_type != DatumTypes::Integer)
		{
			if (_type != DatumTypes::Unknown)
			{
				throw std::runtime_error("Error, type is incorrect!");
			}
			else
			{
				_type = DatumTypes::Integer;
			}
		}

		if (index >= _size)
		{
			throw std::runtime_error("Error, index is out of bounds!");
		}

		_data.i[index] = value;
	}

	void Datum::Set(float value, size_t index)
	{
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (index >= _size)
		{
			throw std::runtime_error("Error, index is out of bounds!");
		}

		_data.f[index] = value;
	}

	void Datum::Set(const glm::vec4& value, size_t index)
	{
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (index >= _size)
		{
			throw std::runtime_error("Error, index is out of bounds!");
		}

		_data.v[index] = value;
	}

	void Datum::Set(const glm::mat4& value, size_t index)
	{
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (index >= _size)
		{
			throw std::runtime_error("Error, index is out of bounds!");
		}

		_data.m[index] = value;
	}

	void Datum::Set(const std::string& value, size_t index)
	{
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (index >= _size)
		{
			throw std::runtime_error("Error, index is out of bounds!");
		}

		_data.s[index].~basic_string();
		_data.s[index] = value;
	}

	void Datum::Set(Scope& value, size_t index)
	{
		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (index >= _size)
		{
			throw std::runtime_error("Error, index is out of bounds!");
		}

		_data.t[index] = &value;
	}

	void Datum::Set(RTTI* value, size_t index)
	{
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (index >= _size)
		{
			throw std::runtime_error("Error, index is out of bounds!");
		}

		_data.r[index] = value;
	}

	void Datum::SetStorage(int* intArray, size_t size)
	{
		assert(intArray != nullptr && size != 0);

		if (_type != DatumTypes::Integer)
		{
			if (_type != DatumTypes::Unknown)
			{
				throw std::runtime_error("Error, type is already set, incompatible!");
			}
			else
			{
				_type = DatumTypes::Integer;
			}
		}

		if (!isExternal)
		{
			Clear();
			free(_data.vp);
		}

		isExternal = true;
		_data.i = intArray;
		_size = _capacity = size;
	}
	void Datum::SetStorage(float* floatArray, size_t size)
	{
		assert(floatArray != nullptr && size != 0);

		if (_type != DatumTypes::Float)
		{
			if (_type != DatumTypes::Unknown)
			{
				throw std::runtime_error("Error, type is already set, incompatible!");
			}
			else
			{
				_type = DatumTypes::Float;
			}
		}

		if (!isExternal)
		{
			Clear();
			free(_data.vp);
		}

		isExternal = true;
		_data.f = floatArray;
		_size = _capacity = size;
	}
	void Datum::SetStorage(glm::vec4* vecArray, size_t size)
	{
		assert(vecArray != nullptr && size != 0);

		if (_type != DatumTypes::Vector)
		{
			if (_type != DatumTypes::Unknown)
			{
				throw std::runtime_error("Error, type is already set, incompatible!");
			}
			else
			{
				_type = DatumTypes::Vector;
			}
		}

		if (!isExternal)
		{
			Clear();
			free(_data.vp);
		}

		isExternal = true;
		_data.v = vecArray;
		_size = _capacity = size;
	}
	void Datum::SetStorage(glm::mat4* matArray, size_t size)
	{
		assert(matArray != nullptr && size != 0);

		if (_type != DatumTypes::Matrix)
		{
			if (_type != DatumTypes::Unknown)
			{
				throw std::runtime_error("Error, type is already set, incompatible!");
			}
			else
			{
				_type = DatumTypes::Matrix;
			}
		}

		if (!isExternal)
		{
			Clear();
			free(_data.vp);
		}

		isExternal = true;
		_data.m = matArray;
		_size = _capacity = size;
	}
	void Datum::SetStorage(std::string* stringArray, size_t size)
	{
		assert(stringArray != nullptr && size != 0);

		if (_type != DatumTypes::String)
		{
			if (_type != DatumTypes::Unknown)
			{
				throw std::runtime_error("Error, type is already set, incompatible!");
			}
			else
			{
				_type = DatumTypes::String;
			}
		}

		if (!isExternal)
		{
			Clear();
			free(_data.vp);
		}

		isExternal = true;
		_data.s = stringArray;
		_size = _capacity = size;
	}
	void Datum::SetStorage(RTTI** pointerArray, size_t size)
	{
		assert(pointerArray != nullptr && size != 0);

		if (_type != DatumTypes::Pointer)
		{
			if (_type != DatumTypes::Unknown)
			{
				throw std::runtime_error("Error, type is already set, incompatible!");
			}
			else
			{
				_type = DatumTypes::Pointer;
			}
		}

		if (!isExternal && _capacity > 0)
		{
			Clear();
			free(_data.vp);
		}

		isExternal = true;
		_data.r = pointerArray;
		_size = _capacity = size;
	}

	void Datum::SetStorage(Scope** pointerArray, size_t size)
	{
		assert(pointerArray != nullptr && size != 0);

		if (_type != DatumTypes::Table)
		{
			if (_type != DatumTypes::Unknown)
			{
				throw std::runtime_error("Error, type is already set, incompatible!");
			}
			else
			{
				_type = DatumTypes::Table;
			}
		}

		if (!isExternal && _capacity > 0)
		{
			Clear();
			free(_data.vp);
		}

		isExternal = true;
		_data.t = pointerArray;
		_size = _capacity = size;
	}

	void Datum::SetStorage(DatumTypes type, void* data, size_t size)
	{
		assert(data != nullptr && size != 0);

		if (type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Error! Cannot set the storage type to Unknown!");
		}

		if (_type != DatumTypes::Unknown && _type != type)
		{
			throw std::runtime_error("Error! Cannot set the storage for already initialized Datum!");
		}

		if (!isExternal && _capacity > 0)
		{
			Clear();
			free(_data.vp);
		}

		isExternal = true;
		_type = type;
		_data.vp = data;
		_size = size;
	}

	void Datum::SetFromString(const std::string& inputString, size_t index)
	{
		if (_type == DatumTypes::Unknown || _type == DatumTypes::Pointer)
		{
			throw std::runtime_error("Error, type is Unknown or Pointer, cannot Set data!");
		}
		
		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum uninitialized, cannot Set data!");
		}

		int scanResult = 0;
		int intValue;
		float floatValue;
		switch (_type)
		{
		case DatumTypes::Integer:

		{
			intValue = std::stoi(inputString);
			Set(intValue, index);
			break;
		}

		case DatumTypes::Float:
		{
			floatValue = std::stof(inputString);
			Set(floatValue, index);
			break;
		}

		case DatumTypes::Vector:
		{
			glm::vec4 vecValue;
			scanResult = sscanf_s(inputString.c_str(), "%f %f %f %f", &vecValue.x, &vecValue.y, &vecValue.z, &vecValue.w);
			assert(scanResult == 4);
			Set(vecValue, index);
			break;
		}

		case DatumTypes::Matrix:
		{
			glm::mat4 matValue;

			scanResult = sscanf_s(inputString.c_str(), "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
				&matValue[0].x, &matValue[0].y, &matValue[0].z, &matValue[0].w, &matValue[1].x, &matValue[1].y, &matValue[1].z, &matValue[1].w,
				&matValue[2].x, &matValue[2].y, &matValue[2].z, &matValue[2].w, &matValue[3].x, &matValue[3].y, &matValue[3].z, &matValue[3].w);
			assert(scanResult == 16);
			Set(matValue, index);
			break;
		}

		case DatumTypes::String:
		{
			Set(inputString, index);
			break;
		}
		}
	}

	void Datum::PushBackFromString(const std::string& inputString)
	{
		if (_type == DatumTypes::Unknown || _type == DatumTypes::Pointer)
		{
			throw std::runtime_error("Error, type is Unknown or Pointer, cannot Set data!");
		}

		int scanResult = 0;
		int intValue;
		float floatValue;
		switch (_type)
		{
		case DatumTypes::Integer:

		{
			intValue = std::stoi(inputString);
			PushBack(intValue);
			break;
		}

		case DatumTypes::Float:
		{
			floatValue = std::stof(inputString);
			PushBack(floatValue);
			break;
		}

		case DatumTypes::Vector:
		{
			glm::vec4 vecValue;
			scanResult = sscanf_s(inputString.c_str(), "%f %f %f %f", &vecValue.x, &vecValue.y, &vecValue.z, &vecValue.w);
			assert(scanResult == 4);
			PushBack(vecValue);
			break;
		}

		case DatumTypes::Matrix:
		{
			glm::mat4 matValue;

			scanResult = sscanf_s(inputString.c_str(), "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
				&matValue[0].x, &matValue[0].y, &matValue[0].z, &matValue[0].w, &matValue[1].x, &matValue[1].y, &matValue[1].z, &matValue[1].w,
				&matValue[2].x, &matValue[2].y, &matValue[2].z, &matValue[2].w, &matValue[3].x, &matValue[3].y, &matValue[3].z, &matValue[3].w);
			assert(scanResult == 16);
			PushBack(matValue);
			break;
		}

		case DatumTypes::String:
		{
			PushBack(inputString);
			break;
		}
		}
	}

	void Datum::PushBack(int i)
	{
		if (isExternal && _capacity > 0)
		{
			throw std::runtime_error("Error, store set to External, operation not allowed.");
		}

		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Integer;
		}
		else if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Error, type does not match!");
		}

		if (_size == _capacity)
		{
			const size_t DATUM_GROWTH_FACTOR = 2;
			Reserve((_capacity * DATUM_GROWTH_FACTOR) + 1);
		}

		new(_data.i + _size)int(i);
		++_size;
	}

	void Datum::PushBack(float f)
	{
		if (isExternal && _capacity > 0)
		{
			throw std::runtime_error("Error, store set to External, operation not allowed.");
		}

		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Float;
		}
		else if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Error, type does not match!");
		}

		if (_size == _capacity)
		{
			const size_t DATUM_GROWTH_FACTOR = 2;
			Reserve((_capacity * DATUM_GROWTH_FACTOR) + 1);
		}

		new(_data.f + _size)float(f);
		++_size;
	}

	void Datum::PushBack(const glm::vec4& v)
	{
		if (isExternal && _capacity > 0)
		{
			throw std::runtime_error("Error, store set to External, operation not allowed.");
		}

		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Vector;
		}
		else if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Error, type does not match!");
		}

		if (_size == _capacity)
		{
			const size_t DATUM_GROWTH_FACTOR = 2;
			Reserve((_capacity * DATUM_GROWTH_FACTOR) + 1);
		}

		new(_data.v + _size)glm::vec4(v);
		++_size;
	}

	void Datum::PushBack(const glm::mat4& m)
	{
		if (isExternal && _capacity > 0)
		{
			throw std::runtime_error("Error, store set to External, operation not allowed.");
		}

		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Matrix;
		}
		else if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Error, type does not match!");
		}

		if (_size == _capacity)
		{
			const size_t DATUM_GROWTH_FACTOR = 2;
			Reserve((_capacity * DATUM_GROWTH_FACTOR) + 1);
		}

		new(_data.m + _size)glm::mat4(m);
		++_size;
	}

	void Datum::PushBack(const std::string& s)
	{
		if (isExternal && _capacity > 0)
		{
			throw std::runtime_error("Error, store set to External, operation not allowed.");
		}

		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::String;
		}
		else if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Error, type does not match!");
		}

		if (_size == _capacity)
		{
			const size_t DATUM_GROWTH_FACTOR = 2;
			Reserve((_capacity * DATUM_GROWTH_FACTOR) + 1);
		}

		new(_data.s + _size)std::string(s);
		++_size;
	}

	void Datum::PushBack(Scope& t)
	{
		if (isExternal && _capacity > 0)
		{
			throw std::runtime_error("Error, store set to External, operation not allowed.");
		}

		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Table;
		}
		else if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Error, type does not match!");
		}

		if (_size == _capacity)
		{
			const size_t DATUM_GROWTH_FACTOR = 2;
			Reserve((_capacity * DATUM_GROWTH_FACTOR) + 1);
		}
		*(_data.t + _size) = &t;
		++_size;
	}

	void Datum::PushBack(RTTI* r)
	{
		if (isExternal && _capacity > 0)
		{
			throw std::runtime_error("Error, store set to External, operation not allowed.");
		}

		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Pointer;
		}
		else if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Error, type does not match!");
		}

		if (_size == _capacity)
		{
			const size_t DATUM_GROWTH_FACTOR = 2;
			Reserve((_capacity * DATUM_GROWTH_FACTOR) + 1);
		}
		*(_data.r + _size) = r;
		++_size;
	}

	void Datum::PopBack()
	{
		if (isExternal)
		{
			throw std::runtime_error("Error, store set to External, operation not allowed.");

		}
		if (_size == 0)
		{
			return;
		}

		if (_type == DatumTypes::String)
		{
			(_data.s + (_size - 1))->~basic_string();
		}
		--_size;
	}

	int& Datum::IntFront()
	{
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.i[0];
	}

	int Datum::IntFront() const
	{
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.i[0];
	}

	int& Datum::IntBack()
	{
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.i[_size - 1];
	}

	int Datum::IntBack() const
	{
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.i[_size - 1];
	}

	float& Datum::FloatFront()
	{
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.f[0];
	}

	float Datum::FloatFront() const
	{
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.f[0];
	}

	float& Datum::FloatBack()
	{
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.f[_size - 1];
	}

	float Datum::FloatBack() const
	{
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.f[_size - 1];
	}

	glm::vec4& Datum::VectorFront()
	{
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.v[0];
	}

	const glm::vec4& Datum::VectorFront() const
	{
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.v[0];
	}

	glm::vec4& Datum::VectorBack()
	{
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.v[_size - 1];
	}

	const glm::vec4& Datum::VectorBack() const
	{
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.v[_size - 1];
	}

	glm::mat4& Datum::MatrixFront()
	{
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.m[0];
	}

	const glm::mat4& Datum::MatrixFront() const
	{
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.m[0];
	}

	glm::mat4& Datum::MatrixBack()
	{
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.m[_size - 1];
	}

	const glm::mat4& Datum::MatrixBack() const
	{
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.m[_size - 1];
	}

	std::string& Datum::StringFront()
	{
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.s[0];
	}

	const std::string& Datum::StringFront() const
	{
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.s[0];
	}

	std::string& Datum::StringBack()
	{
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.s[_size - 1];
	}

	const std::string& Datum::StringBack() const
	{
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.s[_size - 1];
	}

	RTTI*& Datum::PointerFront()
	{
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.r[0];
	}

	RTTI* Datum::PointerFront() const
	{
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.r[0];
	}

	RTTI*& Datum::PointerBack()
	{
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.r[_size - 1];
	}

	RTTI* Datum::PointerBack() const
	{
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (_size == 0)
		{
			throw std::runtime_error("Error, Datum is empty!");
		}

		return _data.r[_size - 1];
	}

	void Datum::RemoveAt(size_t index)
	{
		/*if (isExternal)
		{
			throw std::runtime_error("Error, store set to External, operation not allowed.");
		}*/

		if (index >= _size)
		{
			throw std::runtime_error("Error, index is out of bounds!");
		}

		if (_type == DatumTypes::String)
		{
			(_data.s + index)->~basic_string();
		}

		size_t size = _typeSizes[static_cast<size_t>(_type)];
		auto data = reinterpret_cast<uint8_t*>(_data.vp);
		assert(data != nullptr);
		std::memmove((data + (index * size)), (data + ((index + 1) * size)), (size * (_size - index - 1)));
		--_size;
	}

	bool Datum::Remove(int value)
	{
		if (isExternal)
		{
			throw std::runtime_error("Error, store set to External, operation not allowed.");
		}

		std::pair<size_t, bool> findResult = Find(value);

		if (findResult.second)
		{
			RemoveAt(findResult.first);
			return true;
		}

		else
		{
			return false;
		}
	}

	bool Datum::Remove(float value)
	{
		if (isExternal)
		{
			throw std::runtime_error("Error, store set to External, operation not allowed.");
		}

		std::pair<size_t, bool> findResult = Find(value);

		if (findResult.second)
		{
			RemoveAt(findResult.first);
			return true;
		}

		else
		{
			return false;
		}
	}

	bool Datum::Remove(const glm::vec4& value)
	{
		if (isExternal)
		{
			throw std::runtime_error("Error, store set to External, operation not allowed.");
		}

		std::pair<size_t, bool> findResult = Find(value);

		if (findResult.second)
		{
			RemoveAt(findResult.first);
			return true;
		}

		else
		{
			return false;
		}
	}

	bool Datum::Remove(const glm::mat4& value)
	{
		if (isExternal)
		{
			throw std::runtime_error("Error, store set to External, operation not allowed.");
		}

		std::pair<size_t, bool> findResult = Find(value);

		if (findResult.second)
		{
			RemoveAt(findResult.first);
			return true;
		}

		else
		{
			return false;
		}
	}

	bool Datum::Remove(const std::string& value)
	{
		if (isExternal)
		{
			throw std::runtime_error("Error, store set to External, operation not allowed.");

		}
		std::pair<size_t, bool> findResult = Find(value);

		if (findResult.second)
		{
			RemoveAt(findResult.first);
			return true;
		}

		else
		{
			return false;
		}
	}

	bool Datum::Remove(RTTI* value)
	{
		if (isExternal)
		{
			throw std::runtime_error("Error, store set to External, operation not allowed.");

		}
		std::pair<size_t, bool> findResult = Find(value);

		if (findResult.second)
		{
			RemoveAt(findResult.first);
			return true;
		}

		else
		{
			return false;
		}
	}

	bool Datum::Remove(Scope& value)
	{
		size_t counter = 0;
		for (size_t i = 0; i < _size; ++i)
		{
			if (_data.t[i] == &value)
			{
				counter = i;
				break;
			}
		}

		if (counter < _size)
		{
			size_t size = _typeSizes[static_cast<size_t>(DatumTypes::Table)];
			auto data = reinterpret_cast<uint8_t*>(_data.vp);
			assert(data != nullptr);
			std::memmove((data + (counter * size)), (data + ((counter + 1) * size)), (size * (_size - counter - 1)));
			--_size;
			return true;
		}

		else
		{
			return false;
		}
	}
	std::pair<size_t, bool> Datum::Find(int value) const
	{
		if (_size == 0 || _type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Error, Datum not valid!");
		}

		size_t counter = 0;
		bool found = false;
		for (counter; counter < _size; ++counter)
		{
			if (_data.i[counter] == value)
			{
				found = true;
				break;
			}
		}

		return std::make_pair(counter, found);
	}

	std::pair<size_t, bool> Datum::Find(float value) const
	{
		if (_size == 0 || _type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Error, Datum not valid!");
		}

		size_t counter = 0;
		bool found = false;
		for (counter; counter < _size; ++counter)
		{
			if (_data.f[counter] == value)
			{
				found = true;
				break;
			}
		}

		return std::make_pair(counter, found);
	}

	std::pair<size_t, bool> Datum::Find(const glm::vec4& value) const
	{
		if (_size == 0 || _type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Error, Datum not valid!");
		}

		size_t counter = 0;
		bool found = false;
		for (counter; counter < _size; ++counter)
		{
			if (_data.v[counter] == value)
			{
				found = true;
				break;
			}
		}

		return std::make_pair(counter, found);
	}

	std::pair<size_t, bool> Datum::Find(const glm::mat4& value) const
	{
		if (_size == 0 || _type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Error, Datum not valid!");
		}

		size_t counter = 0;
		bool found = false;
		for (counter; counter < _size; ++counter)
		{
			if (_data.m[counter] == value)
			{
				found = true;
				break;
			}
		}

		return std::make_pair(counter, found);
	}

	std::pair<size_t, bool> Datum::Find(const std::string& value) const
	{
		if (_type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Error, Datum not valid!");
		}

		size_t counter = 0;
		bool found = false;
		for (counter; counter < _size; ++counter)
		{
			if (_data.s[counter] == value)
			{
				found = true;
				break;
			}
		}

		return std::make_pair(counter, found);
	}

	std::pair<size_t, bool> Datum::Find(RTTI* value) const
	{
		if (_size == 0 || _type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Error, Datum not valid!");
		}

		size_t counter = 0;
		bool found = false;
		for (counter; counter < _size; ++counter)
		{
			if (_data.r[counter] == value)
			{
				found = true;
				break;
			}
		}

		return std::make_pair(counter, found);
	}
	
	std::pair<size_t, bool> Datum::Find(const Scope& value) const
	{
		if (_type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Error, Datum not valid!");
		}

		size_t counter = 0;
		bool found = false;
		for (counter; counter < _size; ++counter)
		{
			if ((_data.t[counter]) == &value)
			{
				found = true;
				break;
			}
		}

		return std::make_pair(counter, found);
	}

	const int& Datum::GetInt(size_t index) const
	{
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (index >= _size)
		{
			throw std::runtime_error("Error, index out of bounds!");
		}

		return _data.i[index];
	}

	const float& Datum::GetFloat(size_t index) const
	{
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (index >= _size)
		{
			throw std::runtime_error("Error, index out of bounds!");
		}

		return _data.f[index];
	}

	const glm::vec4& Datum::GetVector(size_t index) const
	{
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (index >= _size)
		{
			throw std::runtime_error("Error, index out of bounds!");
		}

		return _data.v[index];
	}

	const glm::mat4& Datum::GetMatrix(size_t index) const
	{
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (index >= _size)
		{
			throw std::runtime_error("Error, index out of bounds!");
		}

		return _data.m[index];
	}

	const std::string& Datum::GetString(size_t index) const
	{
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (index >= _size)
		{
			throw std::runtime_error("Error, index out of bounds!");
		}

		return _data.s[index];
	}

	Scope& Datum::GetTable(size_t index) const
	{
		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (index >= _size)
		{
			throw std::runtime_error("Error, index out of bounds!");
		}

		return *(_data.t[index]);
	}

	RTTI* Datum::GetPointer(size_t index) const
	{
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Error, type is incorrect!");
		}

		if (index >= _size)
		{
			throw std::runtime_error("Error, index out of bounds!");
		}

		return _data.r[index];
	}

	void Datum::Reserve(size_t capacity)
	{
		if (isExternal)
		{
			throw std::runtime_error("Error, storage set to External, operation not allowed.");
		}

		if (capacity > _capacity)
		{
			size_t size = _typeSizes[static_cast<size_t>(_type)];
			assert(size != 0);
			void* data = realloc(_data.vp, capacity * size);
			assert(data != nullptr);
			_data.vp = data;
			_capacity = capacity;
		}
	}

	void Datum::Resize(size_t size)
	{
		if (isExternal)
		{
			throw std::runtime_error("Error, storage set to External, operation not allowed.");
		}

		if (size < _size)
		{
			if (_type == DatumTypes::String)
			{
				while (_size != size)
				{
					_data.s[_size - 1].~basic_string();
					--_size;
				}
				
			}
			ShrinkToFit();
			_capacity = size;
		}

		else
		{
			Reserve(size);

			if (_type == DatumTypes::String)
			{
				for (size_t counter = _size; counter < size; ++counter)
				{
					new(_data.s + counter)std::string();
				}
			}
		}

		_size = size;
		_capacity = size;
	}

	void Datum::ShrinkToFit()
	{
		if (isExternal)
		{
			throw std::runtime_error("Error, storage set to External, operation not allowed!");
		}

		size_t size = _typeSizes[static_cast<size_t>(_type)];

		if (_size < _capacity)
		{
			auto temp = realloc(_data.vp, size * _size);
			assert(temp != nullptr);
			_data.vp = temp;
			_capacity = _size;
		}
	}

	void Datum::Clear()
	{
		if (isExternal)
		{
			_size = 0;
			_capacity = 0;
			_data.vp = nullptr;
		}

		else
		{
			while (_size != 0)
			{
				PopBack();
			}	
			_size = 0;	
		}
	}

	size_t Datum::Size() const
	{
		return _size;
	}

	size_t Datum::Capacity() const
	{
		return _capacity;
	}

	std::string Datum::ToString(size_t index) const
	{
		switch (_type)
		{
		case DatumTypes::Integer:
			return std::to_string(GetInt(index));
			break;

		case DatumTypes::Float:
			return std::to_string(GetFloat(index));
			break;

		case DatumTypes::Vector:
			return glm::to_string(GetVector(index));
			break;

		case DatumTypes::Matrix:
			return glm::to_string(GetMatrix(index));
			break;

		case DatumTypes::String:
			return GetString(index);
			break;

		case DatumTypes::Pointer:
			return GetPointer(index)->ToString();
			break;

		default:
			throw std::runtime_error("Error, type is unknown or undefined!");
			break;
		}
	}

	bool Datum::IsExternal() const
	{
		return isExternal;
	}

	void Datum::SetType(DatumTypes type)
	{
		if (_type == DatumTypes::Unknown && _type != DatumTypes::Unknown)
		{
			throw std::runtime_error("Error, cannot assign Unknown to a Datum that already has a type!");
		}

		if (_type != DatumTypes::Unknown && type != _type)
		{
			throw std::runtime_error("Error, a type is already assigned to this Datum!");
		}
		_type = type;
	}

	DatumTypes Datum::Type() const
	{
		return _type;
	}

}