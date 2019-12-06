#include <iostream>
#include <string>

#define osWINDOWS
//#define osLINUX

#include "AbstractGUIFactory.h"
#include "WinGUIFactory.h"
#include "LinGUIFactory.h"
#include "FourPinExternInput.h"
#include <memory>

using namespace HW3;

void testDrawingWindowsAndButtons();
void testMainProgram();

int main(int argc, char* argv[])
{
   if( argc >= 2 && std::string{ argv[1] } == "test" )
      testDrawingWindowsAndButtons();
   else
      testMainProgram();

	return 0;
}

void testMainProgram()
{
	#ifdef osWINDOWS
		auto f = new WinGUIFactory();
	#elif defined osLINUX
		f = new LinGUIFactory();
	#endif

	std::vector<std::shared_ptr<Button>> inbutts, outbutts;
	auto wi = f->createWindow();
	auto wo = f->createWindow();
	wi->setName("Inputs");
	wo->setName("Outputs");

	for (int i = 0; i < 3; i++)
	{
		int three = 3;
		auto bi = f->createButton();
		auto bo = f->createButton();
		std::string inname = "In  " + std::to_string(three-i);
		std::string outname = "Out " + std::to_string(three-i);
		bi->setName(inname);
		bo->setName(outname);
		bi->setState(BUTTON_STATE::ON);
		bo->setValue("o");
		bi->setState(BUTTON_STATE::OFF);
		bo->setValue("o");
		wi->addButton(bi);
		wo->addButton(bo);
		inbutts.push_back(bi);
		outbutts.push_back(bo);
	}

	FourPinExternInput::listenForPinChange([=](const auto& pins) {
		for (int i = 0; i < 3; i++)
		{
			if (pins[i] == PIN_VAL::HIGH)
			{
				inbutts.at(i)->setValue("l");
			}
			else
			{
				inbutts.at(i)->setValue("o");

			}
		}
		});
	auto outbuttlamb = [=](auto & pins) { 
		for (int i = 0; i < 3; i++)
		{
			if (outbutts.at(i)->getState() == BUTTON_STATE::ON) {
				if (pins[i] == PIN_VAL::HIGH)
				{
					outbutts.at(i)->setValue("l");
				}
				else
				{
					outbutts.at(i)->setValue("o");
				}
			}
		}
	};

	//FourPinExternInput::listenForPinChange(inbuttlamb);
	FourPinExternInput::listenForPinChange(outbuttlamb);

   //create the two windows - input and output. The top window is the "input" window. The bottom is the "output" window.
   //One button exists in each window for each external hardware pin we are monitoring. Create these buttons and associate
   //them with the corresponding hardware pins they are monitoring, respectively.

   std::cout << "Enter a 4-bit mask as a set of space\n"
             << " separated 1 or 0 values\n"
             << " High bit to low bit, ex:  \"1 1 1 1\"\n"
             << " This mask will be applied \n"
             << " to all 4 hardware input pins: ";

   char m3, m2, m1, m0;
   std::cin >> m3;
   std::cin >> m2;
   std::cin >> m1;
   std::cin >> m0;

   if (m3 == static_cast<char>(1))
   {
	   outbutts.at(0)->setState(BUTTON_STATE::ON);
   }
   if (m2 == static_cast<char>(1))
   {
	   outbutts.at(1)->setState(BUTTON_STATE::ON);
   }
   if (m1 == static_cast<char>(1))
   {
	   outbutts.at(2)->setState(BUTTON_STATE::ON);
   }
   if (m0 == static_cast<char>(1))
   {
	   outbutts.at(3)->setState(BUTTON_STATE::ON);
   }
   
   
   //read 4 ints from standard in. First read input corresponds to Most Significant Bit of the mask.
   //Last read input corresponds to Least Significant Bit of the mask.
   //The input Window's buttons always have a state of "ON"
   //The button state for the output Window corresponds to its associated mask value.
   //There is no functional difference between an input window and an output window from a design/
   //implementation perspective.


   //This eats any extra white space of input characters after the mask has been read in.
   std::cout << "Ready to mask digital inputs. Iterating\n"
             << " through all pin inputs now...\n";
   std::string s;
   std::getline( std::cin, s );

   for( int i = 0; i < 17; i++ )
   {
      FourPinExternInput::receiveNewPinState();
      wi->draw( std::cout ); //update the input window w/ your instance
      wo->draw( std::cout ); //update the output window w/ your instance
      std::cin.get();
   }
}

void testDrawingWindowsAndButtons()
{
   //Do NOT touch any code in this function. Your output must identically match my output.
   AbstractGUIFactory* f = nullptr;

#ifdef osWINDOWS
   f = new WinGUIFactory();
#elif defined osLINUX
   f = new LinGUIFactory();
#endif

   auto w = f->createWindow();
   w->setName( "This is a big, long name" );

   {
      auto b = f->createButton();
      w->addButton( b );
      b->setName( "My Big Button name" );
      b->setState( BUTTON_STATE::OFF );
      b->setValue( "This is my value as a button..." );
   }

   w->draw( std::cout );

   {
      auto b = f->createButton();
      w->addButton( b );
      b->setName( "Small" );
      b->setState( BUTTON_STATE::ON );
      b->setValue( "Short" );
   }

   {
      auto b = f->createButton();
      w->addButton( b );
      b->setName( "My other button's name" );
      b->setState( BUTTON_STATE::ON );
      b->setValue( "Here is another value" );
   }

   w->draw( std::cout );

   w->setName( "This is a very long title, it should be longer than the buttons..." );
   w->draw( std::cout );
   std::cin.get();
}
