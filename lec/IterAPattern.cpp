#include <iostream>
#include <algorithm>
#include <typeinfo>
#include <vector>
#include <array>
#include <unordered_map>
#include <deque>
#include <fstream>
#include <string>


std::ostream& operator<< ( std::ostream& os, const std::pair< std::string, int >& p )
{
   os << p.first << ", " << p.second << "\n";
   return os;
}

int main( int argc, char* argv[] )
{
   std::cout << "Hello World...\n";
   
   std::array< int, 10 > a = { 22,8,5,34,12,54,67,34,3,7 };
   std::vector< int > v =  { 22,8,5,34,12,54,67,34,3,7 };
   std::deque< int > s = { 22,8,5,34,12,54,67,34,3,7 };
   std::unordered_map< std::string, int > m = { {"watson", 23}, {"nash", 27}, {"volt", 18}, {"aurora", 4} };
   
   auto p = [](const auto& container )
   {
      std::cout << "Looking at a " << typeid( container ).name() << ": ";
      for( auto it = std::begin( container ); it != std::end( container ); it++ )
         std::cout << *it << " ";
      std::cout << "\n";
   };
   
   p( a );
   std::sort( std::begin( a ), std::end( a ) );
   p( a );
   
   p( v );
   std::sort( std::begin( v ), std::end( v ), std::greater< int >() );
   p( v );
   
   p( s );
   std::sort( std::begin( s ), std::end( s ), std::less< decltype(v)::value_type >() );
   p( s );
   
   //std::function< std::string( const container::value_type& )
   auto pm = [](const auto& container, auto printVal )
   {
      std::cout << "Looking at a " << typeid( container ).name() << ": ";
      for( auto it = std::begin( container ); it != std::end( container ); it++ )
         printVal( *it );
      std::cout << "\n";
   };
   
   pm( m, []( const decltype(m)::value_type& t )
   {
      std::cout << t.first << " = " << t.second << "; ";
   } );
   
   pm( v, []( const decltype(v)::value_type& t ){std::cout<< t << "  ";} );
   
   p( m );
   
   
   return 0;
}