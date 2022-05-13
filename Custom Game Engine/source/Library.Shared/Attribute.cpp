#include "Attribute.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attribute)
	
	Attribute::Attribute(RTTI::IdType typeId)
	{
		Populate(TypeRegistry::getInstance()->Find(typeId));
	}

	Attribute::Attribute(const Attribute& rhs) :
		Scope::Scope(rhs), numPrescribedAttributes(rhs.numPrescribedAttributes)
	{
		(*this)["this"] = this;
		UpdateExternalStorage(TypeRegistry::getInstance()->Find(rhs.TypeIdInstance()));
	}

	Attribute& Attribute::operator=(const Attribute& rhs)
	{
		Scope::operator=(rhs);
		numPrescribedAttributes = rhs.numPrescribedAttributes;
		(*this)["this"] = this;
		UpdateExternalStorage(TypeRegistry::getInstance()->Find(rhs.TypeIdInstance()));

		return *this;
	}

	Attribute::Attribute(Attribute&& rhs) noexcept :
		Scope::Scope(std::move(rhs)), numPrescribedAttributes(std::move(rhs.numPrescribedAttributes))
	{
		(*this)["this"] = this;
		UpdateExternalStorage(TypeRegistry::getInstance()->Find(rhs.TypeIdInstance()));
	}

	Attribute& Attribute::operator=(Attribute&& rhs) noexcept
	{
		Scope::operator=(std::move(rhs));
		numPrescribedAttributes = std::move(rhs.numPrescribedAttributes);
		(*this)["this"] = this;
		UpdateExternalStorage(TypeRegistry::getInstance()->Find(rhs.TypeIdInstance()));

		return *this;
	}

	bool Attribute::IsAttribute(const std::string& name)
	{
		auto foundResult = _scopeMap.Find(name);
		
		if (foundResult != _scopeMap.end())
		{
			return true;
		}
		return false;
	}

	bool Attribute::IsPrescribedAttribute(const std::string& name)
	{
		if (!IsAttribute(name))
		{
			return false;
		}

		size_t counter = 0;
		for (auto it = _orderArray.begin(); counter < numPrescribedAttributes; ++counter, ++it)
		{
			if ((*it)->first == name)
			{
				return true;
			}
		}

		return false;
	}

	bool Attribute::IsAuxiliaryAttribute(const std::string& name)
	{
		return (IsAttribute(name) && !IsPrescribedAttribute(name));
	}

	Datum& Attribute::AppendAuxiliaryAttribute(const std::string& name)
	{
		if (IsPrescribedAttribute(name))
		{
			throw std::runtime_error("Error, this attribute is already a Prescribed Attribute!");
		}

		return Append(name);
	}

	void Attribute::UpdateExternalStorage(HashMap<RTTI::IdType, Vector<Signature>>::PairType& typeInfo)
	{
		uint8_t* thisPtr = reinterpret_cast<uint8_t*>(this);
		for (auto value : typeInfo.second)
		{
			if (value._type != DatumTypes::Table)
			{
				Datum* newDatum = Find(value._name);
				assert(newDatum != nullptr);
				newDatum->SetStorage(value._type, (thisPtr + value._offset), value._numElements);
			}
		}
	}

	Datum& Attribute::At(const std::string name)
	{
		return _scopeMap.At(name);
	}

	void Attribute::Populate(HashMap<RTTI::IdType, Vector<Signature>>::PairType& typeInfo)
	{
		(*this)["this"] = this;	

		auto& signatures = typeInfo.second;

		for (auto& value : signatures)
		{
			Datum& datum = Append(value._name);
			if (value._type == DatumTypes::Table)
			{
				datum.SetType(DatumTypes::Table);
				if (value._numElements > 0)
				{
					for (size_t i = 0; i < value._numElements; ++i)
					{
						AppendScope(value._name);
					}
				}
			}
			else
			{
				datum.SetType(value._type);
				uint8_t* thisPtr = reinterpret_cast<uint8_t*>(this);
				datum.SetStorage(value._type, (thisPtr + value._offset), value._numElements);
			}
		}

		numPrescribedAttributes = signatures.Size() + 1;
	}


}