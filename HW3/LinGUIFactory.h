#pragma once
#include "AbstractGUIFactory.h"
#include "LinButton.h"
#include "LinWindow.h"

namespace HW3
{
	class LinGUIFactory : public GUIFactory {
	public:
		Button* createButton() {
			return new LinButton;
		}
		Window* createWindow() {
			return new LinWindow;
		}
	};
}