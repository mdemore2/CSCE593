#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <queue>
#include <mutex>
#include <chrono>
#include <fstream>
#include <string>

//https://youtu.be/80BZxujhY38?t=4634
//https://www.youtube.com/watch?v=80BZxujhY38&feature=youtu.be&t=4634
//http://www.drdobbs.com/parallel/prefer-using-active-objects-instead-of-n/225700095?pgno=2

//This class Actively manages its own thread which has been tied to this object's lifetime
//Uses classic "message pump" interface

//Active Object Pattern (https://youtu.be/U9Tf7h-etl0?t=1813):
//Intent of AOP:
//1. Define service requests on components as the units of concurrencey & 
//   run service requests on a component in thread(s) other than client thread.
//2. Enable client & component to interact asynchronously to produce & consume
//   service results.
//Applicability of AOP:
//1. Separate calling method from method execution. *Can* employ multiple threads.
//2. Client desires to have a method invocation occur, but that invocation could be time consuming
//       - Download a file from a server / Log into a server
//       - Perform a very expension calculation
//       - The client may be using a GUI thread which *must* remain responsive at all times
//       - Request a worker thread or pipelined background task perform an operation specific to that pipeline
//

//Considerations / Drawbacks:
//https://youtu.be/U9Tf7h-etl0?t=2702 (Doug Schmidt talks about Herb's implementation).
//1.Overhead
//       -Generally, do not create more threads than ~2x number of processors
//2.Debugging
//       -Non-determinism


template< typename T >
class MsgQueue_ThreadSafe
{public:
   MsgQueue_ThreadSafe() = default;
   MsgQueue_ThreadSafe( MsgQueue_ThreadSafe&& toMoveCopy ) { *this = std::move( toMoveCopy ); }
   ~MsgQueue_ThreadSafe() = default;
   MsgQueue_ThreadSafe& operator =( MsgQueue_ThreadSafe&& toMoveAssign )
   {
      if( this != &toMoveAssign )
      {
         this->q = std::move( toMoveAssign.q );
         this->m = std::move( toMoveAssign.m );
      }
      return *this;
   }

   T popMsg()
   {
      std::lock_guard< std::mutex > lock{ this->m };
      if( this->q.size() == 0 )
         return [](){};
      T msg = this->q.front();
      this->q.pop();
      return std::move( msg );
   }

   void pushMsg( T&& msg )
   {
      std::lock_guard< std::mutex > lock{ this->m };
      this->q.push( std::move( msg ) );
   }
private:
   std::queue<T> q;
   std::mutex m;
};

class Active
{public:
   using Msg = std::function<void()>;
   Active() : thd( [&]()
   {
      while( !this->done )
         this->mq.popMsg()();
   }) {}

   ~Active()
   {
      send( [=] { done = true; } ); 
      this->thd.join(); 
   } 

   void send( Msg m ) { mq.pushMsg( std::move(m) ); }
private:
   bool done = false;
   MsgQueue_ThreadSafe< Msg > mq;
   std::thread thd;
};

template< typename T >
class Logger
{public:
   Logger( std::function<void( T, std::ostream& )> w, 
           std::string const& fileName = "ActiveLogger.txt" ) : workerFunc{w}
   {
      f.open( fileName, std::fstream::trunc | std::fstream::out );
      if( ! f.good() )
         std::cout << "Failed to open log file \"" << fileName << "\"...\n";
   }

   ~Logger() {} //automatically destructs the fstream! No clean up, RAII FTW

   void println( T e )
   {
      a.send( [=](){ workerFunc( std::move(e), f ); } ); 
   }

   private:
   std::function<void(T, std::ostream&)> workerFunc;
   std::fstream f;
   Active a; //Remember to put this last, always, for destruction order purposes.
};


// int main( int argc, char* argv[] )
// {
//    std::cout << "Hello World -- Active Object... Logger Async Example...\n";

//    auto startTime = std::chrono::system_clock::now();   

//    Logger<std::string> log{ [=]( std::string&& s, std::ostream& fout )
//                               { fout << "Log: " << s << "\n"; }
//                           };

//    constexpr auto wait_period = std::chrono::seconds(3);
//    while( startTime + wait_period > std::chrono::system_clock::now() )
//    {
//       auto now = std::chrono::system_clock::now();
//       auto countdown = std::chrono::duration_cast< std::chrono::microseconds >( 
//                   startTime - now + wait_period ).count();
//       std::string t = "The time is now " + std::to_string( countdown ) + " microseconds...";
//       log.println( t );
//       //std::cout << t << std::endl; //toggle this
//    }

//    auto elapsedTime = std::chrono::duration_cast< std::chrono::microseconds >( 
//                   std::chrono::system_clock::now() - startTime ).count();
//    std::cout << "Time is usec " << elapsedTime << "\n";

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

   
    Logger<std::string> log{ [=]( std::string&& s, std::ostream& fout )
                               { fout << "Log: " << s << "\n"; }
                           };
   
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
   return 0;
}