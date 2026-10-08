// BismIllahIRRahmaanIRRaheem
/* profiler header */

#ifndef PROFILER_FLX_H
#define PROFILER_FLX_H
#include <runtime/enginedef.h>
#include <runtime/updateable.h>
namespace falx {
	// General profiler - not only for the main thread
	// Each frame does not mean graphical frame rather a computational frame (though yes for the main thread this must be per graphical frame)
	// as this is made for real-time stuff in general
	class Profiler : public IUpdateable {
		largefloat m_LastFrameTime;
		largefloat m_Delta;
		uint32 m_FPS;
		bit m_FirstFrame;
	public:
		// to be ran by the user - initializes the profiler
		void Initialize();
		void BeginFrame() override;
		void EndFrame() override;
		// throws an exception if no frame was ran
		void GetDelta(largefloat& oDelta);
		// throws an exception if no frame was ran
		void GetFPS(uint32& oFPS);
		// gets if this is the first frame
		bool IsFirstFrame();
		void Dismiss() override;
	};
	// does NOT run Initialize
	void CreateProfiler(Profiler*& oiProfiler);
}
#endif // !PROFILER_FLX_H