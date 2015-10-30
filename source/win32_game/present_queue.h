#pragma once

#include <core/vec3.h>
#include <core/types.h>

class PresentQueue
{
public:
	PresentQueue() : frame(0), color{ 0.f } {};
	PresentQueue(core::frame_t f, core::vec3 v) : frame(f), color(std::move(v)) {}
	PresentQueue(PresentQueue&&) = default;
	PresentQueue& operator = (PresentQueue&&) = default;
	PresentQueue(const PresentQueue&) = default;
	PresentQueue& operator = (const PresentQueue&) = default;

	core::frame_t frame;
	core::vec3 color;
};