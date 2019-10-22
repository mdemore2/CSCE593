#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

template< typename T >
void add( T& x )
{
   x += 3;
}

void someOneElsesCode( const std::function< double(double, double) >& myBehavior )
{
   //other people's important algorithms that you're not concerned with
   //...
   //int x = 3; //this x is not used by myBehavior. myBehavior only uses
   //the one it already captured by reference
   double q = 8, r = 7, s = 0;
   s = myBehavior( q, r);
   std::cout << "s is " << s << "\n";
   //important code uses s here, based on the function your passed in in main.
}

int main()
{
   std::vector< int > v = { 1,2,3,4,5 };
   std::vector< int > w = { 90,80,70,60,50 };
   for( auto& x : v ) std::cout << x << " "; std::cout << "\n";
   
   for( auto& x : v ) add( x );
   std::for_each( v.begin(), v.end(), [](auto& x) { std::cout << x << " "; } ); std::cout << "\n";
   
   
   std::for_each( v.begin(), v.end(), [](auto& x) { x+= 3; } );   
   for( auto& x : v ) std::cout << x << " "; std::cout << "\n";
   
   
   std::for_each( v.begin(), v.end(), [](auto& x) { x+= 3; } );   
   for( auto& x : v ) std::cout << x << " "; std::cout << "\n";
   
   int x = 5;
   
   std::function< double( double, double ) > f = [&x] (double a, double b)
   {
      if( x == 5 )
         return a * b;
      else
         return a + b;
      
   };
   
   std::cout << "Calling f: " << f(3,4) << "\n";
   x = 4;
   std::cout << "Calling f: " << f(3,4) << "\n";
   
   
   someOneElsesCode( f );
   
   std::vector< std::string > words = {"dog", "fish", "cat", "homework", "zebra", "alligator"};
   std::sort( words.begin(), words.end(), []( const std::string& left, const std::string& right )
   {
     return left < right; 
   } );
   
   for( auto& v : words )
      std::cout << v << " ";
   std::cout << "\n";
   
   return 0;
}