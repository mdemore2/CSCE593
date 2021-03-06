#include "Window.h"
#include "WinWindow.h"
#include "Button.h"
#include "WinButton.h"
#include<memory>

namespace HW3
{

	std::shared_ptr<WinWindow> WinWindow::New()
	{
		auto newWindow = new WinWindow();
		return std::shared_ptr<WinWindow>(newWindow);
	}

	void WinWindow::setName(const std::string& id)
	{
		std::string newname = "WinWindow: " + id;
		this->name = newname;
	}
	void Window::setName(const std::string& id)
	{
		this->name = id;
	}
	void Window::addButton(std::shared_ptr<Button> button) 
	{
		this->buttons.push_back(button);
	}
	void WinWindow::draw(std::ostream& os) const
	{
		int height = 7;
		int width = 0;
		width = static_cast<int>(this->name.length()) + 5;
		int altwidth = 0;
		for (int i = 0; i < buttons.size(); i++)
		{
			std::shared_ptr<Button> button = buttons[i];
			altwidth += button->getWidth();
		}
		if (altwidth > width)
		{
			width = altwidth;
		}
		int linecount = 0;
		while (height > 0)
		{
			if (height == 7)
			{
				os << "+---" << this->name;
				linecount += static_cast<int>(this->name.length()) + 4;
				while (linecount < width)
				{
					os << "-";
					linecount++;
				}
				os << "+";
				
			}
			if (height == 6)
			{
				os << "|";
				linecount++;
				while (linecount < width)
				{
					os << " ";
					linecount++;
				}
				os << "|";
				
			}
			if (height == 5)
			{
				os << "|";
				linecount++;
				for (int i = 0; i < static_cast<int>(buttons.size()); i++)
				{
					int factor = buttons[i]->drawTopEdge(os);
					linecount += factor;
				}
				while (linecount < width)
				{
					os << " ";
					linecount++;
				}
				os << "|";
			}
			if (height == 4)
			{
				os << "|";
				linecount++;
				for (int i = 0; i < static_cast<int>(buttons.size()); i++)
				{
					int factor = buttons[i]->drawState(os);
					linecount += factor;
				}
				while (linecount < width)
				{
					os << " ";
					linecount++;
				}
				os << "|";
			}
			if (height == 3)
			{
				os << "|";
				linecount++;
				for (int i = 0; i < buttons.size(); i++)
				{
					int factor = buttons[i]->drawValue(os);
					linecount += factor;
				}
				while (linecount < width)
				{
					os << " ";
					linecount++;
				}
				os << "|";
			}
			if (height == 2)
			{
				os << "|";
				linecount++;
				for (int i = 0; i < buttons.size(); i++)
				{
					int factor = buttons[i]->drawBottomEdge(os);
					linecount += factor;
				}
				while (linecount < width)
				{
					os << " ";
					linecount++;
				}
				os << "|";
			}
			if (height == 1)
			{
				os << "+";
				linecount++;
				while (linecount < width)
				{
					os << "-";
					linecount++;
				}
				os << "+";
			}
			os << "\n";
			height--;
			linecount = 0;
		}
	}
}