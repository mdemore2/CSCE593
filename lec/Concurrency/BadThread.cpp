#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <fstream>
#include <queue>
#include <optional>
#include <chrono>

std::string s = "My Data is here";

void printString()
{
   while( true )
   {
      //s = "I am overwriting the data to be a longer string that performs a memory alloc...\n";
      s = "Short\n";
      std::cout << s <<"\n";
      std::this_thread::sleep_for( std::chrono::milliseconds(10) );
   }
}

int main( int argc, char* argv[] )
{

   s = "Hello World\n";
   //std::future<void> f = std::async( std::launch::async, &Logger<std::string>::run, &log );
   std::thread t{ &printString };

   constexpr auto wait_period = std::chrono::seconds(3);
   auto startTime = std::chrono::system_clock::now();
   while( (startTime + wait_period) > std::chrono::system_clock::now() )
   {
      static int count = 0;
      ++count;
      auto now = std::chrono::system_clock::now();
      auto countdown = std::chrono::duration_cast< std::chrono::microseconds >( 
                        startTime - now + wait_period ).count();
      s = "The time is now " + std::to_string( countdown ) + " microseconds...";
      s += "Isn't thread a peach (" + std::to_string(count) + ")?";
      //std::cout << t << std::endl; //toggle this
   }
   std::cout << "Made it here...\n";

   auto elapsedTime = std::chrono::duration_cast< std::chrono::microseconds >( 
            std::chrono::system_clock::now() - startTime ).count();
   std::cout << "Time is usec " << elapsedTime << "\n";

   //log.shutdown(); //toggle this
   t.join(); //only when using std::thread and not async

   return 0;
}