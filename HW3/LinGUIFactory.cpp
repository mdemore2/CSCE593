#include "AbstractGUIFactory.h"
#include "LinGUIFactory.h"
#include "LinButton.h"
#include "LinWindow.h"

namespace HW3
{
	static std::shared_ptr<LinGUIFactory> New()
	{
		std::shared_ptr<LinGUIFactory> newFactory;
		return newFactory;
	}
	std::shared_ptr<LinButton> createButton() {
		return LinButton::New();
	}
	std::shared_ptr<LinWindow> createWindow() {
		return  LinWindow::New();
	}
}