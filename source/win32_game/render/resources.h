#pragma once
#include <memory>

namespace render
{
	namespace resource
	{
		class Factory_Impl;
		/*! A resource factory enables creation of resources needed for rendering
		*/
		class Factory
		{
		public:
			static std::unique_ptr<Factory> make_unique();
			~Factory();
		protected:
			Factory();
		private:
			std::unique_ptr<Factory_Impl> m_impl;
		};
	}
}