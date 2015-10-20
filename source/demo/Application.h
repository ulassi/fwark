#pragma once

#include <core/types.h>
#include <core/vec3.h>
#include <graphics/device.h>
#include <memory>
#include <future>


struct PresentQueue
{
	PresentQueue() : frame(0), color{0.f} {};
	PresentQueue(core::frame_t f, core::vec3 v) : frame(f), color(std::move(v)){}
	PresentQueue(PresentQueue&&) = default;
	PresentQueue& operator = (PresentQueue&&) = default;
	core::frame_t frame;
	core::vec3 color;
};

struct InputState
{
	uint32_t mouse_x = 0, mouse_y = 0;
};

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
