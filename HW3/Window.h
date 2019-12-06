#pragma once
#include <string>
#include <vector>
#include <functional>
#include "Button.h"


namespace HW3
{
	class Window 
	{
	public:
		virtual ~Window() = 0;
		virtual void setName(const std::string& id);
		//virtual int getWidth() const = 0;
		//virtual int getHeight() const = 0;

		virtual int addButton(std::shared_ptr<Button> button) const = 0;
		virtual int draw(std::ostream& os) const = 0;
		

		/*
		virtual int drawTopEdge(std::ostream& os) const = 0;
		virtual int drawState(std::ostream& os) const = 0;
		virtual int drawValue(std::ostream& os) const = 0;
		virtual int drawBottomEdge(std::ostream& os) const = 0;
		*/

	protected:
		std::string name;
		//int width, height;
		std::shared_ptr<Window> window;
		std::vector<std::shared_ptr<Button>> buttons;
	};

}