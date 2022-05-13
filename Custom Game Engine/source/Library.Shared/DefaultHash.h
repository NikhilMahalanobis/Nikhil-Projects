#pragma once
#include <cstdint>
#include <cstdlib>
#include <string>

namespace FieaGameEngine
{
	size_t AdditiveHash(const uint8_t* data, size_t size);

	template<typename TKey>
	struct DefaultHash
	{
		size_t operator()(const TKey& key);
	};

	template<>
	struct DefaultHash<std::string>
	{
		size_t operator()(const std::string& value);
	};

	template<>
	struct DefaultHash<std::string*>
	{
		size_t operator()(const std::string* value);
	};

	template<>
	struct DefaultHash<const std::string>
	{
		size_t operator()(const std::string& value);
	};

	template<>
	struct DefaultHash<std::wstring>
	{
		size_t operator()(const std::wstring& value);
	};


	template<>
	struct DefaultHash<std::wstring*>
	{
		size_t operator()(const std::wstring* value);
	};

	template<>
	struct DefaultHash<char*>
	{
		size_t operator()(const char* value);
	};
}

#include "DefualtHash.inl"