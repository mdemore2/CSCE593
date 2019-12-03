#pragma once
//PINS implemented with Singleton pattern, ensures single instance of pins

namespace HW3
{

enum class PIN_NUM : int { PIN_0 = 0, PIN_1, PIN_2, PIN_3 };
enum class PIN_VAL : int { LOW = 0, HIGH };

class FourPinExternInput
{
public:

   //Reads the input pins again. For simplicity, we simulate a new value by incrementing an internal counter
   //that wraps around the domain [0,15] (4 bits).
   //Do NOT modify this method
   static void receiveNewPinState();

   static void listenForPinChange(std::function<void(const std::array<PIN_VAL,4>)> observer);


   // implement a method called listenForPinChange( ... ));
   //It should take as a parameter a function that consumes an array of PIN_VAL. When a change to the pins occurs,
   //each function passed to this method should be called back and be passed the latest pin state.
   
   //add any other methods needed to fulfil your solution. ensure you include them in your UML diagram

private:
   //implement me
   //this should store all necessary member data for the pins.
	static std::array<PIN_VAL, 4> PINS;
	static std::vector<std::function<void(const std::array<PIN_VAL, 4>)>> SUBSCRIBERS;
};
}