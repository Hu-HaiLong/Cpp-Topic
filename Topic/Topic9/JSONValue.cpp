
#include <sstream>
#include "JSONValue.h"

// Macros to free an array/object
#define FREE_ARRAY(x) { JSONArray::iterator Iter; for (Iter = x.begin(); Iter != x.end(); Iter++) { delete *Iter; } }
#define FREE_OBJECT(x) { JSONObject::iterator Iter; for (Iter = x.begin(); Iter != x.end(); Iter++) { delete (*Iter).second; } }

/**
 * Parses a JSON encoded value to a JSONValue object
 *
 * @return JSONValue* Returns a pointer to a JSONValue object on success, NULL on error
 */
JSONValue* JSONValue::Parse(const wchar_t** InData)
{
    // String
    if (**InData == '"')
    {
        wstring Str;

        if (!JSON::ExtractString(&(++(*InData)), Str))
        {
            return NULL;
        }
        else
        {
            return new JSONValue(Str);
        }
    }
    // Bool
    else if ((CheckLen(*InData, 4) && _wcsnicmp(*InData, L"true", 4) == 0) || (CheckLen(*InData, 5) && _wcsnicmp(*InData, L"false", 5) == 0))
    {
        bool value = _wcsnicmp(*InData, L"true", 4) == 0;
        (*InData) += value ? 4 : 5;
        return new JSONValue(value);
    }
    // NULL
    else if (CheckLen(*InData, 4) && _wcsnicmp(*InData, L"null", 4) == 0)
    {
        (*InData) += 4;
        return new JSONValue();
    }
    // Number 
    else if (**InData == L'-' || (**InData >= L'0' && **InData <= L'9'))
    {
        // Negative
        bool Neg = **InData == L'-';
        if (Neg) (*InData)++;

        double Number = 0.0;

        // Not 0
        if (**InData == L'0')
        {
            (*InData)++;
        }
        else if (**InData >= L'1' && **InData <= L'9')
        {
            Number = JSON::ParseInt(InData);
        }
        else
        {
            return NULL;
        }

        // Decimal
        if (**InData == '.')
        {
            (*InData)++;

            // No More 
            if (!(**InData >= L'0' && **InData <= L'9'))
                return NULL;

            double decimal = JSON::ParseDecimal(InData);

            // Save the number
            Number += decimal;
        }

        // 幂
        if (**InData == L'E' || **InData == L'e')
        {
            (*InData)++;

            // 正负检查
            bool bIsNeg = false;
            if (**InData == L'-' || **InData == L'+')
            {
                bIsNeg = **InData == L'-';
                (*InData)++;
            }

            if (!(**InData >= L'0' && **InData <= L'9'))
            {
                return NULL;
            }

            // Sort the expo out
            double Pow = JSON::ParseInt(InData);
            for (double i = 0.0; i < Pow; i++)
            {
                Number = bIsNeg ? (Number / 10.0) : (Number * 10.0);
            }
        }

        // 负数
        if (Neg)
        {
            Number *= -1;
        }

        return new JSONValue(Number);
    }
    // Object {} 
    else if (**InData == L'{')
    {
        JSONObject Object;

        (*InData)++;

        while (**InData != 0)
        {
            // 开始有空格跳过
            if (!JSON::SkipWhitespace(InData))
            {
                FREE_OBJECT(Object);
                return NULL;
            }

            // Empty
            if (Object.size() == 0 && **InData == L'}')
            {
                (*InData)++;
                return new JSONValue(Object);
            }

            // Map Key
            wstring Key;
            if (!JSON::ExtractString(&(++(*InData)), Key))
            {
                FREE_OBJECT(Object);
                return NULL;
            }

            // More whitespace?
            if (!JSON::SkipWhitespace(InData))
            {
                FREE_OBJECT(Object);
                return NULL;
            }

            // Need a : now
            if (*((*InData)++) != L':')
            {
                FREE_OBJECT(Object);
                return NULL;
            }

            // More whitespace?
            if (!JSON::SkipWhitespace(InData))
            {
                FREE_OBJECT(Object);
                return NULL;
            }

            // The value is here
            JSONValue* Value = Parse(InData);
            if (Value == NULL)
            {
                FREE_OBJECT(Object);
                return NULL;
            }

            // Add the Name:value
            if (Object.find(Key) != Object.end())
                delete Object[Key];
            Object[Key] = Value;

            // More whitespace?
            if (!JSON::SkipWhitespace(InData))
            {
                FREE_OBJECT(Object);
                return NULL;
            }

            // End of object?
            if (**InData == L'}')
            {
                (*InData)++;
                return new JSONValue(Object);
            }

            // Want a , now
            if (**InData != L',')
            {
                FREE_OBJECT(Object);
                return NULL;
            }

            (*InData)++;
        }

        // Only here if we ran out of data
        FREE_OBJECT(Object);
        return NULL;
    }
    // Array []
    else if (**InData == L'[')
    {
        JSONArray Array;

        (*InData)++;

        while (**InData != 0)
        {
            // Whitespace at the start?
            if (!JSON::SkipWhitespace(InData))
            {
                FREE_ARRAY(Array);
                return NULL;
            }

            // Empty
            if (Array.size() == 0 && **InData == L']')
            {
                (*InData)++;
                return new JSONValue(Array);
            }

            // Get the value
            JSONValue* Value = Parse(InData);

            if (Value == NULL)
            {
                FREE_ARRAY(Array);
                return NULL;
            }

            // Add the value
            Array.push_back(Value);

            // More whitespace?
            if (!JSON::SkipWhitespace(InData))
            {
                FREE_ARRAY(Array);
                return NULL;
            }

            // End
            if (**InData == L']')
            {
                (*InData)++;
                return new JSONValue(Array);
            }

            // Want a , now
            if (**InData != L',')
            {
                FREE_ARRAY(Array);
                return NULL;
            }

            (*InData)++;
        }

        // Only here if we ran out of data
        FREE_ARRAY(Array);
        return NULL;
    }
    // Out of
    else
    {
        return NULL;
    }
}

