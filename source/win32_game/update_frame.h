#pragma once
#include "present_queue.h"
#include "input_state.h"
#include "debug_stats.h"

#include <memory>

class FrameUpdateContext
{
public:
	InputState		input_state;
	core::frame_t	frame_number;

	DebugStats debug_stats;

	FrameUpdateContext(core::frame_t);
	FrameUpdateContext(FrameUpdateContext&&);
	FrameUpdateContext& operator=(FrameUpdateContext&&);
};

class UpdateFrameFunction
{
public:
	static PresentQueue update(FrameUpdateContext&);
};