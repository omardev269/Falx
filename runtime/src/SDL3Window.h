// BismIllahIRRahmaanIRRaheem
/* SDL3 window implementation declaration */
#ifndef FLX_SDL3WINDOW_H
#define FLX_SDL3WINDOW_H
#include <runtime/window.h>
#include <SDL3/SDL.h>
namespace falx {

	class SDL3Window : public IWindow {
		SDL_Window* p_Window;
		bool m_ShouldClose;
		float2 m_MousePos;
		float2 m_MouseDelta;
	public:
		bool Create(const char* aTitle, int2 aWindowSize) override;
		bool ShouldClose() override;
		bool GetKey(Key aKey) override;
		bool GetMouseButton(MouseButton aButton) override;
		void GetMouseDelta(float2& oDelta) override;
		void GetMousePos(float2& oPos) override;
		void* GetWindowsIdentifier() override; 
		void BeginFrame() override;
		void EndFrame() override;
		void Dismiss() override;
	};
}
#endif // !FLX_SDL3WINDOW_H
