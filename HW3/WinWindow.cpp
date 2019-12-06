#include "Window.h"
#include "WinWindow.h"
#include "Button.h"
#include "WinButton.h"

namespace HW3
{

	static std::shared_ptr<WinWindow> New()
	{
		std::shared_ptr<WinWindow> newWindow;
		return newWindow;
	}

	void Window::setName(const std::string& id)
	{
		this->name = id;
	}

	int WinWindow::addButton(std::shared_ptr<WinButton> button) const
	{
		this->buttons.push_back(button);
	}
	int WinWindow::draw(std::ostream& os) const
	{
		//if window name bigger than width of all buttons, no extra char before corner
		//else fill to fit one beyond width of all buttons
	}
}