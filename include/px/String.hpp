#ifndef PX_UTIL_STRING_HPP
#define PX_UTIL_STRING_HPP

#include <codecvt>
#include <locale>
#include <algorithm>
#include <Windows.h>
#include <cwctype>
#include <list>
#include <sstream>

namespace px
{
	namespace Util
	{
		
		static char ToLowerCase(char ch)
		{
			return std::tolower(ch);
		}

		static wchar_t ToLowerCase(wchar_t ch)
		{
			return std::towlower(ch);
		}

		static char ToUpperCase(char ch)
		{
			return std::toupper(ch);
		}

		static wchar_t ToUpperCase(wchar_t ch)
		{
			return std::towupper(ch);
		}

		static std::string ToLowerCase(const std::string& str)
		{
			std::string result = str;
			for (unsigned int c = 0; c < result.size(); ++c)
				result[c] = ToLowerCase(result[c]);
			return result;
		}

		static std::wstring ToLowerCase(const std::wstring& str)
		{
			std::wstring result = str;
			for (unsigned int c = 0; c < result.size(); ++c)
				result[c] = ToLowerCase(result[c]);
			return result;
		}

		static std::string ToUpperCase(const std::string& str)
		{
			std::string result = str;
			for (unsigned int c = 0; c < result.size(); ++c)
				result[c] = ToUpperCase(result[c]);
			return result;
		}

		static std::wstring ToUpperCase(const std::wstring& str)
		{
			std::wstring result = str;
			for (unsigned int c = 0; c < result.size(); ++c)
				result[c] = ToUpperCase(result[c]);
			return result;
		}

		static bool IsWhiteSpace(wchar_t c)
		{
			return c == L' ';
		}

		static bool IsWhiteSpace(char c)
		{
			return c == ' ';
		}

		static std::string RightTrim(const std::string& str, const std::string& chars)
		{
			std::string newStr = str;
			newStr.erase(newStr.find_last_not_of(chars) + 1);
			return newStr;
		}

		static std::wstring RightTrim(const std::wstring& str, const std::wstring& chars)
		{
			std::wstring newStr = str;
			newStr.erase(newStr.find_last_not_of(chars) + 1);
			return newStr;
		}

		static std::string LeftTrim(const std::string& str, const std::string& chars)
		{
			std::string newStr = str;
			newStr.erase(0, newStr.find_first_not_of(chars));
			return newStr;
		}

		static std::wstring LeftTrim(const std::wstring& str, const std::wstring& chars)
		{
			std::wstring newStr = str;
			newStr.erase(0, newStr.find_first_not_of(chars));
			return newStr;
		}

		static std::string Trim(const std::string& str, const std::string& chars)
		{
			return LeftTrim(RightTrim(str, chars), chars);
		}

		static std::wstring Trim(const std::wstring& str, const std::wstring& chars)
		{
			return LeftTrim(RightTrim(str, chars), chars);
		}


		static bool IsWhiteSpace(const std::wstring& str)
		{
			for (unsigned int c = 0; c < str.size(); ++c)
				if (!IsWhiteSpace(str[c]))
					return false;

			return true;
		}

		static bool IsWhiteSpace(const std::string& str)
		{
			for (unsigned int c = 0; c < str.size(); ++c)
				if (!IsWhiteSpace(str[c]))
					return false;

			return true;
		}

		static std::wstring ReplaceAll(const std::wstring& str, const std::wstring& what, const std::wstring& with)
		{
			std::wstring result = str;
			auto itr = result.find(what);
			while (itr != std::wstring::npos)
			{
				result.replace(itr, what.size(), with);
				itr = result.find(what, itr + 1);
			}
			return result;
		}

		static std::vector<px::String> Split(const px::String& syntax)
		{
			std::list<px::String> result;
			std::wstringstream iss(syntax);

			px::String line;
			while (std::getline(iss, line))
				result.emplace_back(line);

			return { result.begin(), result.end() };
		}

		static px::String Join(const std::vector<px::String>& lines)
		{
			px::String result;

			for (unsigned int ln = 0; ln < static_cast<unsigned int>(lines.size()); ++ln)
				result.append(lines[ln] + px::Str("\n"));

			return result;
		}

		/// <summary>
		/// Case insensitive equality check.
		/// </summary>
		/// <param name="lhs">Left hand string to compare.</param>
		/// <param name="rhs">Right hand string to compare.</param>
		/// <returns>True if the strings have the same sequentially ordered characters (irrelevant of case)</returns>
		static bool StringsEqualIgnoringCase(const std::string& lhs, const std::string& rhs)
		{
			if (lhs.size() != rhs.size())
				return false;

			for (unsigned int c = 0; c < lhs.size(); ++c)
				if (ToLowerCase(lhs[c]) != ToLowerCase(rhs[c]))
					return false;

			return true;
		}

		static bool StringsEqualIgnoringCase(const std::wstring& lhs, const std::wstring& rhs)
		{
			if (lhs.size() != rhs.size())
				return false;

			for (unsigned int c = 0; c < lhs.size(); ++c)
				if (ToLowerCase(lhs[c]) != ToLowerCase(rhs[c]))
					return false;

			return true;
		}
	}
}

#endif // PX_UTIL_STRING_HPP