#include <iostream>
#include <algorithm>
#include <typeinfo>
#include <vector>
#include <array>
#include <unordered_map>
#include <deque>


int main( int argc, char* argv[] )
{
   std::cout << "Hello World...\n";
   
   std::array< int, 10 > a = { 22,8,5,34,12,54,67,34,3,7 };
   
   auto p = [](const auto& container )
   {
      std::cout << "Looking at a " << typeid( container ).name() << ": ";
      for( auto it = std::begin( container ); it != std::end( container ); it++ )
         std::cout << *it << " ";
      std::cout << "\n";
   };
   
   p( a );
   auto f = std::find( std::begin( a ), a.end(), 5 );
   if( f != a.end() )
      std::cout << "Found element at index " << std::distance( a.begin(), f ) << "\n";
   
   auto minmax = std::minmax_element( std::begin(a), std::end(a) );
   std::cout << "Min is " << *minmax.first << ", Max is " << *minmax.second << "\n";
   
   //assume we have to find multiple numbers from this collection...
   //lets create a binary tree and use a log n search
   std::sort( a.begin(), a.end() );
   std::vector< int > needles = {1,3,5}; //we are searching to find these "needles" in "a" haystack
   for( auto n : needles )
   {
      if( std::binary_search( a.begin(), a.end(), n ) )
         std::cout << "Found a " << n << "\n";
      else
         std::cout << n << " is not there...\n";
   }
   
   //Let's actually get an iterator to the element
   //this is still log n search
   //Returns an iterator pointing to the first element in the range [first, last)
   //that is not less than (i.e. greater or equal to) value, or last if no such
   //element is found.
   for( auto n : needles )
   {
      auto it = std::lower_bound( std::begin(a), std::end(a), n );
      if( it != a.end() && *it == n )
         std::cout << "Have iterator to " << *it << "\n";
      else
         std::cout << "Couldn't find " << n << "\n";
   }
   
   //Let's count number of odd elements in a
   int numOdd = std::count_if( a.begin(), a.end(), [](auto v){ return v % 2 == 0; } );
   std::cout << "Number of odd numbers is " << numOdd << "\n";
   
   //tes if it contains a "big" number
   int numBigs = std::count_if( a.begin(), a.end(), [](auto v){ return v > 20; });
   std::cout << "Number of big numbers is " << numBigs << "\n";
   
   //Google Challenge Problem
   std::vector< int > first  = {2,4,6,8,10,12,14,16,18,20,5,10,15,20,25,30,35,46,49,48,47,43,45,41,42,31,32,33,34,35,36,37};
   std::vector< int > second = {2,4,6,8,10,12,14,16,18,20,5,10,15,20,25,30,35,46,45,47,43,49,41,48,31,32,33,34,35,36,37};
   std::sort( first.begin(), first.end() );
   std::sort( second.begin(), second.end() );
   std::vector< int > diff;
   std::set_symmetric_difference( std::begin(first), std::end(first), std::begin(second), std::end(second), std::back_inserter( diff ) );
   for( auto n : diff ) std::cout << " a difference is " << n << "\n";
   
   
   return 0;
}