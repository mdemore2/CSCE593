#pragma once
#include "AbstractGUIFactory.h"
#include "WinButton.h"
#include "WinWindow.h"

namespace HW3
{
	class WinGUIFactory : public GUIFactory {
	public:
		static std::shared_ptr<WinGUIFactory> New();

		Button* createButton() {
			return WinButton::New();
		}
		Window* createWindow() {
			return new WinWindow;
		}
	};

}