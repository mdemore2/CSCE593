#pragma once
#include "Button.h"
#include "Window.h"
//osWINDOWS and osLINUX implemented using inheritence, os determines makeup of gui (windows, buttons)

#define osWINDOWS
#define osLINUX

namespace HW3
{
	class GUIFactory
	{
	public:
		virtual ~GUIFactory() = 0;
		virtual std::shared_ptr<Window> createWindow() = 0;
		virtual std::shared_ptr<Button> createButton() = 0;

	protected:
		std::shared_ptr<GUIFactory> factory;
	};
}