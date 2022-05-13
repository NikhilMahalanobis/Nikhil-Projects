#pragma once

#include "JsonParseHelper.h"
namespace FieaGameEngine
{
	class JsonParseHelper;
	
	class JsonParseCoordinator final
	{
	public: 

		/// <summary>
		/// Compiler Supplied Defaulted Constructor for ParseCoordinator
		/// </summary>
		JsonParseCoordinator() = default;

		/// <summary>
		/// Typecast Constructor for ParseCoordinator
		/// </summary>
		/// <param name="input"> Takesa a shared_ptr of type JsonParseHelper::SharedData </param>
		JsonParseCoordinator(std::shared_ptr<JsonParseHelper::SharedData> input);

		/// <summary>
		/// Move Constructor for ParseCoordinator
		/// </summary>
		/// <param name="other"> Takes a rvalue reference to a JsonParseCoordinator </param>
		JsonParseCoordinator(JsonParseCoordinator&& other) noexcept;

		/// <summary>
		/// Move Assignment Operator for ParseCoordinator
		/// </summary>
		/// <param name="other"> Takes a rvalue reference to a JsonParseCoordinator </param>
		/// <returns> Returns a reference to a JsonParseCoordinator </returns>
		JsonParseCoordinator& operator=(JsonParseCoordinator&& other) noexcept;

		/// <summary>
		/// Compiler defaulted Destructor for ParseCoordinator
		/// </summary>
		~JsonParseCoordinator() = default;

		/// <summary>
		/// Clone method for ParseCoordinator, used as a Virtual Copy Constructor
		/// </summary>
		/// <returns> Returns a unique_ptr of type JsonParseCoordinator </returns>
		std::unique_ptr<JsonParseCoordinator> Clone() const;

		/// <summary>
		/// AddHelper method for ParseCoordinator 
		/// </summary>
		/// <param name="helper"> Takes a const reference to a shared_ptr of type JsonParseHelper </param>
		void AddHelper(const std::shared_ptr<JsonParseHelper>& helper);

		/// <summary>
		/// RemoveHelper method for ParseCoordinator 
		/// </summary>
		/// <param name="helper"> Takes a const reference to a shared_ptr of type JsonParseHelper </param>
		void RemoveHelper(const std::shared_ptr<JsonParseHelper>& helper);
		
		/// <summary>
		/// Overloaded DeserializeObject method for ParseCoordinator
		/// </summary>
		/// <param name="input"> Takes a const reference to a std::string </param>
		void DeserializeObject(const std::string& input);

		/// <summary>
		/// DeserializeObjectFromFile method for ParseCoordinator
		/// </summary>
		/// <param name="fileName"> Takes a const reference to a std::string </param>
		void DeserializeObjectFromFile(const std::string& fileName);

		/// <summary>
		/// Overloaded DeserializeObject method for ParseCoordinator
		/// </summary>
		/// <param name="stream"> Takes a reference to a std::istream </param>
		void DeserializeObject(std::istream& stream);
		
		/// <summary>
		/// _sharedData Getter method for ParseCoordinator
		/// </summary>
		/// <returns> Returns a shared_ptr of type JsonParseHelper::SharedData </returns>
		std::shared_ptr<JsonParseHelper::SharedData> GetSharedData();

		/// <summary>
		/// _sharedData Setter method for ParseCoordinator
		/// </summary>
		/// <param name="newSharedData"> Takes a shared_ptr of type JsonParseHelper::SharedData </param>
		void SetSharedData(std::shared_ptr<JsonParseHelper::SharedData> newSharedData);

		/// <summary>
		/// _helpers Accessor method for ParseCoordinator 
		/// </summary>
		/// <returns> Returns a const reference to a Vector of type shared_ptr<JsonParseHelper>> </returns>
		const Vector<std::shared_ptr<JsonParseHelper>>& Helpers() const;
	private:
		/// <summary>
		/// ParseMembers method for ParseCoordinator
		/// </summary>
		/// <param name="input"> Takes a reference to a Json::Value object </param>
		void ParseMembers(Json::Value& input, bool isArray = false);

		/// <summary>
		/// Parse method for ParseCoordinator
		/// </summary>
		/// <param name="key"> Takes a const reference to a std::string </param>
		/// <param name="value"> Takes a reference to a Json::Value object</param>
		void Parse(const std::string& key, Json::Value& value, bool isArray = false, size_t index = 0);

		Vector<std::shared_ptr<JsonParseHelper>> _helpers{ 10 };
		std::shared_ptr<JsonParseHelper::SharedData> _sharedData{ nullptr };
	};
}