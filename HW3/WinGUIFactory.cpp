#include "AbstractGUIFactory.h"
#include "WinGUIFactory.h"
#include <memory>

#include "WinButton.h"
//factory pattern, uses abstract factory and makes concrete gui using concrete windows and buttons for specific os

namespace HW3
{
	std::shared_ptr<WinGUIFactory> WinGUIFactory::New()
	{
		auto newFactory = new WinGUIFactory();
		return std::shared_ptr<WinGUIFactory>(newFactory);
	}
	std::shared_ptr<Button> WinGUIFactory::createButton() const {
		return std::move(WinButton::New());
	}
	std::shared_ptr<Window> WinGUIFactory::createWindow() const {
		return  std::move(WinWindow::New());
	}
}