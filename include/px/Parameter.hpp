#ifndef PX_PARAMETER_HPP
#define PX_PARAMETER_HPP

#include <variant>
#include <optional>
#include <map>

#include "type.hpp"

namespace px
{
	class Parameter
	{
		std::variant<bool, Integer, float, String> value_;
	public:

		using Key = String;

		/// <summary>
		/// 
		/// </summary>
		Parameter() : value_(0) {}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		Parameter(int value) : value_(value) {}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		Parameter(float value) : value_(value) {}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		Parameter(bool value) : value_(value) {}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		Parameter(const String& value) : value_(value) {}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		bool operator==(const Parameter& rhs) const
		{
			// First check if they are strings...
			if (auto lhsStringValue = GetString())
			{
				if (auto rhsStringValue = rhs.GetString())
					return *lhsStringValue == *rhsStringValue;
			}

			// Then check if they are floats...
			if (auto lhsFloatValue = GetFloat())
			{
				if (auto rhsFloatValue = rhs.GetFloat())
					return *lhsFloatValue == *rhsFloatValue;
			}

			// Then check if they are ints...
			if (auto lhsIntValue = GetInt())
			{
				if (auto rhsIntValue = rhs.GetInt())
					return *lhsIntValue == *rhsIntValue;
			}

			// Finally check if they are bools...
			if (auto lhsBoolValue = GetBool())
			{
				if (auto rhsBoolValue = rhs.GetBool())
					return *lhsBoolValue == *rhsBoolValue;
			}

			return false;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		bool operator!=(const Parameter& rhs) const
		{
			return !(*this == rhs);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		std::optional<Integer> GetInt() const
		{
			if (const int* val = std::get_if<Integer>(&value_))
				return *val;
			return std::optional<int>();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		std::optional<bool> GetBool() const
		{
			if (const bool* val = std::get_if<bool>(&value_))
				return *val;
			return std::optional<bool>();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		std::optional<float> GetFloat() const
		{
			if (const float* val = std::get_if<float>(&value_))
				return *val;
			return std::optional<float>();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		std::optional<String> GetString() const
		{
			if (const String* val = std::get_if<String>(&value_))
				return *val;
			return std::optional<String>();
		}

		String GetAsString() const
		{
			if (auto val = GetFloat())
				return px::Str(std::to_string(*val));
			if (auto val = GetInt())
				return px::Str(std::to_string(*val));
			if (auto val = GetBool())
				return px::Str(*val ? "True" : "False");
			if (auto val = GetString())
				return *val;
			return px::Str("%");
		}
	};

	using Parameters = std::map<Parameter::Key, Parameter>;
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="params"></param>
	/// <param name="parameterName"></param>
	/// <param name="defaultParameterValue"></param>
	/// <returns></returns>
	Parameter GetParameter(const Parameters& params, const Parameter::Key& parameterName, const Parameter& defaultParameterValue);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="params"></param>
	/// <param name="parameterName"></param>
	/// <returns></returns>
	const Parameter* GetParameter(const Parameters& params, const Parameter::Key& parameterName);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="syntax"></param>
	/// <returns></returns>
	Parameters From(const String& syntax);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="parameters"></param>
	/// <returns></returns>
	String To(const Parameters& parameters);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="params"></param>
	/// <param name="paramsToMerge"></param>
	/// <param name="overwrite"></param>
	/// <returns></returns>
	Parameters MergeParameters(const Parameters& params, const Parameters& paramsToMerge, bool overwrite);

}

#endif // PX_PARAMETER_HPP