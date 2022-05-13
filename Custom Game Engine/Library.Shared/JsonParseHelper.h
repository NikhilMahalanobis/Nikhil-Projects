#pragma once

#include <memory>
#include <istream>
#include <fstream>
#include <sstream>
#pragma warning (push)
#pragma warning (disable : 26812)
#include <json/json.h>
#pragma warning (pop)
#include"RTTI.h"
#include"Vector.h"

namespace FieaGameEngine
{
	class JsonParseCoordinator;

	class JsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(JsonParseHelper, RTTI)
	public: 


		class SharedData : public RTTI
		{
			friend JsonParseCoordinator;

			RTTI_DECLARATIONS(SharedData, RTTI)

		public:
			/// <summary>
			/// Compiler Supplied default Constructor for SharedData
			/// </summary>
			SharedData() = default;

			/// <summary>
			/// Virtual Compiler Supplied default Destructor for SharedData
			/// </summary>
			/// <returns></returns>
			virtual ~SharedData() override = default;

			/// <summary>
			/// Pure Virtual Create method for SharedData, used as Virtual Constructor
			/// </summary>
			/// <returns> Returns a shared_ptr of type SharedData </returns>
			virtual std::shared_ptr<SharedData> Create() const = 0;

			/// <summary>
			/// Getter method for SharedData, for getting the owning JsonParseCoordinator
			/// </summary>
			/// <returns> Returns a pointer to a JsonParseCoordinator </returns>
			JsonParseCoordinator* GetJsonParseCoordinator() const;

			/// <summary>
			/// IncrementDepth method for SharedData
			/// </summary>
			void IncrementDepth();

			/// <summary>
			/// DecrementDepth method for SharedData
			/// </summary>
			void DecrementDepth();

			/// <summary>
			/// Depth getter method for SharedData
			/// </summary>
			/// <returns> Returns a copy of a size_t </returns>
			size_t Depth() const;
		private:
			/// <summary>
			/// JsonParseCoordinator setter method for SharedData, sets the owning ParseCoordinator
			/// </summary>
			/// <param name="owner"> Takes a reference to a JsonParseCoordinator </param>
			void SetJsonParseCoordinator(JsonParseCoordinator& owner);

			JsonParseCoordinator* _owningParseCoordinator{ nullptr };
			size_t _nestingDepth{ 0 };
		};

		/// <summary>
		/// Compiler Supplied Default Constructor for JsonParseHelper
		/// </summary>
		JsonParseHelper() = default;

		/// <summary>
		/// Virtual Compiler Supplied Default Destructor for JsonParseHelper
		/// </summary>
		virtual ~JsonParseHelper() = default;

		/// <summary>
		/// Initialize Method for JsonParseHelper
		/// </summary>
		virtual void Initialize() {};

		/// <summary>
		/// Pure Virtual StartHandler method for JsonParseHelper
		/// </summary>
		/// <param name="sharedData"> Takes a reference to a SharedData </param>
		/// <param name="key"> Takes a const reference to a std::string </param>
		/// <param name="input"> Takes a const reference to a Json::Value object</param>
		/// <param name="isArray"> Takes a copy of a bool </param>
		/// <returns> Returns a copy of a bool </returns>
		virtual bool StartHandler(SharedData& sharedData, const std::string& key, const Json::Value& input, bool isArray = false , size_t index = 0) = 0;

		/// <summary>
		/// Pure Virtual EndHandler method for JsonParseHelper
		/// </summary>
		/// <param name="sharedData"> Takes a reference to a SharedData </param>
		/// <param name="key"> Takes a const reference to a std::string </param>
		/// <param name="input"> Takes a const reference to a Json::Value object </param>
		/// <returns> Returns a copy of a bool </returns>
		virtual bool EndHandler(SharedData& sharedData, const std::string& key, const Json::Value& input) = 0;

		/// <summary>
		/// Pure Virtual Create method for JsonParseHelper, used as Virtual Constructor
		/// </summary>
		/// <returns> Returns a shared_ptr of type JsonParseHelper </returns>
		virtual std::shared_ptr<JsonParseHelper> Create() const = 0;
	};
}

