#pragma once
#include "AbstractGUIFactory.h"
#include "LinButton.h"
#include "LinWindow.h"

namespace HW3
{
	class LinGUIFactory : public GUIFactory {
	public:
		static std::shared_ptr<LinGUIFactory> New();

		Button* createButton() {
			return new LinButton;
		}
		Window* createWindow() {
			return new LinWindow;
		}
	};
}