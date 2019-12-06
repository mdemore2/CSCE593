#include "AbstractGUIFactory.h"
#include "LinGUIFactory.h"
#include <memory>

#include "LinButton.h"
#include "LinWindow.h"
//factory pattern, uses abstract factory and makes concrete gui using concrete windows and buttons for specific os

namespace HW3
{
	std::shared_ptr<LinGUIFactory> LinGUIFactory::New()
	{
		auto newFactory = new LinGUIFactory();
		return std::shared_ptr<LinGUIFactory>(newFactory);
	}
	std::shared_ptr<Button> LinGUIFactory::createButton() {
		return std::move(LinButton::New());
	}
	std::shared_ptr<Window> LinGUIFactory::createWindow() {
		return  std::move(LinWindow::New());
	}
}