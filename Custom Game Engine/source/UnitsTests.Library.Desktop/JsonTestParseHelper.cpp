#include "pch.h"
#include "JsonTestParseHelper.h"

namespace UnitsTestsLibraryDesktop
{
	RTTI_DEFINITIONS(JsonTestParseHelper)
	RTTI_DEFINITIONS(JsonTestParseHelper::SharedData)
		
	void JsonTestParseHelper::Initialize()
	{
		JsonParseHelper::Initialize();
		InializeCalled = true;
		StartHandlerCount = 0;
		EndHandlerCount = 0;
	}

	bool JsonTestParseHelper::StartHandler(JsonParseHelper::SharedData& sharedData, const std::string& key, const Json::Value& input, bool isArray, size_t index)
	{

		index;
		isArray;

		JsonTestParseHelper::SharedData* customSharedData = sharedData.As<JsonTestParseHelper::SharedData>();

		if (customSharedData == nullptr)
		{
			return false;
		}

		if (key != IntegerKey)
		{
			return false;
		}
		if (_parsingData)
		{
			throw runtime_error("Error, already parsing an integer!");
		}
		
		_parsingData = true;

		customSharedData->Data.PushBack(input.asInt());

		return true;
	}

	bool JsonTestParseHelper::EndHandler(JsonParseHelper::SharedData& sharedData, const std::string& key, const Json::Value& input)
	{
		input;
		JsonTestParseHelper::SharedData* customSharedData = sharedData.As<JsonTestParseHelper::SharedData>();
		if (customSharedData == nullptr || key != IntegerKey || _parsingData == false)
		{
			return false;
		}

		_parsingData = false;

		if (customSharedData->MaxDepth < sharedData.Depth())
		{
			customSharedData->MaxDepth = sharedData.Depth();
		}
		return true;
	}

	std::shared_ptr<JsonTestParseHelper::SharedData> JsonTestParseHelper::SharedData::CreateSmart() const
	{
		return std::shared_ptr<SharedData>(new SharedData);
	}

	std::shared_ptr<JsonTestParseHelper> JsonTestParseHelper::CreateSmart() const
	{
		return std::shared_ptr<JsonTestParseHelper>(new JsonTestParseHelper);
	}
}

