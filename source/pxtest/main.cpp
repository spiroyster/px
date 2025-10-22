#include <thirdparty/jahoutf/include/jahoutf.hpp>
#include <pxlib/px.hpp>

JAHOUTF_TEST_RUNNER{ RUNALL }

// Create the context...kill the context...
TEST(handleKill, "instance kill")
{
	try
	{
		auto context = px::Context({}, [](const wchar_t* msg) { std::wcout << msg << '\n'; });
		context.Kill();
		SUCCEED
	}
	catch (...) { FAIL; }
}

// Create the context...	// Hard reset (no elegant shutdown)...
TEST(handleScoped, "handle scoped")
{
	try
	{
		auto context = px::Context({}, [](const wchar_t* msg) { std::wcout << msg << '\n'; });
	}
	catch (...)
	{
		FAIL;
	}
	SUCCEED
}

// 
TEST(handleKillCall, "handle kill call scoped")
{
	auto context = px::Context({}, [](const wchar_t* msg) { std::wcout << msg << '\n'; });
	context.Kill();
	try
	{
		// Attempt to call something in the
		px::Message(px::Str("Called after kill"));
		FAIL
	}
	catch (...)
	{
	}
	SUCCEED;
}

// Events...
TEST(eventConstructDestruct, "event construct destruct")
{
	auto context = px::Context({}, [](const wchar_t* msg) { std::wcout << msg << '\n'; });
	auto msgCallback = std::make_shared<px::Observer::Message>([](const px::String& msg) { std::wcout << msg.c_str() << '\n'; });
	msgCallback.reset();
	context.Kill();
	SUCCEED
}

TEST(eventConstructInvokeDestruct, "event construct scoped")
{
	auto context = px::Context({}, [](const wchar_t* msg) { std::wcout << msg << '\n'; });
	auto msgCallback = std::make_shared<px::Observer::Message>([](const px::String& msg) { std::wcout << msg.c_str() << '\n'; });
	context.Kill();
	SUCCEED
}

TEST(eventInvoke, "event invoke")
{
	auto context = px::Context({}, [](const wchar_t* msg) { std::wcout << msg << '\n'; });
	auto msgCallback = std::make_shared<px::Observer::Message>([](const px::String& msg) { std::wcout << msg.c_str() << '\n'; });
	px::Dispatch::Message(px::Str("Message 42"));
	context.Kill();
	SUCCEED
}

TEST(eventInvokeDestructInvoke, "event invoke destruct")
{
	auto context = px::Context({}, [](const wchar_t* msg) { std::wcout << msg << '\n'; });
	auto msgCallback = std::make_shared<px::Observer::Message>(
		[=](const px::String& msg)
		{
			std::wcout << msg.c_str() << '\n';
			if (msg == px::Str("Should not see this"))
				FAIL
		});
	px::Dispatch::Message(px::Str("Message 42"));
	msgCallback.reset();
	px::Dispatch::Message(px::Str("Should not see this"));
	context.Kill();
	SUCCEED
}

class pxInstance : public jahoutf::fixture
{
	std::shared_ptr<px::Context> context_;
	std::shared_ptr<px::Observer::Message> message_;

public:
	void setup()
	{
		context_ = std::make_shared<px::Context>(px::Parameters(), [](const wchar_t* msg) { std::wcout << msg << '\n'; });
		message_ = std::make_shared<px::Observer::Message>(
			[=](const px::String& msg)
			{
				std::wcout << msg.c_str() << '\n';
			});
	}

	void teardown()
	{
		context_->Kill();
	}

};

// Test command valid...
TEST_F(commandValidSyntax, pxInstance, "command valid syntax")
{
	ASSERT(px::Command("version"))
		SUCCEED
}
//
TEST_F(commandValidParameters, pxInstance, "command valid parameters")
{
	ASSERT(px::Command({ px::Str("version") }))
		SUCCEED
}

TEST_F(commandEmptySyntax, pxInstance, "command empty syntax")
{
	ASSERT(!px::Command(""))
		SUCCEED
}

TEST_F(commandEmptyArguments, pxInstance, "command empty arguments")
{
	ASSERT(!px::Command(px::CommandInterface::Arguments()))
		SUCCEED
}

TEST_F(commandNotFoundArguments, pxInstance, "command invalid notfound arguments")
{
	try
	{
		px::Command({ px::Str("notACommand") });
		FAIL
	}
	catch (...) { SUCCEED }
}

TEST_F(commandNotFoundSyntax, pxInstance, "command invalid notfound syntax")
{
	try
	{
		px::Command("notACommand");
		FAIL
	}
	catch (...) { SUCCEED }
}

// Test module install
TEST_F(installModuleKill, pxInstance, "install pxtestmodule1 scoped")
{
	// Install the module
	px::Command("install pxtestmodule1");
	SUCCEED
}

TEST_F(installModuleUninstall, pxInstance, "install pxtestmodule1 uninstall")
{
	// Install the module
	px::Command("install pxtestmodule1");
	px::Command("uninstall pxtestmodule1");
	SUCCEED
}

// Load modules etc
TEST_F(installModuleCommand, pxInstance, "install pxtestmodule1 command")
{
	px::Command({ px::Str("install"), px::Str("pxtestmodule1") });
	ASSERT(px::Command({ px::Str("command1") }))
	SUCCEED
}

TEST_F(module1module2, pxInstance, "modules pxtestmodule1")
{

	ASSERT(px::Command({ px::Str("install"), px::Str("pxtestmodule1") }))
	ASSERT(px::Command({ px::Str("command1") }))
	ASSERT(px::Command({ px::Str("install"), px::Str("pxtestmodule2") }))
	ASSERT(px::Command({ px::Str("command1") }))
	ASSERT(px::Command({ px::Str("command2") }))
	SUCCEED
}

TEST_F(module1module2uninstall2command2, pxInstance, "modules pxtestmodule1")
{
	ASSERT(px::Command({ px::Str("install"), px::Str("pxtestmodule1") }))
	ASSERT(px::Command({ px::Str("command1") }))
	ASSERT(px::Command({ px::Str("install"), px::Str("pxtestmodule2") }))
	ASSERT(px::Command({ px::Str("command1") }))
	ASSERT(px::Command({ px::Str("command2") }))
	ASSERT(px::Command({ px::Str("uninstall"), px::Str("pxtestmodule2") }))

	try
	{
		px::Command({ px::Str("command2") });
		FAIL
	}
	catch (...)
	{
		SUCCEED
	}
}

TEST_F(module1module2uninstall1comman1, pxInstance, "modules pxtestmodule1")
{
	ASSERT(px::Command({ px::Str("install"), px::Str("pxtestmodule1") }))
	ASSERT(px::Command({ px::Str("command1") }))
	ASSERT(px::Command({ px::Str("command3") }))
	ASSERT(px::Command({ px::Str("install"), px::Str("pxtestmodule1") }))
	ASSERT(px::Command({ px::Str("command1") }))
	ASSERT(px::Command({ px::Str("command2") }))
	ASSERT(px::Command({ px::Str("uninstall"), px::Str("pxtestmodule1") }))

	try
	{
		px::Command({ px::Str("command3") });
		FAIL
	}
	catch (...)
	{
		SUCCEED
	}

}
