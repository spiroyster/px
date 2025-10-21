#include <pxlib/px.hpp>

PX_MAIN
{
	px::Message(px::Str("Initialising module : ") + px::Context::Name());
}

PX_KILL
{
	px::Message(px::Str("Killing module : ") + px::Context::Name());
}

PX_COMMAND(command1)
{
	px::Message(px::Str("module1.command1"));
	return true;
}

PX_COMMAND(command2)
{
	px::Message(px::Str("module1.command2"));
	return true;
}