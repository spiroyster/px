#ifndef PXGL_DRAWOBJECT_HPP
#define PXGL_DRAWOBJECT_HPP

#include "GLFunctions.hpp"

namespace pxgl
{
	class GLDrawObject
	{
	public:
		
		enum DirtyState
		{
			//NotDirty = 0,
			GeometryDirty = 0,
			MaterialDirty,
			TransformDirty,
			Destroyed
		};

		GLDrawObject(const px::DrawObject* drawObject, unsigned int pickerID) 
			:	drawObject_(drawObject), pickerID_(pickerID) 
		{
		}

		void Free(const GLContext& context)
		{
			try
			{
				context.Functions().glDeleteBuffers_(buffers_.size(), &buffers_[0]);
				buffers_.clear();
			}
			catch (...)
			{
			}
		}

		unsigned int PickerID() const { return pickerID_; }

		void SetDirtyState(const DirtyState& state)
		{
			dirtyState_ = state;
			drawObject_ = state == DirtyState::Destroyed ? nullptr : drawObject_;
		}

		const DirtyState& GetDirtyState() const { return dirtyState_; }
		
		// The buffers...
		std::vector<unsigned int> buffers_;

		// The VAO
		unsigned int vao_;

		// Ptr to px::DrawObject
		const px::DrawObject* drawObject_;

		unsigned int pickerID_;
		DirtyState dirtyState_;
	};
}


#endif // PXGL_DRAWOBJECT_HPP