#pragma once
#include "Button.h"

namespace HW3
{
	class LinButton : public Button
	{
		static std::shared_ptr< LinButton > New();

		int getWidth() const override = 0;
		int getHeight() const override = 0;

		int drawTopEdge(std::ostream& os) const override = 0;
		int drawState(std::ostream& os) const override = 0;
		int drawValue(std::ostream& os) const override = 0;
		int drawBottomEdge(std::ostream& os) const override = 0;

		void listenForValueChange(std::function<void(const std::string & value)> observer) override;
	};
}