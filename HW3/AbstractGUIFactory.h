#pragma once
#include "Button.h"
#include "Window.h"
#include <memory>
//osWINDOWS and osLINUX use inheritence, os determines makeup of gui (windows, buttons)
//abstract factory pattern used, both guis composed of windows and buttons, but windows and buttons are different based on os

namespace HW3
{
	class AbstractGUIFactory
	{
	public:
		virtual ~AbstractGUIFactory() = default;
		virtual std::shared_ptr<Window> createWindow() const = 0;
		virtual std::shared_ptr<Button> createButton() const = 0;

	protected:
		std::shared_ptr<AbstractGUIFactory> factory;
	};
}