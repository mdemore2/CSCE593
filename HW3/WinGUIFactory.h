#pragma once
#include "AbstractGUIFactory.h"
#include "WinButton.h"
#include "WinWindow.h"

namespace HW3
{
	class WinGUIFactory : public GUIFactory {
	public:
		Button* createButton() {
			return new WinButton;
		}
		Window* createWindow() {
			return new WinWindow;
		}
	};

}