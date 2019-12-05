#pragma once
#include <string>
#include <vector>
#include <functional>

namespace HW3
{
	enum class BUTTON_STATE { ON, OFF };

	class Button
	{
	public:
		virtual ~Button() = 0;
		void setName(const std::string& id);
		void setValue(const std::string& value);
		void setState(const BUTTON_STATE& state);
		virtual int getWidth() const = 0;
		virtual int getHeight() const = 0;
		std::string getStateStr() const;
		BUTTON_STATE getState() const;

		virtual int drawTopEdge(std::ostream& os) const = 0;
		virtual int drawState(std::ostream& os) const = 0;
		virtual int drawValue(std::ostream& os) const = 0;
		virtual int drawBottomEdge(std::ostream& os) const = 0;

		virtual void listenForValueChange(std::function<void(const std::string& value)> observer);

	protected:
		 std::string name, value;
		 BUTTON_STATE state;
		 int width, height;
		 std::vector<std::function<void(const std::string& value)>> SUBSCRIBERS;
		 std::shared_ptr<Button> button;


	};

}