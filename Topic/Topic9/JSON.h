#pragma once

#include <vector>
#include <string>
#include <map>

using namespace std;

// 至少N个字节数
static inline bool CheckLen(const wchar_t* s, size_t n) {
	if (s == 0)
		return false;

	const wchar_t* save = s;
	while (n-- > 0)
	{
		if (*(save++) == 0) 
			return false;
	}

	return true;
}

class JSONValue;
typedef vector<JSONValue*> JSONArray;
typedef map<wstring, JSONValue*> JSONObject;

#include "JSONValue.h"

class JSON
{
	friend class JSONValue;

public:
	static JSONValue* Parse(const wchar_t* data);
	static wstring Stringify(const JSONValue* value);

protected:
	static bool SkipWhitespace(const wchar_t** data);
	static bool ExtractString(const wchar_t** data, wstring& str);
	static double ParseInt(const wchar_t** data);
	static double ParseDecimal(const wchar_t** data);

private:
	JSON();
};