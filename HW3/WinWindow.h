#pragma once
#include "Window.h"
#include "Button.h"
#include "WinButton.h"

namespace HW3
{
	class WinWindow : public Window
	{
	public:
		static std::shared_ptr< WinWindow > New();
		
		virtual int addButton(std::shared_ptr<WinButton> button) const = 0;
		virtual int draw(std::ostream& os) const override = 0;

	protected:

		std::vector<std::shared_ptr<WinButton>> buttons;



	};
}