// BismIllahIRRahmaanIRRaheem
/* graphics device loader implementation */

#include <graphics/device.h>
#include <runtime/rtdebug.h>
#ifdef FLX_TRY_D3D11
#include <d3d11device.h>
#endif // FLX_TRY_D3D11
void falx::CreateGraphicsDevice(IGraphicsDevice*& oiGraphicsDevice, bit aAllowSoftwareRendering, IWindow* aiWindow)
{
	FLX_SMART_CHECK(aiWindow != NULL, "Invalid window for graphics device creation");
#ifdef FLX_TRY_D3D11
	oiGraphicsDevice = new D3D11GraphicsDevice;
	if (oiGraphicsDevice->Create(aAllowSoftwareRendering, aiWindow)) return;
	delete oiGraphicsDevice;
	oiGraphicsDevice = NULL;
#else
	oiGraphicsDevice = NULL;
	return;
#endif // FLX_TRY_D3D11
}

void falx::CreateGraphicsDevice(IGraphicsDevice*& oiGraphicsDevice, IWindow* aiWindow)
{
	FLX_SMART_CHECK(aiWindow != NULL, "Invalid window for graphics device creation");
#ifdef FLX_TRY_D3D11
	oiGraphicsDevice = new D3D11GraphicsDevice;
	if (oiGraphicsDevice->Create(true, aiWindow)) return;
	delete oiGraphicsDevice;
	oiGraphicsDevice = NULL;
#else
	oiGraphicsDevice = NULL;
	return;
#endif // FLX_TRY_D3D11
}
