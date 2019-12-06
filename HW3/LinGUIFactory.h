#pragma once
#include "AbstractGUIFactory.h"
#include "LinButton.h"
#include "LinWindow.h"

namespace HW3
{
	class LinGUIFactory : public AbstractGUIFactory {
	public:
		~LinGUIFactory() = default;
		static std::shared_ptr<LinGUIFactory> New();

		virtual std::shared_ptr<Button> createButton() const override = 0;
		virtual std::shared_ptr<Window> createWindow() const override = 0;

		
	};
}