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
	std::shared_ptr<WinButton> createButton() {
		return WinButton::New();
	}
	std::shared_ptr<WinWindow> createWindow() {
		return  WinWindow::New();
	}
}