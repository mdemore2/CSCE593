#include "Button.h"
#include "WinButton.h"

namespace HW3
{
	static std::shared_ptr<WinButton> New()
	{
		std::shared_ptr<WinButton> newButton;
		return newButton;
	}

	

	int Button::getWidth() const
	{
		return this->width;
	}
	int Button::getHeight() const
	{
		return this->height;
	}

	int Button::drawTopEdge(std::ostream& os) const
	{
	}
	int Button::drawState(std::ostream& os) const
	{
	}
	int Button::drawValue(std::ostream& os) const
	{
	}
	int Button::drawBottomEdge(std::ostream& os) const
	{
	}
}