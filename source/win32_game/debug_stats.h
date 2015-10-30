#pragma once
#include <memory>
#include <intrin.h>
#include <windows.h>

struct ProfileScope
{
	uint64_t	cycles;
	const char* func;
	const char* file;
	uint32_t	line;
	uint32_t	thread;
};

//! Per frame structure for keeping track of profiling stats etc
class DebugStats_Impl;
class DebugStats
{
public:
	DebugStats();
	~DebugStats();

	DebugStats(DebugStats&&);
	DebugStats& operator=(DebugStats&&);

	//! Makes a new entry into the profiling database and retursn ptr to it
	// this function is thread safe
	ProfileScope* getProfileScope();
private:
	std::unique_ptr<DebugStats_Impl> m_impl;
};

struct TimingScope
{
	ProfileScope* scope;
	TimingScope(DebugStats& stats, const char* in_file, const char* in_function, uint32_t in_line )
	{
		scope = stats.getProfileScope();
		scope->cycles = __rdtsc();
		scope->func = in_function;
		scope->file = in_file;
		scope->line = in_line;
		scope->thread = GetCurrentThreadId();
	}
	~TimingScope()
	{
		scope->cycles = __rdtsc() - scope->cycles;
	}
};

#define TIMING_SCOPE(stats) TimingScope __scoped_timing(stats, __FILE__, __FUNCTION__, __LINE__);