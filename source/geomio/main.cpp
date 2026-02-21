#include <include/px.hpp>
#include <filesystem>

PX_MAIN
{
	px::Message(px::Str("Initialising module : ") + px::Context::Name());
}

PX_KILL
{
	px::Message(px::Str("Killing module : ") + px::Context::Name());
}

PX_COMMAND(import)
{
	// Check the file extension of the filepath...
	if (args.size() != 2)
		return false;

	// Check the file exists...
	if (!std::filesystem::exists(args.back()))
		throw px::Exception(px::ErrorID::FileNotFound, 
			{
				{ px::TagID::Command, Name() },
				{ px::TagID::Filename, args.back() },
				{ px::TagID::Callee, px::Str(__FUNCTION__) }
			});

	auto extension = std::filesystem::path(args.back()).extension();

	if (extension == ".obj")
		return px::Command(
			{
				px::Str("importobj"), 
				px::Str(args.back())
			});

	if (extension == ".stl")
		return px::Command(
			{
				px::Str("importstl"),
				px::Str(args.back())
			});

	return false;
}
