#include "AbstractGUIFactory.h"
#include "WinGUIFactory.h"
#include <memory>

#include "WinButton.h"

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