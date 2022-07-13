#pragma once

#include "JSON.h"
class JSON;

enum class JSONType
{
	Null, 
	String, 
	Bool, 
	Number, 
	Array, 
	Object
};

class JSONValue
{
	friend class JSON;

public:
	JSONValue();
	JSONValue(const wchar_t* InCharValue);
	JSONValue(const wstring& InStringValue);
	JSONValue(bool InBoolValue);
	JSONValue(double InDoubleValue);
	JSONValue(int InIntValue);
	JSONValue(const JSONArray& InArrayValue);
	JSONValue(const JSONObject& InObjectValue);
	JSONValue(const JSONValue& InSource);
	~JSONValue();

	bool IsNull() const;
	bool IsString() const;
	bool IsBool() const;
	bool IsNumber() const;
	bool IsArray() const;
	bool IsObject() const;

	const wstring& AsString() const;
	bool AsBool() const;
	double AsNumber() const;
	const JSONArray& AsArray() const;
	const JSONObject& AsObject() const;

	JSONValue* GetChild(const wchar_t* Name);
	vector<wstring> ObjectKeys() const;

	wstring Stringify(bool const prettyprint = false) const;

protected:
	static JSONValue* Parse(const wchar_t** data);

private:
	static wstring StringifyString(const wstring& str);
	wstring StringifyImpl(size_t const indentDepth) const;
	static wstring Indent(size_t depth);

	JSONType Type = JSONType::Null;

	union
	{
		bool BoolValue = NULL;
		double NumberValue;
		wstring* StringValue;
		JSONArray* ArrayValue;
		JSONObject* ObjectValue;
	};

};