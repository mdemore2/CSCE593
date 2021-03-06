#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <memory>


namespace HW3
{
	enum class BUTTON_STATE { ON, OFF };

	class Button
	{
	public:
		virtual ~Button() = default;
		void setName(const std::string& id);
		void setValue(const std::string& value);
		void setState(const BUTTON_STATE& state);
		int getWidth() const;
		int getHeight() const;
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
		 std::vector<std::function<void(const std::string& value)>> SUBSCRIBERS;
		 std::shared_ptr<Button> button;


	};

}