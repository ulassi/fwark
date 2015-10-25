#pragma once

#include <core/types.h>
#include <core/vec3.h>
#include <graphics/device.h>

#include "present_queue.h"
#include "input_state.h"

#include <memory>
#include <future>

class Application_Impl;
class Application
{
public:
	Application(std::unique_ptr<graphics::Device>);
	~Application();
	void present(PresentQueue);

	//! Poll the current input state and return it
	InputState get_input();

	//! Start frame update for given frame and return a future to present q for that frame
	std::future<PresentQueue> update_frame(core::frame_t, InputState);

	static void app_thread(std::weak_ptr<Application>);
private:
	std::unique_ptr<Application_Impl> m_impl;
};
