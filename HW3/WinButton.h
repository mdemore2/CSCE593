#pragma once
#include "Button.h"
#include <iostream>
#include <string>
#include<memory>

namespace HW3
{
	class WinButton : public Button
	{
	public:
		static std::shared_ptr<WinButton> New();

		//int getWidth() const override = 0;
		//int getHeight() const override = 0;

		int drawTopEdge(std::ostream& os) const override;
		int drawState(std::ostream& os) const override;
		int drawValue(std::ostream& os) const override;
		int drawBottomEdge(std::ostream& os) const override;

		//void listenForValueChange(std::function<void(const std::string & value)> observer) override;

	};
}