// BismIllahIRRahmaanIRRaheem
/* window creation implementation */
#include <runtime/window.h>
#include <runtime/rtdebug.h>
#include <SDL3Window.h>

namespace falx {
	class NullWindow : public IWindow {
	public:
		bool Create(const char*, int2) override { return true; }
		void* GetWindowsIdentifier() override { return nullptr; }
		#ifdef FLX_ALLOW_IMGUI
		void ImGuiImplStart() override {}
		ImGuiContext* GetImGuiContext() override { return nullptr; }
		#endif // FLX_ALLOW_IMGUI
		bool ShouldClose() override { return true; }
		bool GetKey(Key) override { return false; }
		bool GetMouseButton(MouseButton) override { return false; }
		void GetMouseDelta(float2& oDelta) override { oDelta = {}; }
		void GetMousePos(float2& oPos) override { oPos = {}; }
		void BeginFrame() override {}
		void EndFrame() override {}
		void Dismiss() override {}
	};

	void CreateWindowNW(IWindow*& aWindow, const char* aTitle, int2 aWindowSize)
	{
		SDL3Window* m_Window = new SDL3Window();
		if (!m_Window->Create(aTitle, aWindowSize)) {
			delete m_Window;
			aWindow = new NullWindow();
			aWindow->Create(aTitle, aWindowSize);
			return;
		}
		aWindow = m_Window;
	}
}
