#pragma once

#include "Scope.h"
#include "JsonParseHelper.h"
#include "Stack.h"
#include "Factory.h"
#include "GameObject.h"

namespace FieaGameEngine
{
	class JsonTableParseHelper : public JsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, JsonParseHelper)

	public:
		class SharedData final : public JsonParseHelper::SharedData
		{
			RTTI_DECLARATIONS(SharedData, JsonParseHelper::SharedData)

		public:
			/// <summary>
			/// Compiler Supplied Defaulted Constructor for SharedData
			/// </summary>
			SharedData() = default;

			/// <summary>
			/// Typecast Constructor for SharedData
			/// </summary>
			/// <param name="parentScope"> Takes a reference to a Scope </param>
			/// <returns> Returns a reference to a SharedData </returns>
			SharedData(Scope& parentScope);

			/// <summary>
			/// Compiler supplied Defaulted Destructor for SharedData
			/// </summary>
			~SharedData() = default;

			/// <summary>
			/// Overidden Create method for SharedData, used as a wrapper to call CreateSmart due to lack of covariance w/ Smart Pointes
			/// </summary>
			virtual std::shared_ptr<JsonParseHelper::SharedData> Create() const override { return CreateSmart(); };

			/// <summary>
			/// Virtual CreateSmart method, used to create derived instance of SharedData
			/// </summary>
			/// <returns> Returns a shared_ptr of the Derived SharedData </returns>
			virtual std::shared_ptr<SharedData> CreateSmart() const;
			std::size_t MaxDepth = 0;

			Scope* _parentScope{ nullptr };
		};

		struct StackFrame
		{
			/// <summary>
			/// Constructor for StackFrame struct
			/// </summary>
			/// <param name="scopeContext"> Takes a pointer to a Scope </param>
			/// <param name="key"> Takes a const reference to a std::string </param>
			/// <param name="type"> Takes a copy of a DatumType enum class </param>
			StackFrame(Scope* scopeContext, const std::string& key, DatumTypes type, const std::string& className);

			const std::string& _key;
			std::string _className{ std::string() };
			DatumTypes	_type{ DatumTypes::Unknown };
			Scope* _scopeContext;
		};

		/// <summary>
		/// Compiler supplied Defaulted Constructor for JsonTableParseHelper
		/// </summary>
		JsonTableParseHelper() = default;

		/// <summary>
		/// Compiler supplied Defaulted Destructor for JsonTableParseHelper
		/// </summary>
		~JsonTableParseHelper() = default;

		/// <summary>
		/// Overridden Create method for JsonTableParseHelper, used as wrapper to call CreateSmart due to lack of covariance w/ Smart Pointers
		/// </summary>
		std::shared_ptr<JsonParseHelper> Create() const override { return CreateSmart(); };

		/// <summary>
		/// Virtual CreateSmart method for JsonTableParseHelper, used to create Derived instance of JsonParseHelper
		/// </summary>
		std::shared_ptr<JsonTableParseHelper> CreateSmart() const;

		/// <summary>
		/// Overridden Initialize method for JsonTableParseHelper
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// Overridden StartHandler method for JsonTableParseHelper
		/// </summary>
		/// <param name="sharedData"> Takes a reference to a SharedData </param>
		/// <param name="key"> Takes a const reference to a std::string </param>
		/// <param name="input"> Takes a const reference to a Json::Value object</param>
		/// <param name="isArray"> Takes a copy of a bool </param>
		/// <returns> Returns a copy of a bool </returns>
		bool StartHandler(JsonParseHelper::SharedData& sharedData, const std::string& key, const Json::Value& input, bool isArray = false, size_t index = 0) override;

		/// <summary>
		/// Overridden EndHandler method for JsonTableParseHelper
		/// </summary>
		/// <param name="sharedData"> Takes a reference to a SharedData </param>
		/// <param name="key"> Takes a const reference to a std::string </param>
		/// <param name="input"> Takes a const reference to a Json::Value object </param>
		/// <returns> Returns a copy of a bool </returns>
		bool EndHandler(JsonParseHelper::SharedData& sharedData, const std::string& key, const Json::Value& input) override;

		std::uint32_t StartHandlerCount = 0;
		std::uint32_t EndHandlerCount = 0;
		std::uint32_t ArrayElementCount = 0;
		bool InitializeCalled = false;

	private:
		Stack<StackFrame> _callStack;
		inline static const std::string TypeKey = "type";
		inline static const std::string ValueKey = "value";
		inline static const std::string TableKey = "table";
		inline static const std::string ClassKey = "class";

		static const HashMap<const std::string, DatumTypes> _datumTypeNameMap;
	};
}