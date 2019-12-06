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

		void setName(const std::string& id) override;


		//int addButton(std::shared_ptr<WinButton> button) = 0;
		void draw(std::ostream& os) const override;

	protected:

		std::vector<std::shared_ptr<WinButton>> buttons;



	};
}