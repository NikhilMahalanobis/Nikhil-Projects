#include "DefaultHash.h"

namespace FieaGameEngine
{
	inline size_t FieaGameEngine::AdditiveHash(const uint8_t* data, size_t size)
	{
		size_t hash = 29;

		for (size_t i = 0; i < size; ++i)
		{
			hash += data[i];
		}

		return hash;
	}

	template<typename TKey>
	inline size_t DefaultHash<TKey>::operator()(const TKey& key)
	{
		return AdditiveHash(reinterpret_cast<const uint8_t*>(&key), sizeof(TKey));
	}

	inline size_t DefaultHash<std::string>::operator()(const std::string& value)
	{
		return AdditiveHash(reinterpret_cast<const uint8_t*>(value.c_str()), value.size());
	}

	inline size_t DefaultHash<const std::string>::operator()(const std::string& value)
	{
		return AdditiveHash(reinterpret_cast<const uint8_t*>(value.c_str()), value.size());
	}

	inline size_t DefaultHash<std::string*>::operator()(const std::string* value)
	{
		return AdditiveHash(reinterpret_cast<const uint8_t*>(value), value->size());
	}

	inline size_t DefaultHash<std::wstring>::operator()(const std::wstring& value)
	{
		return AdditiveHash(reinterpret_cast<const uint8_t*>(value.c_str()), value.size());
	}

	inline size_t DefaultHash<std::wstring*>::operator()(const std::wstring* value)
	{
		return AdditiveHash(reinterpret_cast<const uint8_t*>(value), value->size());
	}

	inline size_t DefaultHash<char*>::operator()(const char* value)
	{
		return AdditiveHash(reinterpret_cast<const uint8_t*>(value), strlen(value));
	}
}