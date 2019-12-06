#pragma once
#include "AbstractGUIFactory.h"
#include "WinButton.h"
#include "WinWindow.h"

namespace HW3
{
	class WinGUIFactory : public GUIFactory {
	public:
		static std::shared_ptr<WinGUIFactory> New();

		virtual std::shared_ptr<WinButton> createButton() const = 0;
		virtual std::shared_ptr<WinWindow> createWindow() const = 0;
			
	};

}