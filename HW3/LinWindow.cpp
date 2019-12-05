#include "Window.h"
#include "LinWindow.h"

namespace HW3
{
	static std::shared_ptr<LinWindow> New()
	{
		std::shared_ptr<LinWindow> newWindow;
		return newWindow;
	}

	int LinWindow::addButton(std::shared_ptr<LinButton> button) const
	{
		this->buttons.push_back(button);
	}
	int LinWindow::draw(std::ostream& os)
	{

	}

}