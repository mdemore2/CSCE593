#pragma once
#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <queue>
#include <mutex>
#include <chrono>
#include <fstream>
#include <string>

//If necessary, you may need to include gnuplot-iostream, if it wasn't already included elsewhere
//#include "gnuplot-iostream/gnuplot-iostream.h"

//https://youtu.be/80BZxujhY38?t=4634
//https://www.youtube.com/watch?v=80BZxujhY38&feature=youtu.be&t=4634
//http://www.drdobbs.com/parallel/prefer-using-active-objects-instead-of-n/225700095?pgno=2

//This class Actively manages its own thread which has been tied to this object's lifetime
//Uses a "message pump" interface inside an Active Object Pattern.

//Active Object Pattern Herb Sutter (https://youtu.be/80BZxujhY38?t=3924), 
//Doug Schmidt(https://youtu.be/U9Tf7h-etl0?t=1813):
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


namespace Aftr
{

   //This ThreadSafe queue is simply an implementation detail, so we place it in its own nested namespace
   //so that the class name MsgQueue_ThreadSafe is not directly accessible from the Aftr namespace.
   namespace active_plotter_detail
   {
      template< typename T >
      class MsgQueue_ThreadSafe
      {
      public:
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

         std::optional<T> popMsg()
         {
            std::lock_guard< std::mutex > lock{ this->m };
            if( this->q.size() == 0 )
               return std::nullopt;
            T msg = this->q.front();
            this->q.pop();
            return std::move( msg );
         }

         void pushMsg( T&& msg )
         {
            std::lock_guard< std::mutex > lock{ this->m };
            this->q.push( std::move( msg ) );
         }

         size_t size() { return q.size(); }
      private:
         std::queue<T> q; //internally, the thread safe queue is simply a mutex protected std::queue
         std::mutex m;    //we use mutex m to protect access to the queue
      };
   } //namespace active_plotter::detail

   //This class implements the Active Object Pattern. BLUF: This object is "active" because
   //the thread on which this instance runs is the thread that does the work submitted by other threads.
   //That is, other threads simply submit a std::function and this thread executes it.
   //This is opposite from some traditional threading paradigms where the calling thread
   //does the work.
   class Active
   {
   public:
      using Msg = std::function<void()>;
      Active() : thd( [&]()
         {
            while( !this->doneRunning )
            {
               //Here is a very naive implementation -- it will cause latency to build up and eventually become
               //non-real time. Your team must adjust this code to always run in real time.
               this->mq.popMsg()(); //What goes wrong using this as the main driver, why - discuss breifly in presentation?

               //rewrite the above code such that this AOP thread always only plots the most recent set of data and always remains
               //up-to-date with the latest -- it will be more than one line
            }
         } ) {}

         ~Active()
         {
            //The destructor simply sets a doneRunning flag to true so the next iteration of the while loop will
            //terminate. The .join() method causes us to wait for thd to shutdown before leaving the destructor.
            //This way, we guranteed that the AOP is shutdown and destroyed fully by the time the AOP finishes.
            //The code would crash of the non-detached thread was not joined and the thread object left scope.
            this->doneRunning = true;
            this->thd.join();
         }

         void send( Msg m )
         {
            //you need to enqueue the message into the thread safe message queue.
            
            //To make your output like the given solution (only for debugging reasons, not functional)
            //I've left this line here -- I suspect it may help you debug one you figure out stuff :), maybe...
            std::cout << AFTR_FILE_LINE_STR << ": mq.size() is " << mq.size() << "\n";
         }
   private:
      std::atomic<bool> doneRunning = false; //atomic thread ensures no race condition exists
      active_plotter_detail::MsgQueue_ThreadSafe< Msg > mq;
      std::thread thd;
   };

   //This class is the actual plotter that is *composed* of an Active Object (see definition above)
   //as well as a GNUPlot instance. When a user passes a message into this object, it simply queues
   //it for execution inside of its Active Object. Notice that the workerFunc is a function
   //that consumes a type T *and also* requires a GNUPlot instance.
   //This class is templated on the data format of our plot data. In other words, what type of
   //data structure do we build when we plot our GNUPlot data. Perhaps it is a collection of x,y
   //points? This could be represented many ways. So to provide maximum flexibility, it is a Template
   //paramter.
   template< typename T >
   class ActivePlotter
   {
   public:

      using Worker_Func_Prototype = std::function<void( T, Gnuplot& )>;

      ActivePlotter( std::function<void( T, Gnuplot& )> w ) :
                                    workerFunc{ w }, p( ManagerEnvironmentConfiguration::getGNUPlotBin() )
      { /*Nothing to do here, the initializer assigned values to this->workerFunc and this->a.*/ }
      ~ActivePlotter() {} //automatically destruct member variables! No clean up, RAII FTW

      //The user thread invokes this plot method. We do *not* want the busy user thread to spend its
      //time executing GNUPlot instructions. Instead, we simply want the user thread to enque work for 
      //this Active Object's thread to execute. Hence the user thread simply places an std::function
      //and its corresponding data inside a queue. Then later on, inside the Active Object's thread,
      //the active object executes this function, processes the in passed data, and actually uses
      //Gnuplot to plot the data.
      void plot( T plotData )
      {
         //Use the Active object, a, and invoke its send method. The input to the send method
         //should be a lambda that will execute workerFunc on both the T plotData and Gnuplot p.
         
         //implement me
      }

   private:
      Worker_Func_Prototype workerFunc; //this is the lambda defined in the Worker_Func_Prototype alias above
      Gnuplot p; //This ActivePlotter has its own GNUPlot instance
      Active a; //Remember to put this last, always, for destruction order purposes.
   };

} //namespace Aftr