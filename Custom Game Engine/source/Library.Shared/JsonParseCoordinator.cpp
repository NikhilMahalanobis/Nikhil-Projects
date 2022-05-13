#include "JsonParseCoordinator.h"

namespace FieaGameEngine
{
	JsonParseCoordinator::JsonParseCoordinator(std::shared_ptr<JsonParseHelper::SharedData> input)
	{
		SetSharedData(input);
	}

	JsonParseCoordinator::JsonParseCoordinator(JsonParseCoordinator&& other) noexcept :
		_helpers(other._helpers), _sharedData(other._sharedData)
	{
		_sharedData->SetJsonParseCoordinator(*this);
		other._sharedData.reset();
	}

	JsonParseCoordinator& JsonParseCoordinator::operator=(JsonParseCoordinator&& other) noexcept
	{
		_sharedData = std::move(other._sharedData);
		_helpers = std::move(other._helpers);

		_sharedData->SetJsonParseCoordinator(*this);
		other._sharedData.reset();

		return *this;
	}

	std::unique_ptr<JsonParseCoordinator> JsonParseCoordinator::Clone() const
	{
		JsonParseCoordinator* copy = new JsonParseCoordinator();

		if (_sharedData != nullptr)
		{
			copy->SetSharedData(_sharedData->Create());
		}

		for (auto& handler : _helpers)
		{
			copy->AddHelper(handler->Create());
		}

		return std::unique_ptr<JsonParseCoordinator>(copy);
	}

	void JsonParseCoordinator::AddHelper(const std::shared_ptr<JsonParseHelper>& helper)
	{
		auto it = _helpers.Find(helper);
		if (it != _helpers.end())
		{
			throw std::runtime_error("Error, Helper already exists in this Coordinator!");
		}
		_helpers.PushBack(helper);
	}

	void JsonParseCoordinator::RemoveHelper(const std::shared_ptr<JsonParseHelper>& helper)
	{
		auto it = _helpers.Find(helper);
		_helpers.Remove(it);
	}

	void JsonParseCoordinator::DeserializeObject(const std::string& input)
	{
		std::istringstream iStream(input);
		DeserializeObject(iStream);
	}

	void JsonParseCoordinator::DeserializeObjectFromFile(const std::string& fileName)
	{
		std::ifstream file;
		file.open(fileName.c_str());

		if (file.is_open())
		{
			DeserializeObject(file);
			file.close();
		}

		else
		{
			throw std::runtime_error("Error, given file name could not be opened!");
		}
	}

	void JsonParseCoordinator::DeserializeObject(std::istream& stream)
	{	
		for (auto& handler : _helpers)
		{
			handler->Initialize();
		}

		if (_sharedData == nullptr || _helpers.Size() == 0)
		{
			throw std::runtime_error("Error, ParseCoordinator not properly initialized to parse any data.");
		}

		Json::Value inputStream;
		stream >> inputStream;
		
		_sharedData->IncrementDepth();
		ParseMembers(inputStream);
		_sharedData->DecrementDepth();
	}

	std::shared_ptr<JsonParseHelper::SharedData> JsonParseCoordinator::GetSharedData()
	{
		return _sharedData;
	}

	void JsonParseCoordinator::SetSharedData(std::shared_ptr<JsonParseHelper::SharedData> newSharedData)
	{
		_sharedData = std::move(newSharedData);
		_sharedData->SetJsonParseCoordinator(*this);
	}
	const Vector<std::shared_ptr<JsonParseHelper>>& JsonParseCoordinator::Helpers() const
	{
		return _helpers;
	}
	void JsonParseCoordinator::ParseMembers(Json::Value& input, bool isArray)
	{
		for (const auto& value : input.getMemberNames())
		{
			Parse(value, input[value], isArray);
		}
	}

	void JsonParseCoordinator::Parse(const std::string& key, Json::Value& value, bool isArray, size_t index)
	{
		
			// Object
			if (value.isObject())
			{
				for (auto& handler : _helpers)
				{

					if (handler->StartHandler(*_sharedData, key, value, isArray, index))
					{
						_sharedData->IncrementDepth();
						ParseMembers(value);
						handler->EndHandler(*_sharedData, key, value);
						_sharedData->DecrementDepth();
						break;
					}
				}
			}
			
			// Array
			else if (value.isArray())
			{	
				size_t counter = 0;
				for (auto& member : value)
				{
					if (member.isObject())
					{
						_sharedData->IncrementDepth();

						for (const auto& arrayValue : member.getMemberNames())
						{
							Parse(arrayValue, member[arrayValue], true, counter);
						}
						_sharedData->DecrementDepth();
					}
					else
					{
						Parse(key, member, true, counter);
					}

					++counter;
				}

			}

			// Primitive
			else
			{
				for (auto& handler : _helpers)
				{
					if (handler->StartHandler(*_sharedData, key, value, isArray, index))
					{
						handler->EndHandler(*_sharedData, key, value);
						break;
					}
				}
			}
	}
}