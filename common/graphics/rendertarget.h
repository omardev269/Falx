// BismIllahIRRahmaanIRRaheem
/* render target vclass decl */

#ifndef RENDERTARGET_H
#define RENDERTARGET_H
#include <vector>
#include <graphics/device.h>
namespace falx {
	struct changeablePixel {
		int2 Coord;
		color NewColor;
	};
	struct IRenderTarget {
		virtual bit Create(IGraphicsDevice* aiParentDevice, int2 aTextureSize) = 0;
		virtual void ChangePixels(std::vector<changeablePixel> aChangeablePixels) = 0;
		virtual void GetPixels(color*& oColors) = 0;
		virtual bit EnableCPURead() = 0;
		virtual void DisableCPURead() = 0;
		virtual void Resize(int2 aNewSize) = 0;
		virtual void Dismiss() = 0;
	};
}
#endif // !RENDERTARGET_H