// BismIllahIRRahmaanIRRaheem
/* core pipeline declarations */

// What is a Graphics Pipeline:
// An IGraphicsPipeline is an abstraction over specific pipelines for different GFX APIs
// (albedo only vs. basic lighting - vs. shadowed lighting - D3D11 vs OpenGL - et cetera)
// It is a collection of GBuffers and shaders that the imp of a object holder gives a set
// of buffers (ie.. index buffer + vertex buffer) to
// The abstraction was meant to sustain cross-platformability and allow for different
// pipelines to be used (examples listed above) without having to change the object holder
// or scene properties. Also meant to make 2 scenes render at once separately

#ifndef FLX_COREGFXPIPELINE_H
#define FLX_COREGFXPIPELINE_H
#include <graphics/device.h>
namespace falx {
	struct IGraphicsPipeline {
		virtual bit Create(IGraphicsDevice* aiRegistoire) = 0;
		virtual void PushToGraphicsDevice() = 0;
		virtual void Dismiss() = 0;
	};
	void CreateAlbedoGraphicsPipeline(IGraphicsPipeline*& oiPipeline, IGraphicsDevice* aiRegistoire);
}
#endif // !FLX_COREGFXPIPELINE_H