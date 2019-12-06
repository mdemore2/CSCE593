#pragma once
#include "AbstractGUIFactory.h"
#include "WinButton.h"
#include "WinWindow.h"
#include <memory>

namespace HW3
{
	class WinGUIFactory : public AbstractGUIFactory {
	public:
		~WinGUIFactory() = default;
		static std::shared_ptr<WinGUIFactory> New();

		std::shared_ptr<Button> createButton()  const override;
		std::shared_ptr<Window> createWindow() const override;
			
	};

}