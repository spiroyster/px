#ifndef PX_COMMAND_HPP
#define PX_COMMAND_HPP

#include "type.hpp"

#include <vector>
#include <functional>

namespace px
{
	class CommandInterface
	{
	public:

		typedef std::vector<String> Arguments;
		typedef std::function<bool(const Arguments& args)> Callback;

		CommandInterface(const String& name);
		CommandInterface(const String& name, Callback callback);

		virtual ~CommandInterface();

		const String& Name() const { return name_; }
		const String& Owner() const { return owner_; }
		virtual bool Invoke(const Arguments& args) const { return callback_ ? callback_(args) : false; }

	private:
		String name_, owner_;
		Callback callback_;
	};

	bool Command(const String& syntax);
	bool Command(const std::string& syntax);
	bool Command(const CommandInterface::Arguments& arguments);
}

#define PX_COMMAND_INSTANCE(x) x ## Instance

#define PX_COMMAND(name) \
class name : public px::CommandInterface \
{\
public:\
	name() : px::CommandInterface(px::Str(#name)) {}\
	bool Invoke(const px::CommandInterface::Arguments& args) const;\
};\
namespace InstanceCommands\
{\
	name PX_COMMAND_INSTANCE(name);\
}\
bool name::Invoke(const px::CommandInterface::Arguments& args) const


#endif // PX_COMMAND_HPP