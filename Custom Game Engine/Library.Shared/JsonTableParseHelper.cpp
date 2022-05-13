#include "JsonTableParseHelper.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonTableParseHelper)
		RTTI_DEFINITIONS(JsonTableParseHelper::SharedData)

		
	const HashMap<const std::string, DatumTypes> JsonTableParseHelper::_datumTypeNameMap
	{
		std::pair("integer"s, DatumTypes::Integer),
		std::pair("float"s, DatumTypes::Float),
		std::pair("vector"s, DatumTypes::Vector),
		std::pair("matrix"s, DatumTypes::Matrix),
		std::pair("string"s, DatumTypes::String),
		std::pair("table"s, DatumTypes::Table)
	};

	void JsonTableParseHelper::Initialize()
	{
		JsonParseHelper::Initialize();
		InitializeCalled = true;
		StartHandlerCount = 0;
		EndHandlerCount = 0;
	}

	bool JsonTableParseHelper::StartHandler(JsonParseHelper::SharedData& sharedData, const std::string& key, const Json::Value& input, bool isArray, size_t index)
	{
		isArray;
		JsonTableParseHelper::SharedData* customSharedData = sharedData.As<JsonTableParseHelper::SharedData>();

		if (customSharedData == nullptr)
		{
			return false;
		}
		
		// "class"
		if (key == ClassKey)
		{
			StackFrame& currentContext = _callStack.Top();
			currentContext._className = input.asString();

		}
		// "type"
		else if (key == TypeKey)
		{
			StackFrame& currentContext = _callStack.Top();
			auto inputKey = input.asString();
			currentContext._type = _datumTypeNameMap.At(inputKey);
			Datum& contextDatum = currentContext._scopeContext->Append(currentContext._key);
			contextDatum.SetType(currentContext._type);
		}
		// "value"
		else if (key == ValueKey)
		{
			StackFrame& currentContext = _callStack.Top();

			// Non-Table
			if (currentContext._type != DatumTypes::Table)
			{
				Datum& contextDatum = currentContext._scopeContext->operator[](currentContext._key);
				
				if (contextDatum.IsExternal())
				{
					contextDatum.SetFromString(input.asString());
				}
				else
				{
					contextDatum.PushBackFromString(input.asString());
				}
			}
			// Table
			else
			{
				Datum* nestedDatum = currentContext._scopeContext->Find(currentContext._key);
				assert(nestedDatum != nullptr);
				Scope* nestedScope;
				
				if (nestedDatum != nullptr && nestedDatum->Size() > index)
				{
					nestedScope = &(*nestedDatum)[index];
				}
				else if (!currentContext._className.empty())
				{
					nestedScope = Factory<Scope>::Create(currentContext._className);
					assert(nestedScope != nullptr);
					currentContext._scopeContext->Adopt(*nestedScope, currentContext._key);
				}
				else
				{
					nestedScope = &(currentContext._scopeContext->AppendScope(currentContext._key));
				}

				if (isArray)
				{
					_callStack.Push(StackFrame(nestedScope, key, DatumTypes::Table, std::string()));
				}
				else
				{
					currentContext._scopeContext = nestedScope;
				}
			}
		}
		// "ObjectName" ?
		else
		{
			Scope* currentScopeContext;

			if (_callStack.Size() == 0)
			{
				currentScopeContext = customSharedData->_parentScope;
			}
			else
			{
				currentScopeContext = _callStack.Top()._scopeContext;
			}

			_callStack.Push(StackFrame(currentScopeContext, key, DatumTypes::Unknown, std::string()));
		}
		return true;
	}

	bool JsonTableParseHelper::EndHandler(JsonParseHelper::SharedData& sharedData, const std::string& key, const Json::Value& input)
	{
		key;
		input;
		JsonTableParseHelper::SharedData* customSharedData = sharedData.As<JsonTableParseHelper::SharedData>();

		if (customSharedData == nullptr)
		{
			return false;
		}
		if (&key == &_callStack.Top()._key)
		{
			_callStack.Pop();
		}

		if (customSharedData->MaxDepth < sharedData.Depth())
		{
			customSharedData->MaxDepth = sharedData.Depth();
		}

		return true;
	}

	std::shared_ptr<JsonTableParseHelper::SharedData> JsonTableParseHelper::SharedData::CreateSmart() const
	{
		return std::shared_ptr<JsonTableParseHelper::SharedData>(new SharedData);
	}
	
	JsonTableParseHelper::SharedData::SharedData(Scope& parentScope) :
		_parentScope(&parentScope)
	{
	}

	std::shared_ptr<JsonTableParseHelper> JsonTableParseHelper::CreateSmart() const
	{
		return std::shared_ptr<JsonTableParseHelper>(new JsonTableParseHelper);
	}

	JsonTableParseHelper::StackFrame::StackFrame(Scope* scopeContext,  const std::string& key, DatumTypes type, const std::string& className) :
		_scopeContext(scopeContext), _key(key), _type(type), _className(className)
	{
	}
}

