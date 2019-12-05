#include "Button.h"
#include "WinButton.h"
#include <iostream>
#include <string>

namespace HW3
{
	static std::shared_ptr<WinButton> New()
	{
		std::shared_ptr<WinButton> newButton;
		return newButton;
	}


	int WinButton::drawTopEdge(std::ostream& os) const
	{
		int width = this->getWidth();
		if (width > this->name.length() + 4)
		{
			width -= 4;
			width -= this->name.length();
		}
		else
		{
			width = 0;
		}
		os << "+-" << this->name;
		while (width < 0)
		{
			os << "-";
			width--;
		}
		os << "-+";
		return getWidth();
	}
	int WinButton::drawState(std::ostream& os) const
	{
		int width = getWidth();
		width -= getStateStr().length() + 2;
		os << "|";
		while (width > 0)
		{
			os << " ";
			width--;
		}

		os << getStateStr() << "|";
		return getWidth();
	}
	int WinButton::drawValue(std::ostream& os) const
	{
		int width = getWidth();
		width -= this->value.length() + 2;
		os << "|";
		while (width > 0)
		{
			os << " ";
			width--;
		}

		os << getStateStr() << "|";
		return getWidth();

		return getWidth();
	}
	int WinButton::drawBottomEdge(std::ostream& os) const
	{
		int width = getWidth() - 2;
		os << "+";
		while (width > 0)
		{
			os << "-";
			width--;
		}
		os << "+";
		return getWidth();
	}
}