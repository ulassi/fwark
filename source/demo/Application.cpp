#include "Application.h"
#include <core/thread.h>
#include <list>
#include <array>

class Application_Impl
{
public:
	Application_Impl(std::unique_ptr<graphics::Device> dev)
	:	m_graphics_device(std::move(dev))
	{}
	~Application_Impl(){}
//private:
	std::unique_ptr<graphics::Device> m_graphics_device;

	std::list<std::shared_ptr<std::promise<PresentQueue>>> m_frame_updates;
};

Application::Application(std::unique_ptr<graphics::Device> dev)
	:	m_impl(std::make_unique<Application_Impl>(std::move(dev)))
{
}

Application::~Application()
{
}

void Application::present(PresentQueue q)
{
	// negotiate state from application sim, draw that state
	m_impl->m_graphics_device->begin_frame();
	m_impl->m_graphics_device->clear(q.color.x(), q.color.y(), q.color.z());
	m_impl->m_graphics_device->end_frame();
}

InputState Application::get_input()
{
	return InputState();
}

std::future<PresentQueue> Application::update_frame(core::frame_t frame, InputState)
{
	using namespace core;
	static std::array<vec3, 3> bg_colors = {
		vec3{1.f,0.f,0.f}, vec3{0.f,1.f,0.f}, vec3(0.f,0.f,1.f)
	};
	std::shared_ptr<std::promise<PresentQueue>> p;
	if (m_impl->m_frame_updates.size() > 3)
	{
		m_impl->m_frame_updates.pop_front();
	}
	p = std::make_shared<std::promise<PresentQueue>>();

	// just set the state right away for now, this should be async of course
	p->set_value(std::move(PresentQueue(frame, bg_colors[frame%bg_colors.size()])));
	m_impl->m_frame_updates.push_back(p);
	return std::move( p->get_future() );
}

void Application::app_thread(std::weak_ptr<Application> app)
{
	core::thread::name_current_thread("application");
	std::list<std::future<PresentQueue>> frame_q_list;
	core::frame_t current_frame = 1;
	for (;;)
	{
		auto app_ptr = app.lock();
		if (app_ptr)
		{
			// poll input, kick off game update for frame
			frame_q_list.push_back(std::move(app_ptr->update_frame(current_frame, app_ptr->get_input())));

			if (frame_q_list.size() == 1)
			{
				//first frame, so we have nothing to present, so lets just run to next frame
				// and schedule another frame update
			}
			else
			{
				// block until oldest frame is done and present that
				app_ptr->present(frame_q_list.front().get());
				frame_q_list.pop_front(); // drop the presented queue
			}
			current_frame++;
		}
		else
		{
			break;
		}
	}
}