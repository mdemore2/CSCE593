#include "Window.h"
#include "LinWindow.h"

namespace HW3
{
	std::shared_ptr<LinWindow> LinWindow::New()
	{
		auto newWindow = new LinWindow();
		return std::shared_ptr<LinWindow>(newWindow);
	}
	void LinWindow::setName(const std::string& id)
	{
		std::string newname = "Lindow: " + id;
		this->name = id;
	}


	/*void Window::addButton(std::shared_ptr<Button> button)
	{
		this->buttons.push_back(button);
	}*/
	void LinWindow::draw(std::ostream& os) const
	{
		int height = 7;
		int width = 0;
		width = static_cast<int>(this->name.length()) + 5;
		int altwidth = 0;
		for (int i = 0; i < buttons.size(); i++)
		{
			std::shared_ptr<Button> button = buttons.at(i);
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
				os << "<=" << this->name;
				linecount += static_cast<int>(this->name.length()) + 2;
				while (linecount < width)
				{
					os << "=";
					linecount++;
				}
				os << ">";

			}
			if (height == 6)
			{
				os << "[";
				linecount++;
				while (linecount < width)
				{
					os << " ";
					linecount++;
				}
				os << "]";

			}
			if (height == 5)
			{
				os << "[";
				linecount++;
				for (int i = 0; i < buttons.size(); i++)
				{
					int factor = buttons.at(i)->drawTopEdge(os);
					linecount += factor;
				}
				while (linecount < width)
				{
					os << " ";
				}
				os << "]";
			}
			if (height == 4)
			{
				os << "[";
				for (int i = 0; i < buttons.size(); i++)
				{
					int factor = buttons.at(i)->drawState(os);
					linecount += factor;
				}
				while (linecount < width)
				{
					os << " ";
				}
				os << "]";
			}
			if (height == 3)
			{
				os << "[";
				linecount++;
				for (int i = 0; i < buttons.size(); i++)
				{
					int factor = buttons.at(i)->drawValue(os);
					linecount += factor;
				}
				while (linecount < width)
				{
					os << " ";
				}
				os << "]";
			}
			if (height == 2)
			{
				os << "[";
				linecount++;
				for (int i = 0; i < buttons.size(); i++)
				{
					int factor = buttons.at(i)->drawBottomEdge(os);
					linecount += factor;
				}
				while (linecount < width)
				{
					os << " ";
				}
				os << "]";
			}
			if (height == 1)
			{
				os << "<";
				linecount++;
				while (linecount < width)
				{
					os << "=";
					linecount++;
				}
				os << ">";
			}

			height--;
			linecount = 0;
		}

	}

}