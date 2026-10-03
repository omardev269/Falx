// BismIllahIRRahmaanIRRaheem
/* D3D11 gbuffer decl */

#ifndef D3D11GBUFFER_FLX_H
#define D3D11GBUFFER_FLX_H
#include <graphics/graphicsdef.h>
#ifdef FLX_TRY_D3D11
#include <d3d11.h>
namespace falx {
	struct GBufferD3D11 {
		ID3D11Texture2D* i_Texture;
		ID3D11RenderTargetView* i_RenderTargetView;
		ID3D11ShaderResourceView* i_ShaderResourceView;
	};
}
#endif // FLX_TRY_D3D11
#endif // !D3D11GBUFFER_FLX_H