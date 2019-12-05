#include "Button.h"
#include "LinButton.h"

namespace HW3
{
	static std::shared_ptr<LinButton> New()
	{
		std::shared_ptr<LinButton> newButton;
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