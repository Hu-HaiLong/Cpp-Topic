#include "JSON.h"


JSON::JSON()
{
}

JSONValue* JSON::Parse(const wchar_t* data)
{
    // 先跳过一波空段
    if (!SkipWhitespace(&data))
    {
        return NULL;
    }

    // 解析
    JSONValue* value = JSONValue::Parse(&data);
    if (value == NULL)
    {
        return NULL;
    }

    return value;
}

/**
 * JSONValue to JSON encode string
 */
wstring JSON::Stringify(const JSONValue* value)
{
    if (value != NULL)
    {
        return value->Stringify();
    }
    else
    {
        return L"";
    }
}

/**
 * 跳过空白
 */
bool JSON::SkipWhitespace(const wchar_t** data)
{
    while (**data != 0 && (**data == L' ' || **data == L'\t' || **data == L'\r' || **data == L'\n'))
    {
        (*data)++;
    }

    return **data != 0;
}

/**
 * Extracts a JSON String
 */
bool JSON::ExtractString(const wchar_t** data, wstring& str)
{
    str = L"";

    while (**data != 0)
    {
        // Save the char so we can change it if need be
        wchar_t next_char = **data;

        // 开头\\需要转义
        if (next_char == L'\\')
        {
            // Move over the escape char
            (*data)++;

            // Deal with the escaped char
            switch (**data)
            {
            case L'"':
                next_char = L'"';
                break;
            case L'\\':
                next_char = L'\\';
                break;
            case L'/':
                next_char = L'/';
                break;
            case L'b':
                next_char = L'\b';
                break;
            case L'f':
                next_char = L'\f';
                break;
            case L'n':
                next_char = L'\n';
                break;
            case L'r':
                next_char = L'\r';
                break;
            case L't':
                next_char = L'\t';
                break;
            case L'u':
            {
                // We need 5 chars (4 hex + the 'u') or its not valid
                if (!CheckLen(*data, 5))
                {
                    return false;
                }

                // Deal with the chars
                next_char = 0;

                for (int i = 0; i < 4; i++)
                {
                    // Do it first to move off the 'u' and leave us on the
                    // final hex digit as we move on by one later on
                    (*data)++;

                    next_char <<= 4;

                    // Parse the hex digit
                    if (**data >= '0' && **data <= '9')
                        next_char |= (**data - '0');
                    else if (**data >= 'A' && **data <= 'F')
                        next_char |= (10 + (**data - 'A'));
                    else if (**data >= 'a' && **data <= 'f')
                        next_char |= (10 + (**data - 'a'));
                    else
                        return false;
                }

                break;
            }

            // By the spec, only the above cases are allowed
            default:
                return false;
            }
        }

        // String 结束
        else if (next_char == L'"')
        {
            (*data)++;
            str.reserve(); // Remove unused capacity
            return true;
        }

        // ASCII码 32以前定义非法
        else if (next_char < L' ' && next_char != L'\t')
        {
            // SPEC Violation: Allow tabs due to real world cases
            return false;
        }

        // Add the next char
        str += next_char;

        // 继续推进
        (*data)++;
    }

    // If we're here, the string ended incorrectly
    return false;
}

/**
 * 解析整数
 */
double JSON::ParseInt(const wchar_t** Indate)
{
    double ReNum = 0;

    while (**Indate != 0 && **Indate >= '0' && **Indate <= '9')
    {
        ReNum = ReNum * 10 + (*(*Indate)++ - '0'); //char to number
    }

    return ReNum;
}

/**
 * 解析小数
 */
double JSON::ParseDecimal(const wchar_t** data)
{
    double decimal = 0.0;
    double factor = 0.1;

    while (**data != 0 && **data >= '0' && **data <= '9')
    {
        int digit = (*(*data)++ - '0');
        decimal = decimal + digit * factor;
        factor *= 0.1;
    }

    return decimal;
}