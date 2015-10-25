#include "resources.h"

namespace render
{
namespace resource
{

class Factory_Impl
{
public:

};

struct _Factory : public Factory
{
	_Factory() : Factory() {}
};

std::unique_ptr<Factory> Factory::make_unique()
{
	return std::make_unique<_Factory>();
}

Factory::~Factory()
{
}

Factory::Factory() : m_impl(std::make_unique<Factory_Impl>())
{
}

}
}