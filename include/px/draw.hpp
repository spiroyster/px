#ifndef PX_DRAW_HPP
#define PX_DRAW_HPP

#include "px.hpp"

namespace px
{

	// texture

	// view

	// scene

	// scene object

	// camera

	// gui control




	// Draw events..
	PX_EVENT_1(DrawGeometryChanged, const String&, msg)
	PX_EVENT_1(DrawTransformChanged, const String&, msg)
	PX_EVENT_3(DisplayResize, int, displayID, unsigned int, width, unsigned int, height)
	PX_EVENT_1(ViewRegistered, const String&, viewName)
	PX_EVENT_1(ViewUnregistered, const String&, viewName)
	PX_EVENT_2(ViewShow, int, displayID, const String&, viewName)
	PX_EVENT_2(ViewHide, int, displayID, const String&, viewName)
	PX_EVENT_2(ViewResize, int, displayID, const String&, viewName)




		//AxW_EVENT_3(DisplayResize, int, displayID, unsigned int, width, unsigned int, height)
		//AxW_EVENT_1(ViewRegistered, const String&, viewName)
		//AxW_EVENT_1(ViewUnregistered, const String&, viewName)
		//AxW_EVENT_2(ViewShow, int, displayID, const String&, viewName)
		//AxW_EVENT_2(ViewHide, int, displayID, const String&, viewName)
		//AxW_EVENT_2(ViewResize, int, displayID, const String&, viewName)
		//AxW_EVENT_2(ViewAddScene, const String&, viewName, const String&, sceneName)
		//AxW_EVENT_2(ViewRemoveScene, const String&, viewName, const String&, sceneName)
		//AxW_EVENT_1(SceneRegistered, const String&, sceneName)
		//AxW_EVENT_1(SceneUnregistered, const String&, sceneName)
		//AxW_EVENT_2(SceneAddSceneObject, const String&, sceneName, const String&, objectName)
		//AxW_EVENT_2(SceneRemoveSceneObject, const String&, sceneName, const String&, objectName)
		//AxW_EVENT_1(SceneObjectRegistered, const String&, objectName)
		//AxW_EVENT_1(SceneObjectUnregistered, const String&, objectName)
		//AxW_EVENT_1(SceneObjectTransformChanged, const String&, objectName)
		//AxW_EVENT_1(SceneObjectMaterialChanged, const String&, objectName)
		//AxW_EVENT_1(SceneObjectGeometryChanged, const String&, objectName)
		//AxW_EVENT_1(TextureRegistered, const String&, textureName)
		//AxW_EVENT_1(TextureUnregistered, const String&, textureName)
		//AxW_EVENT_1(TextureChanged, const String&, textureName)
		//AxW_EVENT_1(GUIControlRegistered, const String&, guiControlName)
		//AxW_EVENT_1(GUIControlUnregistered, const String&, guiControlName)
		//AxW_EVENT_1(PaletteChanged, const String&, paletteEntryName)
		//AxW_EVENT_1(DrawAnimationRegistered, const String&, animationName)
		//AxW_EVENT_1(DrawAnimationUnregistered, const String&, animationName)


}

#endif // PX_DRAW_HPP