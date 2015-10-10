#pragma once

#include <graphics/device.h>
#include <memory>

class Application_Impl;
class Application
{
public:
	Application(std::unique_ptr<graphics::Device>);
	~Application();
private:
	std::unique_ptr<Application_Impl> m_impl;
};
