#include "debug_stats.h"
#include <vector>
#include <atomic>
#include <assert.h>

#define NUM_PROFILE_SCOPES 4096

class DebugStats_Impl
{
public:
	DebugStats_Impl()
	{
		m_prof_scopes.resize(NUM_PROFILE_SCOPES);
		m_next_prof_scope_index = 0;
	}
	
	std::vector<ProfileScope>	m_prof_scopes;
	std::atomic<uint32_t>		m_next_prof_scope_index;
};

DebugStats::DebugStats() : m_impl(std::make_unique<DebugStats_Impl>())
{
	
}

DebugStats::~DebugStats()
{
}

DebugStats::DebugStats(DebugStats&& rhs)
{
	m_impl = std::move(rhs.m_impl);
}

DebugStats& DebugStats::operator=(DebugStats&& rhs)
{
	m_impl = std::move(rhs.m_impl);
	return *this;
}

ProfileScope * DebugStats::getProfileScope()
{
	assert(m_impl->m_next_prof_scope_index < (m_impl->m_prof_scopes.size()-1));
	return &m_impl->m_prof_scopes[m_impl->m_next_prof_scope_index++];
}

