#pragma once
#include "Window.h"

namespace HW3
{
	class LinWindow : public Window
	{
		static std::shared_ptr< LinWindow > New();
		void onCreate();

	};
}