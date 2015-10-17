#include "Application.h"


class Application_Impl
{
public:
	Application_Impl(std::unique_ptr<graphics::Device> dev)
	:	m_graphics_device(std::move(dev))
	{}
	~Application_Impl(){}
//private:
	std::unique_ptr<graphics::Device> m_graphics_device;
};

Application::Application(std::unique_ptr<graphics::Device> dev)
	:	m_impl(std::make_unique<Application_Impl>(std::move(dev)))
{
}

Application::~Application()
{
}

void Application::draw()
{
	// negotiate state from application sim, draw that state
	m_impl->m_graphics_device->begin_frame();
	m_impl->m_graphics_device->clear(1.f, 0.5f, 1.f);
	m_impl->m_graphics_device->end_frame();
}