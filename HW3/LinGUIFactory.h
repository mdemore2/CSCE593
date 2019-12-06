#pragma once
#include "AbstractGUIFactory.h"
#include "LinButton.h"
#include "LinWindow.h"
#include<memory>


namespace HW3
{
	class LinGUIFactory : public AbstractGUIFactory {
	public:
		~LinGUIFactory() = default;
		static std::shared_ptr<LinGUIFactory> New();

		std::shared_ptr<Button> createButton() const override;
		std::shared_ptr<Window> createWindow() const override;

		
	};
}