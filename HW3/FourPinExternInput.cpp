#include "FourPinExternInput.h"

#include <iostream>
#include <array>
#include <functional>

namespace HW3
{

//any static storage declared in the header file ought to be stored in this translation unit.


void listenForPinChange(std::function<void(const std::array<PIN_VAL, 4>)> observer)
{
	FourPinExternInput::SUBSCRIBERS.push_back(observer);
	//It should take as a parameter a function that consumes an array of PIN_VAL. When a change to the pins occurs,
	//each function passed to this method should be called back and be passed the latest pin state.
}

void FourPinExternInput::receiveNewPinState()
{
   //This internally simulates reading from pins... It sets the internal pins to a
   //new value via idx. It is an incrementing 4 bit number. We interpret it as unsigned.

   static constexpr PIN_VAL o = PIN_VAL::LOW;
   static constexpr PIN_VAL l = PIN_VAL::HIGH;

   static std::array< std::array< PIN_VAL, 4 >, 16 > data =
   { { { o,o,o,o }, //0
       { o,o,o,l }, //1
       { o,o,l,o }, //2
       { o,o,l,l }, //3
       { o,l,o,o }, //4
       { o,l,o,l }, //5
       { o,l,l,o }, //6
       { o,l,l,l }, //7
       { l,o,o,o }, //8
       { l,o,o,l }, //9
       { l,o,l,o }, //10
       { l,o,l,l }, //11
       { l,l,o,o }, //12
       { l,l,o,l }, //13
       { l,l,l,o }, //14
       { l,l,l,l }  //15
   } };

   static int idx = -1;

   idx = ( idx + 1 ) % data.size();

   //idx represents the index of the new value read from the FourPinExternalInput::pins. Let's set the FourPinExternalInput::pins to the value at data[idx]
   FourPinExternInput::PINS = data[idx];

   //using std::underlying_type_t<PINS>(pin)?

   //TODO: All our pins have changed, time to notify any subscribers who want to observe this
   //event... Write that code here

   for (int i = 0; i < FourPinExternInput::SUBSCRIBERS.length(); i++)
   {
	   FourPinExternInput::SUBSCRIBERS[i]();
	   //run lambda
   }

}

}