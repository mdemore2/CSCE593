#include "Window.h"
#include "LinWindow.h"

namespace HW3
{
	static std::shared_ptr<LinWindow> New()
	{
		std::shared_ptr<LinWindow> newWindow;
		return newWindow;
	}
	void Window::setName(const std::string& id)
	{
		this->name = id;
	}


	int LinWindow::addButton(std::shared_ptr<LinButton> button) const
	{
		this->buttons.emplace_back(std::move(button));
	}
	int LinWindow::draw(std::ostream& os) const
	{

	}

}