JSONValue::JSONValue()
{
    Type = JSONType::Null;
}

JSONValue::JSONValue(const wchar_t* InCharValue)
{
    Type = JSONType::String;
    StringValue = new wstring(wstring(InCharValue));
}

JSONValue::JSONValue(const wstring& InStringValue)
{
    Type = JSONType::String;
    StringValue = new wstring(InStringValue);
}

JSONValue::JSONValue(bool InBoolValue)
{
    Type = JSONType::Bool;
    BoolValue = InBoolValue;
}

JSONValue::JSONValue(double InDoubleValue)
{
    Type = JSONType::Number;
    NumberValue = InDoubleValue;
}

JSONValue::JSONValue(int InIntValue)
{
    Type = JSONType::Number;
    NumberValue = (double)InIntValue;
}

JSONValue::JSONValue(const JSONArray& InArrayValue)
{
    Type = JSONType::Array;
    ArrayValue = new JSONArray(InArrayValue);
}

JSONValue::JSONValue(const JSONObject& InObjectValue)
{
    Type = JSONType::Object;
    ObjectValue = new JSONObject(InObjectValue);
}

JSONValue::JSONValue(const JSONValue& InSource)
{
    Type = InSource.Type;

    switch (Type)
    {
    case JSONType::String:
        StringValue = new wstring(*InSource.StringValue);
        break;

    case JSONType::Bool:
        BoolValue = InSource.BoolValue;
        break;

    case JSONType::Number:
        NumberValue = InSource.NumberValue;
        break;

    case JSONType::Array:
    {
        JSONArray SourceArray = *InSource.ArrayValue;
        JSONArray::iterator Iter;
        ArrayValue = new JSONArray();

        for (Iter = SourceArray.begin(); Iter != SourceArray.end(); Iter++)
        {
            ArrayValue->push_back(new JSONValue(**Iter));
        }

        break;
    }

    case JSONType::Object:
    {
        JSONObject SourceObject = *InSource.ObjectValue;
        ObjectValue = new JSONObject();
        JSONObject::iterator Iter;

        for (Iter = SourceObject.begin(); Iter != SourceObject.end(); Iter++)
        {
            wstring Name = (*Iter).first;
            (*ObjectValue)[Name] = new JSONValue(*((*Iter).second));
        }

        break;
    }

    case JSONType::Null:
        break;
    }
}


JSONValue::~JSONValue()
{
    if (Type == JSONType::Array)
    {
        JSONArray::iterator Iter;

        for (Iter = ArrayValue->begin(); Iter != ArrayValue->end(); Iter++)
        {
            delete* Iter;
        }

        delete ArrayValue;
    }
    else if (Type == JSONType::Object)
    {
        JSONObject::iterator Iter;

        for (Iter = ObjectValue->begin(); Iter != ObjectValue->end(); Iter++)
        {
            delete (*Iter).second;
        }

        delete ObjectValue;
    }
    else if (Type == JSONType::String)
    {
        delete StringValue;
    }
}

// 判断
bool JSONValue::IsNull() const
{
    return Type == JSONType::Null;
}

bool JSONValue::IsString() const
{
    return Type == JSONType::String;
}

bool JSONValue::IsBool() const
{
    return Type == JSONType::Bool;
}

bool JSONValue::IsNumber() const
{
    return Type == JSONType::Number;
}

bool JSONValue::IsArray() const
{
    return Type == JSONType::Array;
}

bool JSONValue::IsObject() const
{
    return Type == JSONType::Object;
}

// 提取
const wstring& JSONValue::AsString() const
{
    return (*StringValue);
}

bool JSONValue::AsBool() const
{
    return BoolValue;
}

double JSONValue::AsNumber() const
{
    return NumberValue;
}

const JSONArray& JSONValue::AsArray() const
{
    return (*ArrayValue);
}

