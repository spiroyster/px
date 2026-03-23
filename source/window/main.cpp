// Include for Single Header Only GL (window)
//#include <thirdparty/shogl/include/shogl.hpp>
#include <../shogl/include/shogl.hpp>

// Include for GL HUD
#define GLO_USE_STB
#include <thirdparty/glo/include/glo.hpp>

// Include the platform X 
#include <include/px.hpp>

// Include pxgl
#include <include/pxgl.hpp>

namespace
{
	std::shared_ptr<glo::hud> hud;
}


SHOGL()
{

	shogl()->window_title("px");
	shogl()->window_size(1000, 800);

	hud = std::make_shared<glo::hud>(shogl()->window_width(), shogl()->window_height(), glo::ttf_font("rhregular.ttf", 24));
	//hud = std::make_unique<glo::hud>(800, 600, glo::bitmap_font(glo::image_read("font1.png"), 0, 512 - (3 * 32), 32, -32));

	hud->char_dim(12, 20);
	hud->char_stride(-1); 
	//hud->char_stride(0);

	// Create the px::Context...
	px::Context context({},
		[](const wchar_t* msg)
		{
			*hud << px::ToStr(px::Str(msg));
		});

	// Create an observer for messages... 
	px::Observer::Message m(
		[](const px::String& msg)
		{
			*hud << px::ToStr(msg) + "\n";
		});

	// Create an observer for exceptions...
	px::Observer::Exception e(
		[](const std::vector<px::Exception>& ee)
		{
			*hud << "!!\n";
			for (const auto& e : ee)
				for (const auto& i : e.Meta())
					*hud << "| " + px::ToStr(i.first) + ": " + px::ToStr(i.second.GetAsString()) + "\n";
			*hud << "!!\n";
		});

	// Create our pxgl context...
	auto pxDisplay = pxgl::CreateDisplay(
		shogl()->window_width(), 
		shogl()->window_height(), 
		shogl()->window_context()->DC(), 
		shogl()->window_context()->GLC());

	for (auto info : pxDisplay->GetContextInfo())
		px::Message(info.first + px::Str(" : ") + info.second.GetAsString());

	// Load in the geomio module...
	px::Command({ px::Str("install"), px::Str("geomio")});
	
	// Load in the obj...
	px::Command({ px::Str("importobj"), px::Str("bunny.obj") });

	// Get the data from the data model...
	auto drawObjects = px::DataModelHandle(px::Str("bunny.obj")).Immutable().Entities<px::DrawObject>();

	// Create a view...
	//px::View view(px::Str("defaultView"));

	// Add the draw objects to the view...

	// Display the view
	 
	
	shogl()->draw([=]() 
		{
			glClearColor(0,0,0,0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, shogl()->window_width(), shogl()->window_height());

			// Draw the pxDisplay...
			pxDisplay->Render(nullptr);

			// Draw the hud..
			shogl()->window_context()->make_current(shogl()->window_context()->DC());
			hud->draw_frame();
		});

	shogl()->resize([=](int width, int height)
		{
			// Resize the pxDisplay
			pxDisplay->Resize(nullptr, width, height);

			// Resize the hud...
			shogl()->window_context()->make_current(shogl()->window_context()->DC());
			hud->resize(width, height);
		});

	shogl()->kill([&pxDisplay]() 
		{
			pxDisplay.reset();
		});

	return shogl()->window_show();
}