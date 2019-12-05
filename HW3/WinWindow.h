#pragma once
#include "Window.h"

namespace HW3
{
	class WinWindow : public Window
	{
		static std::shared_ptr< WinWindow > New();
		void onCreate();



	};
}