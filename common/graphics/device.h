// BismIllahIRRahmaanIRRaheem
/* graphics device vclass declaration */
#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H
#include <graphics/graphicsdef.h>
#include <runtime/window.h>
#include <runtime/updateable.h>
namespace falx {
	struct IGraphicsDevice : IUpdateable {
		virtual bit Create(bit aAllowSoftwareRendering, IWindow* aiWindow) = 0;
	};
	// The output should be NULL on failure
	// -
	void CreateGraphicsDevice(IGraphicsDevice*& oiGraphicsDevice, bit aAllowSoftwareRendering, IWindow* aiWindow);
	// The output should be NULL on failure
	// -
	void CreateGraphicsDevice(IGraphicsDevice*& oiGraphicsDevice, IWindow* aiWindow);
}
#endif // !GRAPHICS_DEVICE_H