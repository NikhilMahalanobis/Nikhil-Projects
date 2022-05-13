#include "Signature.h"

namespace FieaGameEngine
{
	Signature::Signature(const std::string& name, DatumTypes type, size_t numElements, size_t offset) :
		_name(name), _type(type), _numElements(numElements), _offset(offset)
	{
	}
}