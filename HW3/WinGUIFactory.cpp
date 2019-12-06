#include "AbstractGUIFactory.h"
#include "WinGUIFactory.h"

namespace HW3
{
	static std::shared_ptr<WinGUIFactory> New()
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