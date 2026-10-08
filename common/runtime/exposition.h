// BismIllahIRRahmaanIRRaheem
/* general exposer */
#ifndef FLX_WINDOWEXPOSITION_H
#define FLX_WINDOWEXPOSITION_H
#include <runtime/window.h>
#include <graphics/device.h>
#include <vector>
namespace falx {
	inline IWindow* g_iWindow;
	inline IGraphicsDevice* g_iGdev;
	inline std::vector<IUpdateable*> g_IUpdateables;
}
#endif // !FLX_WINDOWEXPOSITION_H