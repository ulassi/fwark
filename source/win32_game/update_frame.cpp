#include "update_frame.h"
#include "debug_stats.h"

void debug_draw(PresentQueue& pq)
{

}

PresentQueue UpdateFrameFunction::update(FrameUpdateContext& context)
{
	using namespace core;
	static std::array<vec3, 3> bg_colors = {
		vec3{ 0.2f,0.2f,0.2f }, vec3{ 0.2f,0.3f,0.f }, vec3(0.2f,0.2f,0.3f)
	};
	PresentQueue pq(context.frame_number, bg_colors[context.frame_number%bg_colors.size()]);
	{
		TIMING_SCOPE(context.debug_stats);
		
		Sleep(10);
	}
	
	return std::move(pq);
}

FrameUpdateContext::FrameUpdateContext(core::frame_t frame)
:	frame_number(frame)
{

}

FrameUpdateContext::FrameUpdateContext(FrameUpdateContext && rhs)
:	frame_number(rhs.frame_number),
	input_state(std::move(rhs.input_state)),
	debug_stats(std::move(rhs.debug_stats))
{
}

FrameUpdateContext& FrameUpdateContext::operator=(FrameUpdateContext&& rhs)
{
	frame_number = rhs.frame_number;
	input_state = std::move(rhs.input_state);
	debug_stats = std::move(rhs.debug_stats);
	return *this;
}
