#pragma once
#include "Button.h"
#include "Window.h"
#include <memory>
//osWINDOWS and osLINUX implemented using inheritence, os determines makeup of gui (windows, buttons)

#define osWINDOWS
#define osLINUX

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