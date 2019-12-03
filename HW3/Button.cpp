#include "Button.h"
#include <iostream>
#include <array>
#include <functional>

namespace HW3
{
	void setName(const std::string& id)
	{
		Button::name = id;
	}
	void setValue(const std::string& value)
	{
		Button::value = value;
	}
	void setState(const HW3::BUTTON_STATE& state)
	{
		Button::state = state;
	}
	int getWidth()
	{
		return Button::width;
	}
	int getHeight()
	{
		return Button::height;
	}
	std::string getStateStr()
	{
		if (Button::state == BUTTON_STATE::ON)
		{
			return "ON";
		}
		else
		{
			return "OFF";
		}
	}
	BUTTON_STATE getState()
	{
		return Button::state;
	}

	int drawTopEdge(std::ostream& os)
	{
	}
	int drawState(std::ostream& os)
	{
	}
	int drawValue(std::ostream& os)
	{
	}
	int drawBottomEdge(std::ostream& os)
	{
	}

	void listenForValueChange(std::function<void(const std::string & value)> observer)
	{
		Button::SUBSCRIBERS.push_back(observer);
	}
}