const JSONObject& JSONValue::AsObject() const
{
    return (*ObjectValue);
}

JSONValue* JSONValue::GetChild(const wchar_t* Name)
{
    JSONObject::const_iterator it = ObjectValue->find(Name);

    if (it != ObjectValue->end())
    {
        return it->second;
    }
    else
    {
        return NULL;
    }
}

vector<wstring> JSONValue::ObjectKeys() const
{
    vector<wstring> Keys;

    if (Type == JSONType::Object)
    {
        JSONObject::const_iterator Iter = ObjectValue->begin();

        while (Iter != ObjectValue->end())
        {
            Keys.push_back(Iter->first);

            Iter++;
        }
    }

    return Keys;
}

// 用于打印 带缩进
wstring JSONValue::Stringify(bool const InPrettyPrint) const
{
    size_t const IndentDepth = InPrettyPrint ? 1 : 0;
    return StringifyImpl(IndentDepth);
}

wstring JSONValue::StringifyImpl(size_t const InIndentDepth) const
{
    wstring ReturnStr;
    size_t const IndentDepthS = InIndentDepth ? InIndentDepth + 1 : 0;
    wstring const IndentStr = Indent(InIndentDepth);
    wstring const IndentStrS = Indent(IndentDepthS);

    switch (Type)
    {
    case JSONType::Null:
        ReturnStr = L"null";
        break;

    case JSONType::String:
        ReturnStr = StringifyString(*StringValue);
        break;

    case JSONType::Bool:
        ReturnStr = BoolValue ? L"true" : L"false";
        break;

    case JSONType::Number:
    {
        if (isinf(NumberValue) || isnan(NumberValue))
        {
            ReturnStr = L"null";
        }
        else
        {
            wstringstream ss;
            ss.precision(15);
            ss << NumberValue;
            ReturnStr = ss.str();
        }

        break;
    }

    case JSONType::Array:
    {
        ReturnStr = InIndentDepth ? L"[\n" + IndentStrS : L"[";
        JSONArray::const_iterator Iter = ArrayValue->begin();

        while (Iter != ArrayValue->end())
        {
            ReturnStr += (*Iter)->StringifyImpl(IndentDepthS);

            // Not at the end - add a separator
            if (++Iter != ArrayValue->end())
            {
                ReturnStr += L",";
            }
        }

        ReturnStr += InIndentDepth ? L"\n" + IndentStr + L"]" : L"]";
        break;
    }

    case JSONType::Object:
    {
        ReturnStr = InIndentDepth ? L"{\n" + IndentStrS : L"{";
        JSONObject::const_iterator Iter = ObjectValue->begin();

        while (Iter != ObjectValue->end())
        {
            ReturnStr += StringifyString((*Iter).first);
            ReturnStr += L":";
            ReturnStr += (*Iter).second->StringifyImpl(IndentDepthS);

            // Not at the end - add a separator
            if (++Iter != ObjectValue->end())
            {
                ReturnStr += L",";
            }
        }

        ReturnStr += InIndentDepth ? L"\n" + IndentStr + L"}" : L"}";
        break;
    }
    }

    return ReturnStr;
}

/**
 * 转义Json编码字符串
 * Works from http://www.ecma-internationl.org/publications/files/ECMA-ST/ECMA-262.pdf
 */
wstring JSONValue::StringifyString(const wstring& InStr)
{
    wstring StrReturn = L"\"";

    wstring::const_iterator Iter = InStr.begin();

    while (Iter != InStr.end())
    {
        wchar_t Chr = *Iter;

        if (Chr == L'"' || Chr == L'\\' || Chr == L'/')
        {
            StrReturn += L'\\';
            StrReturn += Chr;
        }
        else if (Chr == L'\b')
        {
            StrReturn += L"\\b";
        }
        else if (Chr == L'\f')
        {
            StrReturn += L"\\f";
        }
        else if (Chr == L'\n')
        {
            StrReturn += L"\\n";
        }
        else if (Chr == L'\r')
        {
        }
        else if (Chr == L'\t')
        {
            StrReturn += L"\\t";
        }
        else if (Chr < L' ' || Chr > 126)
        {
            StrReturn += L"\\u";
            for (int i = 0; i < 4; i++)
            {
                int Value = (Chr >> 12) & 0xf;
                if (Value >= 0 && Value <= 9)
                    StrReturn += (wchar_t)('0' + Value);
                else if (Value >= 10 && Value <= 15)
                    StrReturn += (wchar_t)('A' + (Value - 10));
                Chr <<= 4;
            }
        }
        else
        {
            StrReturn += Chr;
        }

        Iter++;
    }

    StrReturn += L"\"";
    return StrReturn;
}

// 格式缩进
wstring JSONValue::Indent(size_t InDepth)
{
    const size_t IndentStep = 2;
    InDepth ? --InDepth : 0;
    wstring IndentStr(InDepth * IndentStep, ' ');
    return IndentStr;
}