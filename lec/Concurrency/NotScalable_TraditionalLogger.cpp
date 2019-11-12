#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <fstream>
#include <queue>
#include <optional>
#include <chrono>

//Traditional Thread safe logger using a mutex
//Compile:
//g++ --std=c++17 TraditionalLogger.cpp -lpthread
template< typename T >
class Logger
{
public:
   Logger( std::string const& fileName = "NotScalable.txt" )
   {
      //f.open( fileName, std::fstream::app | std::fstream::ate | std::fstream::out );
      f.open( fileName, std::fstream::trunc | std::fstream::out );
      if( ! f.good() )
         std::cout << "Failed to open log file \"" << fileName << "\"...\n";
   }

   ~Logger()
   {
      f.close();
   }

   void println( const std::string& s )
   {
      std::lock_guard< std::mutex > l{this->m};
      f << "Logging: " << s << std::endl;
   }

private:
   std::fstream f;
   mutable std::mutex m; //even though println is const, we still need to be able to mutate the mutex m.
   
};


void worker( Logger<std::string>& logger, int threadID )
{
   std::cout << "In thread " << threadID << "...\n";
   auto workDuration = std::chrono::seconds(3);
   auto startTime = std::chrono::system_clock::now();
   int i = 0;
   while( std::chrono::system_clock::now() < (startTime + workDuration) )
   {
      logger.println( "Thread " + std::to_string(threadID) + " is logging iteration " + std::to_string(i) );
      ++i;
   }
}


int main( int argc, char* argv[] )
{
   auto startTime = std::chrono::system_clock::now();   

   Logger<std::string> log;
   
   constexpr int NUM_THREADS = 10;
   std::vector<std::thread> threads;
   for (int i = 0; i < NUM_THREADS; i++)
   {
      std::thread t( &worker, std::ref(log), i );
      threads.push_back( std::move(t) );
   }
   
   std::cout << "In main thread waiting for work to finish...\n";
   for( auto& t : threads )
      t.join();
   return 0;
}