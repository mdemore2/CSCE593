#include <iostream>
#include <string>
#include <future>
#include <random>
#include <functional>
#include <array>

//Compile this code with: g++ -std=c++17 -Wall 05LecObserver.cpp -lpthread
// -l means link against a library, in this case the library is named libpthread.a (but we omit the lib prefix and .a suffix
// via linux convention)
// pthread is the library against which the linker links (called /usr/lib/x86_64-linux-gnu/libpthread.a)

//Instead of inheritance, we use std::function. This is still polymorphic via
//a technique called type erasure, but maybe this is more elegant code??? //Contrast this to 05LecObserver.cpp
using CallbackFunc = std::function<void(const std::string&)>;

class CellPhone
{
   public:
   CellPhone( const std::string& number ) : number( number ) {}

   void onNotify( const std::string& msg )
   {
      std::cout << "!!!Cell Phone " << this->number << " was notified that: " << msg << "!!!\n\n";
   }

   std::string number = "";
};

class SecurityService
{
   public:   
   std::vector< CallbackFunc > clients;
   void subscribe( CallbackFunc newClient )
   {
      std::cout << "A Client has subscribed...\n";
      clients.push_back( newClient );
   }
   
   std::future<void> startSecurityService( int desiredParanoiaLevel, std::mt19937& gen )
   {
      std::cout << "Launching an async guard dog...\n";
      //This will launch a new thread (could be run on a separate processor)
      std::future<void> f = std::async( std::launch::async, std::bind( &SecurityService::asyncSecurityMonitor, this, desiredParanoiaLevel, gen ) );
      std::cout << "Done Launching an async guard dog... Now You're account will be safe, I promise, I'm equifax, trust me...\n";
      return std::move( f );
   }
   
   void asyncSecurityMonitor( int paranoiaLevel, std::mt19937& gen )
   {
      std::cout << "  **I am the guard dog thread... I'll alert all subscribers when their account is in danger of haxorz (at least until your trail expires)...\n";
      auto startTime = std::chrono::system_clock::now();
      auto duration = std::chrono::seconds(10);
      auto stopTime = startTime + duration;
      
      do
      {
         std::uniform_int_distribution<> distribution( 1, 100 );
         int worryLevel = distribution( gen );         
         std::cout << "  **Threat level is " << worryLevel << "\n";
         if( worryLevel > paranoiaLevel )
         {
            std::string msg = "  **Security Threat level " + std::to_string(worryLevel) + "... WORRY about your account...\n";
            for( auto& c : clients )
               c( msg );
         }
         else
         {
            std::cout << "  **Equifax told me your account looks safe...Time to relax...\n\n\n";
         }
         
         auto doodleStopTime = std::chrono::system_clock::now() + std::chrono::seconds(2);
         const std::array< char, 4 > doodle = {'-','\\', '|', '/'};
         int i = 0;
         while( std::chrono::system_clock::now() < doodleStopTime )
         {  
            std::cout << ' ' << doodle[i++] << '\r' << std::flush;
            i = i % doodle.size();
         }
      }
      while( std::chrono::system_clock::now() < stopTime );
      
      std::cout << "  **Your free security trial has lapsed,\n"
                << "     **Would you like me sell your account\n"
                << "     **info to continue with your free trial?\n";
   }
};


int main( int argc, char* argv[] )
{
   std::cout << "Okay folks, let's get started...\n";
   
   //make a random number engine:
   std::random_device rd;    //C++ random device -- used to obtain a seed for the random number engine
   std::mt19937 gen( rd() ); //Standard mersenne_twister_engine seeded with random_device rd()
   std::vector< std::shared_ptr< CellPhone > > phones;

   SecurityService security;
   //Here we make a normal cell phone that is subscribable
   std::shared_ptr< CellPhone > myCell = std::make_shared<CellPhone>( "867 5309" );
   phones.push_back( std::move( myCell ) );
   std::shared_ptr< CellPhone > aCell = std::make_shared<CellPhone>( "444 4444" );
   phones.push_back( std::move( aCell ) );

   for( auto& p : phones )
   {
      CallbackFunc f = [&]( const std::string& msg ) { p->onNotify( msg ); }; //uses a lambda
      //CallbackFunc f = std::bind( &CellPhone::onNotify, p, std::placeholders::_1 );
      security.subscribe( f ); //security is now an Observer that waits for an event and will Notify the cell phone when that event occurs
   }

   std::future<void> future = security.startSecurityService( 42, gen ); //launch a watchdog thread to look for threats against your bank account
   for (int i = 0; i < 10 ; i++)
   {
      std::cout << "==================Main thread - Doing other things now... Let me know via my cell if I'm in trouble...\n";
      std::this_thread::sleep_for( std::chrono::seconds(2) );
   }

   std::cout << "Done... Press ENTER to exit...\n";
   return 0;
}