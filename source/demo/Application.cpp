#include "Application.h"


class Application_Impl
{
public:
	Application_Impl(std::unique_ptr<graphics::Device> dev)
	:	m_graphics_device(std::move(dev))
	{}
	~Application_Impl(){}
private:
	std::unique_ptr<graphics::Device> m_graphics_device;
};

Application::Application(std::unique_ptr<graphics::Device> dev)
	:	m_impl(std::make_unique<Application_Impl>(std::move(dev)))
{
}

Application::~Application()
{
}
