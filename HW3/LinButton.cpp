#include "Button.h"
#include "LinButton.h"
#include <iostream>
#include <string>

namespace HW3
{
	static std::shared_ptr<LinButton> New()
	{
		std::shared_ptr<LinButton> newButton;
		return newButton;
	}




	int LinButton::drawTopEdge(std::ostream& os) const
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
		os << "<=" << this->name;
		while (width < 0)
		{
			os << "=";
			width--;
		}
		os << "=>";
		return getWidth();
	}
	int LinButton::drawState(std::ostream& os) const
	{
		int width = getWidth();
		width -= getStateStr().length() + 2;
		os << "[";
		while (width > 0)
		{
			os << " ";
			width--;
		}

		os << getStateStr() << "]";
		return getWidth();
	}
	int LinButton::drawValue(std::ostream& os) const
	{
		int width = getWidth();
		width -= this->value.length() + 2;
		os << "[";
		while (width > 0)
		{
			os << " ";
			width--;
		}

		os << getStateStr() << "]";
		return getWidth();

		return getWidth();
	}
	int LinButton::drawBottomEdge(std::ostream& os) const
	{
		int width = getWidth() - 2;
		os << "<";
		while (width > 0)
		{
			os << "=";
			width--;
		}
		os << ">";
		return getWidth();
	}
}