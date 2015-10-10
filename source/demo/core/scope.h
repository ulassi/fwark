#pragma once
#include <functional>

namespace core
{
	class OnExitScope
	{
		std::function<void()> m_functor;
	public:
		explicit OnExitScope(std::function<void()> f) : m_functor(std::move(f))
		{}

		~OnExitScope()
		{
			m_functor();
		}
	};
}