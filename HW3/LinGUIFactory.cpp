#include "AbstractGUIFactory.h"
#include "LinGUIFactory.h"

namespace HW3
{
	static std::shared_ptr<LinGUIFactory> New()
	{
		std::shared_ptr<LinGUIFactory> newFactory;
		return newFactory;
	}
	
}