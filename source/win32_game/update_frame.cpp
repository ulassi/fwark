#include "update_frame.h"

PresentQueue UpdateFrameFunction::update(FrameUpdateContext context)
{
	using namespace core;
	static std::array<vec3, 3> bg_colors = {
		vec3{ 1.f,0.f,0.f }, vec3{ 0.f,1.f,0.f }, vec3(0.f,0.f,1.f)
	};
	PresentQueue pq(context.frame_number, bg_colors[context.frame_number%bg_colors.size()]);
	return std::move(pq);
}
