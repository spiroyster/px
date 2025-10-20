#ifndef PX_TYPE_HPP
#define PX_TYPE_HPP

#include <string>

#define PX_STRING(str) L## str

#define PX_TAG(tag) \
namespace px { namespace TagID { static const px::String tag = PX_STRING(#tag); } }

#define PX_KEY(key) \
namespace px { namespace KeyID { static const px::String key = PX_STRING(#key); } }

namespace px
{
	using String = std::wstring;
	using Integer = int;
	using UInteger = unsigned int;

	String Str(const std::string& s);
	String Str(const std::wstring& s);

	std::string ToStr(const String& str);
}

#endif // PX_TYPE_HPP