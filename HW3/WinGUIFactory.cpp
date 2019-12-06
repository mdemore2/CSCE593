#include "AbstractGUIFactory.h"
#include "WinGUIFactory.h"
#include "WinButton.h"

namespace HW3
{
	std::shared_ptr<WinGUIFactory> WinGUIFactory::New()
	{
		std::shared_ptr<WinGUIFactory> newFactory;
		return newFactory;
	}
	std::shared_ptr<Button> WinGUIFactory::createButton() const {
		return WinButton::New();
	}
	std::shared_ptr<Window> WinGUIFactory::createWindow() const {
		return  WinWindow::New();
	}
}