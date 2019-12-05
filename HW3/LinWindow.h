#pragma once
#include "Window.h"
#include "Button.h"
#include "LinButton.h"

namespace HW3
{
	class LinWindow : public Window
	{
	public:

		static std::shared_ptr< LinWindow > New();

		virtual int addButton(std::shared_ptr<LinButton> button) const = 0;
		virtual int draw(std::ostream& os) const override = 0;

	protected:

		std::vector<std::shared_ptr<LinButton>> buttons;
	};
}