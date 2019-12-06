#pragma once
#include "AbstractGUIFactory.h"
#include "LinButton.h"
#include "LinWindow.h"

namespace HW3
{
	class LinGUIFactory : public GUIFactory {
	public:
		~LinGUIFactory() = default;
		static std::shared_ptr<LinGUIFactory> New();

		virtual std::shared_ptr<LinButton> createButton() const = 0;
		virtual std::shared_ptr<LinWindow> createWindow() const = 0;

		
	};
}