// BismIllahIRRahmaanIRRaheem
/* graphics device loader implementation */

#include <graphics/device.h>
#include <runtime/rtdebug.h>
#ifdef FLX_TRY_D3D11
#include <d3d11device.h>
#endif // FLX_TRY_D3D11

namespace falx {
	class NullGraphicsDevice : public IGraphicsDevice {
	public:
		bit Create(bit, IWindow*) override { return true; }
		void BeginFrame() override {}
		void EndFrame() override {}
		void SetDepth(bit) override {}
		void Dismiss() override {}
	};
}

void falx::CreateGraphicsDevice(IGraphicsDevice*& oiGraphicsDevice, bit aAllowSoftwareRendering, IWindow* aiWindow)
{
	FLX_SMART_CHECK(aiWindow != NULL, "Invalid window for graphics device creation");
	falx::g_GraphicsDeviceAvailable = false;
	if (falx::g_GraphicsAPIAvailable) {
#ifdef FLX_TRY_D3D11
		oiGraphicsDevice = new D3D11GraphicsDevice;
		if (oiGraphicsDevice->Create(aAllowSoftwareRendering, aiWindow)) {
			falx::g_GraphicsDeviceAvailable = true;
			return;
		}
		delete oiGraphicsDevice;
#endif // FLX_TRY_D3D11
	}
	oiGraphicsDevice = new falx::NullGraphicsDevice;
	oiGraphicsDevice->Create(aAllowSoftwareRendering, aiWindow);
}

void falx::CreateGraphicsDevice(IGraphicsDevice*& oiGraphicsDevice, IWindow* aiWindow)
{
	FLX_SMART_CHECK(aiWindow != NULL, "Invalid window for graphics device creation");
	falx::g_GraphicsDeviceAvailable = false;
	if (falx::g_GraphicsAPIAvailable) {
#ifdef FLX_TRY_D3D11
		oiGraphicsDevice = new D3D11GraphicsDevice;
		if (oiGraphicsDevice->Create(true, aiWindow)) {
			falx::g_GraphicsDeviceAvailable = true;
			return;
		}
		delete oiGraphicsDevice;
#endif // FLX_TRY_D3D11
	}
	oiGraphicsDevice = new falx::NullGraphicsDevice;
	oiGraphicsDevice->Create(true, aiWindow);
}
