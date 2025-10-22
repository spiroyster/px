#include <include/px.hpp>

#include <iostream>

void OutputError(const px::Exception& e)
{
	std::wcerr << px::Str("!!\n");
	for (auto eItr = e.Meta().begin(); eItr != e.Meta().end(); ++eItr)
		std::wcerr << px::Str("| ") << eItr->first << px::Str(": ") << eItr->second.GetAsString() << '\n';
	std::wcerr << px::Str("!!\n");
}

int main(int argc, char** argv)
{
	// Craate a handle to the pX system. This is owned by this 
	try
	{
		// Create our parameters...
		//px::Parameters param;
		//param[px::KeyID::ClientPath] = 

		// Create our px context...
		auto context = px::Context({}, 
			[](const wchar_t* msg) 
			{ 
				std::wcout << msg << '\n'; 
			});

		// Create an observer for messages...
		px::Observer::Message m(
			[](const px::String& msg) 
			{
				std::wcout << msg.c_str() << '\n';
			});

		// Create an observer for exceptions...
		px::Observer::Exception e(
			[](const std::vector<px::Exception>& ee) 
			{
				for (auto e : ee)
					OutputError(e);
			});

		// Create an observer for kill...
		bool kill = false;

		px::Observer::Kill k(
			[&kill]() 
			{ 
				kill = true; 
			});

		// Main command loop...
		while (!kill)
		{
			std::cout << "px>";

			// Read the input...
			std::string input;
			std::getline(std::cin, input);

			try
			{
				// Invoke the input as a command...
				px::Command(px::Str(input));
			}
			catch (const px::Exception& e)
			{
				px::Context::ReportException({ e });
			}

		}

		// Kill the context
		context.Kill({});

		return 0;
	}
	catch (const px::Exception& e)
	{
		std::wcerr << px::To(e.Meta()).c_str();
	}
	catch (...)
	{
		std::wcerr << px::Str("Unknown Failure").c_str();
	}
	
	return 1;
}