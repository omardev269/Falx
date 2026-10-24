// BismIllahIRRahmaanIRRaheem
/* SDL3 window implementation declaration */
#ifndef FLX_SDL3WINDOW_H
#define FLX_SDL3WINDOW_H
#include <runtime/window.h>
#include <SDL3/SDL.h>
#ifdef FLX_ALLOW_IMGUI
#include <imgui.h>
#endif // FLX_ALLOW_IMGUI
namespace falx {

	class SDL3Window : public IWindow {
		SDL_Window* p_Window;
		bool m_ShouldClose;
		float2 m_MousePos;
		float2 m_MouseDelta;
		float2 m_UIMousePos = {};
		bit m_Concentrated = true;
		#ifdef FLX_ALLOW_IMGUI
				ImGuiContext* p_ImGuiContext;
		#endif // FLX_ALLOW_IMGUI
	public:
		bool Create(const char* aTitle, int2 aWindowSize) override;
		bool ShouldClose() override;
		bool GetKey(Key aKey) override;
		bool GetMouseButton(MouseButton aButton) override;
		void GetMouseDelta(float2& oDelta) override;
		void GetMousePos(float2& oPos) override;
		bool GetKeyUI(Key aKey) override;
		bool GetMouseButtonUI(MouseButton aButton) override;
		void GetMouseDeltaUI(float2& oDelta) override;
		void GetMousePosUI(float2& oPos) override;
		void ConcentrateMouse(bit aConcentrate) override;
		void* GetWindowsIdentifier() override; 
		#ifdef FLX_ALLOW_IMGUI
		ImGuiContext* GetImGuiContext() override;
		void ImGuiImplStart() override;
#endif // FLX_ALLOW_IMGUI
		void BeginFrame() override;
		void EndFrame() override;
		void Dismiss() override;
	};
}
#endif // !FLX_SDL3WINDOW_H
