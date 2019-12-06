#pragma once
#include "Window.h"
#include "Button.h"
#include <memory>

#include "LinButton.h"

namespace HW3
{
	class LinWindow : public Window
	{
	public:

		static std::shared_ptr< LinWindow > New();

		void setName(const std::string& id)  override;


		//int addButton(std::shared_ptr<LinButton> button)   override;
		void draw(std::ostream& os) const override;

	protected:

		std::vector<std::shared_ptr<LinButton>> buttons;
	};
}