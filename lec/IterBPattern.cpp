#include <iostream>
#include <algorithm>
#include <typeinfo>
#include <vector>
#include <array>
#include <unordered_map>
#include <deque>


std::ostream& operator<< ( std::ostream& os, const std::pair< const std::string&, int > p )
{
	os << p.first << ", " << p.second << "\n";
	return os;
}

int main( int argc, char* argv[] )
{
   std::cout << "Hello World...\n";
   
   std::array< int, 10 > a = { 22,8,5,34,12,54,67,34,3,7 };
   std::vector< int > v =  { 22,8,5,34,12,54,67,34,3,7 };
   std::unordered_map< std::string, int > m = { {"watson", 23}, {"nash", 27}, {"volt", 18}, {"aurora", 4} };
   
   auto p = [](const auto& container )
   {
      std::cout << "Looking at a " << typeid( container ).name() << "\n";
      for( auto it = std::begin( container ); it != std::end( container ); it++ )
         std::cout << *it << " ";
      std::cout << "\n";
   };
   
   p(a);
   p(v);
   p(m);
   auto f = std::find( std::begin( a ), a.end(), 5 );
   if( f != a.end() )
      std::cout << "Found element at index " << std::distance( a.begin(), f ) << "\n";
   
   
   std::string s{ "hello csce 593" };
   std::cout << "unaltered s is \"" << s << "\"\n";
   p( s );
   std::transform( std::begin(s), s.end(), s.begin(), [](char c) -> char { return std::toupper(c); } );
   std::cout << "A transformed s is \"" << s << "\"\n";
   std::vector< int > asciiVals;
   std::transform( s.begin(), s.end(), std::back_inserter( asciiVals ), [](auto c) -> int { return static_cast<int>(c); } );
   p( asciiVals );
   
   std::vector< int > asciiValsAllocated( s.length() );
   std::transform( s.begin(), s.end(), std::begin( asciiValsAllocated ), [](auto c) -> int { return static_cast<int>(c); } );
   p( asciiValsAllocated );
   
   int q = 3;
   std::cout << "type of q is " << typeid(q).name() << "\n";
   decltype(q) r = q + 1;
   std::cout << "type of r is " << typeid(r).name() << "\n";
   
   auto myV = std::vector<float>{};
   const std::vector<float>& rV = myV;
   std::remove_reference_t<decltype( rV )>::value_type t;
   std::cout << "Type of vector is " << typeid(t).name() << "\n";
   
   auto cp = [](const auto& container )
   {
      std::cout << "Looking at a " << typeid( container ).name() << "\n";
      typename std::remove_reference_t< decltype( container ) >::value_type t;
      std::cout << "This container contains " << typeid( t ).name() << "\n";
      for( auto it = std::begin( container ); it != std::end( container ); it++ )
         std::cout << *it << " ";
      std::cout << "\n";
   };
   
   cp( myV );
   
   
   
   return 0;
}