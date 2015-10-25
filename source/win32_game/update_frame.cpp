#include "update_frame.h"

void debug_draw(PresentQueue& pq)
{

}

PresentQueue UpdateFrameFunction::update(FrameUpdateContext context)
{
	using namespace core;
	static std::array<vec3, 3> bg_colors = {
		vec3{ 0.2f,0.2f,0.2f }, vec3{ 0.2f,0.3f,0.f }, vec3(0.2f,0.2f,0.3f)
	};
	PresentQueue pq(context.frame_number, bg_colors[context.frame_number%bg_colors.size()]);
	return std::move(pq);
}
