#include <thirdparty/shogl/include/shogl.hpp>

#define GLO_USE_STB
#include <thirdparty/glo/include/glo.hpp>


#include <include/px.hpp>

namespace
{
	std::shared_ptr<glo::hud> hud;
}


SHOGL()
{

	shogl()->window_title("px");
	shogl()->window_size(1000, 800);

	hud = std::make_shared<glo::hud>(shogl()->window_width(), shogl()->window_height(), glo::ttf_font("rhregular.ttf", 48));
	hud->char_dim(16, 25);
	hud->char_stride(-1); 
	
	px::Context context({},
		[](const wchar_t* msg)
		{
			*hud << px::ToStr(px::Str(msg));
		});

	// Create an observer for messages...
	px::Observer::Message m(
		[](const px::String& msg)
		{
			*hud << px::ToStr(msg);
		});

	// Create an observer for exceptions...
	px::Observer::Exception e(
		[](const std::vector<px::Exception>& ee)
		{
			hud->fg(1.0, 0, 0);
			*hud << "Error...";
		});


	shogl()->draw([=]() 
		{
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, shogl()->window_width(), shogl()->window_height());

			// Draw the hud..
			hud->draw_frame();
		});

	shogl()->resize([=](int width, int height)
		{
			// Resize the hud...
			hud->resize(width, height);
		});


}