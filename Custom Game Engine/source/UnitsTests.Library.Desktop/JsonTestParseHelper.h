#pragma once

#include <cstdint>
#include "JsonParseHelper.h"
#include "JsonParseCoordinator.h"

namespace UnitsTestsLibraryDesktop
{
	class JsonTestParseHelper final : public FieaGameEngine::JsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTestParseHelper, FieaGameEngine::JsonParseHelper)

	public:
		class SharedData final : public FieaGameEngine::JsonParseHelper::SharedData
		{
			RTTI_DECLARATIONS(SharedData, FieaGameEngine::JsonParseHelper::SharedData)

		public:
			virtual std::shared_ptr<FieaGameEngine::JsonParseHelper::SharedData> Create() const override { return CreateSmart(); };
			virtual std::shared_ptr<SharedData> CreateSmart() const;
			std::size_t MaxDepth = 0;

			FieaGameEngine::Vector<std::int32_t> Data;
		};

		std::shared_ptr<FieaGameEngine::JsonParseHelper> Create() const override { return CreateSmart(); };
		std::shared_ptr<JsonTestParseHelper> CreateSmart() const;
		void Initialize() override;
		bool StartHandler(JsonParseHelper::SharedData& sharedData, const std::string& key, const Json::Value& input, bool isArray, size_t index) override ;
		bool EndHandler(JsonParseHelper::SharedData& sharedData, const std::string& key, const Json::Value& input) override;

		std::uint32_t StartHandlerCount = 0;
		std::uint32_t EndHandlerCount = 0;
		std::uint32_t ArrayElementCount = 0;
		bool InializeCalled = false;

	private:
		bool _parsingData{ false };
		inline static const std::string IntegerKey = "integer";

	};
}