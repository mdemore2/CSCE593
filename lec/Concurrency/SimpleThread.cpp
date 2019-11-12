#include <iostream>
#include <thread>

void worker( std::string data, int count )
{
    while( count > 0 )
    {
       std::cout << "Count is: " << count << ", Data is " << data << "\n";
       std::this_thread::sleep_for( std::chrono::milliseconds(10) );
       --count;
    }
}

int main( int argc, char* argv[] )
{
    std::cout << "In main thread...\n";
    std::thread t( &worker, "I'm copied to the new thread", 100 );
    for( int i = 0; i < 100; ++i )
    {
        std::cout << " main thread i is " << i << "\n";
        std::this_thread::sleep_for( std::chrono::milliseconds(10) );
    }

    std::cout << "Main thread done working, waiting for worker to finish...\n";
    t.join();
    return 0;
}