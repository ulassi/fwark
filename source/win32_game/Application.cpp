#include "Application.h"
#include "update_frame.h"

#include <core/thread.h>
#include <core/debug.h>
#include "render/resources.h"

#include <list>
#include <array>
#include <future>
#include <atomic>

#include <concurrent_queue.h>
using namespace std;

class Application_Impl
{
public:
	
	Application_Impl(unique_ptr<graphics::Device> dev);
	~Application_Impl();

	unique_ptr<graphics::Device>			m_graphics_device;
	unique_ptr<render::resource::Factory>	m_render_resource_factory;

	//=================================================================================
	// Signal variables
	atomic<bool>	m_quit;

	//=================================================================================
	// Frame update thread data
	typedef shared_ptr<packaged_task<PresentQueue()>> FrameUpdateTaskType;
	typedef FrameUpdateTaskType FrameUpdateQueueItem;
	typedef concurrency::concurrent_queue<FrameUpdateQueueItem> FrameUpdateTaskQueueType;
	FrameUpdateTaskQueueType	m_frame_update_tasks;
	condition_variable			m_frame_task_available;
	mutex						m_frame_task_available_mutex;
	thread						m_frame_update_thread_worker;

	//! Handle lifetime and reuses of frame contexts
	shared_ptr<FrameUpdateContext>	aquireFrameContext(core::frame_t, InputState);
	void							releaseFrameContext(shared_ptr<FrameUpdateContext>);
	concurrency::concurrent_queue<shared_ptr<FrameUpdateContext>> m_used_contexts;

	void notify_frame_update_worker();
	static void update_task_worker_function(
		std::mutex& lock, 
		std::condition_variable& var,
		FrameUpdateTaskQueueType& task_queue,
		std::atomic<bool>& quitting );
	//=================================================================================
};


//=================================================================================
// IMPLEMENTATION
Application_Impl::Application_Impl(std::unique_ptr<graphics::Device> dev)
:	m_graphics_device(std::move(dev)),
	m_quit(false),
	m_frame_update_thread_worker(&Application_Impl::update_task_worker_function, std::ref(m_frame_task_available_mutex), std::ref(m_frame_task_available), std::ref(m_frame_update_tasks), std::ref(m_quit)),
	m_render_resource_factory(render::resource::Factory::make_unique())
{
}
Application_Impl::~Application_Impl()
{
	m_quit.store(true);
	notify_frame_update_worker(); // to ensure it can return safely
	m_frame_update_thread_worker.join();
}

shared_ptr<FrameUpdateContext>	Application_Impl::aquireFrameContext(core::frame_t n, InputState input)
{
	shared_ptr<FrameUpdateContext> ctx;
	if (m_used_contexts.try_pop(ctx))
	{
		ctx->frame_number = n;
		ctx->input_state = input;
	}
	else
	{
		ctx = make_shared<FrameUpdateContext>(n);
		ctx->input_state = input;
		m_used_contexts.push(ctx);
	}
	return ctx;
}

void Application_Impl::releaseFrameContext(shared_ptr<FrameUpdateContext>)
{

}


//=================================================================================
// INTERFACE

Application::Application(std::unique_ptr<graphics::Device> dev)
	:	m_impl(std::make_unique<Application_Impl>(std::move(dev)))
{
}

Application::~Application()
{
}

void Application::present(PresentQueue& q)
{
	// execute present queue in optimal fashion through graphics api
	m_impl->m_graphics_device->begin_frame();
	m_impl->m_graphics_device->clear(q.color.x(), q.color.y(), q.color.z());
	m_impl->m_graphics_device->end_frame();
}

InputState Application::get_input()
{
	return InputState();
}

PresentQueue function()
{
	void* a = nullptr;
	return PresentQueue();
}

std::future<PresentQueue> Application::update_frame(core::frame_t frame, InputState input)
{
	using namespace std;
	shared_ptr<FrameUpdateContext> context = m_impl->aquireFrameContext(frame, input);
	auto func = [=]()->PresentQueue
	{
		auto pq = std::move(UpdateFrameFunction::update(*context));
		m_impl->releaseFrameContext(context);
		return pq;
	};
	auto update_task = std::make_shared<std::packaged_task<PresentQueue()>>(std::move(func));
	auto f = update_task->get_future();
	m_impl->m_frame_update_tasks.push(update_task);
	m_impl->notify_frame_update_worker();
	return std::move( f );
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

void Application_Impl::notify_frame_update_worker()
{
	std::lock_guard<std::mutex> lock(m_frame_task_available_mutex);
	m_frame_task_available.notify_one();
}

void Application_Impl::update_task_worker_function(
	std::mutex & var_mutex, 
	std::condition_variable & var, 
	FrameUpdateTaskQueueType & task_queue,
	std::atomic<bool>& should_quit )
{
	core::thread::name_current_thread("frame_update_worker");
	while (!should_quit)
	{
		// check for tasks in queue...
		{
			FrameUpdateQueueItem task;
			while (task_queue.try_pop(task))
			{
				// .. run them .. 
				(*task)();
			}
		}

		// ... if no tasks, rest until signalled
		std::unique_lock<std::mutex> lock(var_mutex);
		var.wait(lock, [&]()
		{
			// stop waiting if quit was signalled or more work is there
			return should_quit.load() || (!task_queue.empty());
		});
	}
}
