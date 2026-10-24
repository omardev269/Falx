// BismIllahIRRahmaanIRRaheem
/* realtime profiler */

// This file was made by AI

#include <runtime/profiler.h>
#include <runtime/time.h>
#include <runtime/rtdebug.h>

void falx::Profiler::Initialize()
{
	m_LastFrameTime = GetTimeNotSinceFalxStartup();
	m_Delta = 0;
	m_SecondTimer = 0;
	m_TFPS = 0;
	m_FPS = 0;
	m_FirstFrame = true;
}

void falx::Profiler::BeginFrame()
{
	m_LastFrameTime = GetTimeNotSinceFalxStartup();
}

void falx::Profiler::EndFrame()
{
	largefloat m_Now = GetTimeNotSinceFalxStartup();
	m_Delta = m_Now - m_LastFrameTime;
	m_TFPS++;
	m_SecondTimer += m_Delta;
	if (m_SecondTimer >= 1.0f)  
	{
		m_FPS = m_TFPS;
		m_TFPS = 0;
		m_SecondTimer -= 1.0f;
	}
	m_FirstFrame = false;
}

void falx::Profiler::GetDelta(largefloat& oDelta)
{
	FLX_ASSERT(!m_FirstFrame, "No frame was ran before querying the profiler delta");
	oDelta = m_Delta;
}

void falx::Profiler::GetFPS(uint32& oFPS)
{
	FLX_ASSERT(!m_FirstFrame, "No frame was ran before querying the profiler FPS");
	oFPS = m_FPS;
}

bool falx::Profiler::IsFirstFrame()
{
	return m_FirstFrame;
}

void falx::Profiler::Dismiss()
{

}

void falx::CreateProfiler(Profiler*& oiProfiler)
{
	oiProfiler = new Profiler;
}