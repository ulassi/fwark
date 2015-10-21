#pragma once
#include "present_queue.h"
#include "input_state.h"

#include <memory>

struct FrameUpdateContext
{
	InputState		input_state;
	core::frame_t	frame_number;
};

class UpdateFrameFunction
{
public:
	static PresentQueue update(FrameUpdateContext);
};