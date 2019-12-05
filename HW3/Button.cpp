#include "Button.h"
#include <iostream>
#include <array>
#include <functional>

namespace HW3
{
	void Button::setName(const std::string& id) 
	{
		this->name = id;
	}
	void Button::setValue(const std::string& value)
	{
		this->value = value;
	}
	void Button::setState(const HW3::BUTTON_STATE& state)
	{
		this->state = state;
	}
	int Button::getWidth() const
	{
		return this->width;
	}
	int Button::getHeight() const
	{
		return this->height;
	}
	std::string Button::getStateStr() const
	{
		if (this->state == BUTTON_STATE::ON)
		{
			return "ON";
		}
		else
		{
			return "OFF";
		}
	}
	BUTTON_STATE Button::getState() const
	{
		return this->state;
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

	void Button::listenForValueChange(std::function<void(const std::string & value)> observer)
	{
		this->SUBSCRIBERS.push_back(observer);
	}
}