#include <iostream>
#include "Singleton.h"

void myFunc( const Singleton& s )
{
   std::cout << s.toString() << "\n";
}

int main( int argc, char* argv[] )
{
   std::cout << "Hello World...\n";
   
   auto s = Singleton::getVal( "hi" );
   std::cout << "getVal hi is '" << s << "'\n";
   Singleton::setVal( "hi", "there" );
   Singleton::setVal( "bye", "now" );
   Singleton::setVal( "SE", "593" );
   
   std::cout << "getVal hi is '" << Singleton::getVal( "hi" ) << "'\n";
   
   std::cout << Singleton::toString() << "\n";
   
   
   //Singleton sq = Singleton::instance();
   //myFunc( Singleton::instance() );
   
   return 0;
}