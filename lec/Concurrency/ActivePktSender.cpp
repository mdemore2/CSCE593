#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <queue>
#include <mutex>
#include <chrono>
#include <fstream>
#include <string>
#include <algorithm>

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

void encrypt(  std::string* s );
void compress( std::string* s );
void decorate( std::string* s );

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
      std::cout << "Thread " << std::this_thread::get_id() << " online...\n";
      while( !this->done )
         this->mq.popMsg()();
      std::cout << "Thread " << std::this_thread::get_id() << " shutting down...\n";
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
class Stage
{public:
   Stage( std::function<void(T*)> w ) : workerFunc{w} {}
   ~Stage() {}

   void process( std::string* e )
   {
      a.send( [=](){ workerFunc( e ); } ); 
   }
private:
   std::function<void(T*)> workerFunc;
   Active a; //Remember to put this last, always, for destruction order purposes.
};

//Decorate, Compress, Encrypt pipelined in 3 threads
void sendPackets( std::vector< std::string >& data )
{
   Stage<std::string> encryptor ( []  (std::string* s) { encrypt( s );} );
   Stage<std::string> compressor( [&] (std::string* s) { compress( s );
                                                         encryptor.process( s ) ;
                                                       } );
   Stage<std::string> decorator ( [&] (std::string* s) { decorate( s );
                                                         compressor.process( s );
                                                       } );

   for(auto& s : data )
      encryptor.process( &s );

}

void encrypt( std::string* s )
{
   //performs sweet encryption algorithm...
   std::transform( s->begin(), s->end(), s->begin(), []( char c ) { return std::toupper(c);} );
}

void compress( std::string* s )
{
   //do compression here
}

void decorate( std::string* s )
{
   *s = "[Header Here][DATA]=" + *s + "[Footer Here]\n";

}


std::vector<std::string> getData()
{
   size_t size = 1000;
   std::vector<std::string> v( size );
   
   for(int i = 0; i < size; i++)   
   {
      std::string s = std::to_string(i);
      for( char& c : s ) c += 49; //map [0,9] -> [a,j]
      v[i] = std::move( s );
   }
   

   return std::move(v);
}

int main( int argc, char* argv[] )
{
   std::cout << "Hello World -- Active Object... Faux Packet Sender...\n";

   auto startTime = std::chrono::system_clock::now();   

   std::vector< std::string > data = getData();
   //std::vector< std::string > data = { "hello", "world", "lets", "write", "pipelines", "using", "active", "objects" };
   std::thread t{ &sendPackets, std::ref(data) };
   //for( auto& s : data ) std::cout << s << "\n";
   for( auto it = data.crbegin(); it != data.crend(); ++it )
      std::cout << *it << "\n";
   //std::thread t{ &sendPackets, std::ref(data) };
   std::cout << "Main thread is back in main... Gonna wait for 1 second and then print data...\n";
   //std::this_thread::sleep_for( std::chrono::milliseconds(250));
   for( std::string& s : data ) std::cout << s;

   t.join();

   return 0;
}