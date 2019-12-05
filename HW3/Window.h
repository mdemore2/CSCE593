#pragma once
#include <string>
#include <vector>
#include <functional>


namespace HW3
{
	class Window 
	{
	public:
		virtual ~Window() = 0;
		void setName(const std::string& id);
		virtual int getWidth() const = 0;
		virtual int getHeight() const = 0;
		

		virtual int drawTopEdge(std::ostream& os) const = 0;
		virtual int drawState(std::ostream& os) const = 0;
		virtual int drawValue(std::ostream& os) const = 0;
		virtual int drawBottomEdge(std::ostream& os) const = 0;

	protected:
		std::string name;
		int width, height;
		std::shared_ptr<Window> window;
	};

}