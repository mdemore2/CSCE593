#include "Button.h"
#include "WinButton.h"
#include <iostream>
#include <string>
#include <memory>


namespace HW3
{
	std::shared_ptr<WinButton> WinButton::New()
	{
		auto newButton = new WinButton();
		return std::shared_ptr<WinButton>(newButton);
	}


	int WinButton::drawTopEdge(std::ostream& os) const
	{
		
		int width = this->getWidth();
		if (width > this->name.length() + 4)
		{
			width -= 4;
			width -= static_cast<int>(this->name.length());
		}
		else
		{
			width = 0;
		}
		os << "+-" << this->name;
		while (width > 0)
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
		width -= static_cast<int>(getStateStr().length()) + 2;
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
		width -= static_cast<int>(this->value.length()) + 2;
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