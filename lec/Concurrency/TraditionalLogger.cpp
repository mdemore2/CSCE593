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
   Logger( std::string const& fileName = "traditionalLog.txt" )
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

   void println( const T& s )
   {
      std::lock_guard< std::mutex > l{this->m};
      this->q.push( s );
   }

   void shutdown() { done = true; }

   void run()
   {
      while( !done || this->qSz > 0 )
      {
         auto item = this->getNextMsg();
         if( item )
            f << "Logging: " << *item << std::endl;
      }
   }

private:
   std::fstream f;
   std::mutex m;
   std::queue<T> q;
   bool done = false;
   size_t qSz = 0; //updated on each call to getNextMsg()

   std::optional<T> getNextMsg()
   {
      std::lock_guard< std::mutex > lock{this->m};
      if( !this->q.empty() )
      {
         auto item = std::move( q.front() );
         q.pop();
         this->qSz = q.size(); //nasty side-effect, I don't like this, but it is relative efficient solution
         return std::move(item);
      }
      return std::nullopt;
   }
};

// int main( int argc, char* argv[] )
// {
//    auto startTime = std::chrono::system_clock::now();   

//    Logger<std::string> log;
//    //std::future<void> f = std::async( std::launch::async, &Logger<std::string>::run, &log );
//    std::thread t{ &Logger<std::string>::run, &log };

//    constexpr auto wait_period = std::chrono::seconds(3);

//    while( startTime + wait_period > std::chrono::system_clock::now() )
//    {
//       auto now = std::chrono::system_clock::now();
//       auto countdown = std::chrono::duration_cast< std::chrono::microseconds >( 
//                         startTime - now + wait_period ).count();
//       std::string t = "The time is now " + std::to_string( countdown ) + " microseconds...";
//       log.println( t );
//       //std::cout << t << std::endl; //toggle this
//    }

//    auto elapsedTime = std::chrono::duration_cast< std::chrono::microseconds >( 
//             std::chrono::system_clock::now() - startTime ).count();
//    std::cout << "Time is usec " << elapsedTime << "\n";

//    log.shutdown(); //toggle this
//    t.join(); //only when using std::thread and not async

//    return 0;
// }

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
   std::thread t{ &Logger<std::string>::run, &log };
   
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

   std::cout << "All worker threads have finished logging, shutting down logger...\n";

   log.shutdown(); //don't forget about doing this
   t.join();       //or this!
   return 0;
}