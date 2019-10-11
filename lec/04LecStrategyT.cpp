#include <iostream>
#include <memory>


class AddNums
{
public:
   double run( double a, double b ) { return a + b; }
};

struct MultiplyNums
{  
   double run( double a, double b ) { return a * b; }
};

class DivideNums
{
public:   
   double run( double a, double b ) { return a / b; }
};

template< typename HeresMyStrategy >
class NumberCruncher
{
public:
   NumberCruncher() = default;
   ~NumberCruncher() = default;
   
   double operator() ( double a, double b ) { return this->algorithm.run( a, b ); }
   double crunchNumbers( double a, double b ) { return (*this)( a, b ); }
      
private:
   HeresMyStrategy algorithm;
};

int main( int argc, char* argv[] )
{
   std::cout << "Hello World...\n";
   double a = 9;
   double b = 4;
   
   NumberCruncher< AddNums > adder{};
   NumberCruncher< MultiplyNums > multiplier{};
   NumberCruncher< DivideNums > divider{};
   
   //Here we simply call a "traditional method"
   std::cout << adder.crunchNumbers( a,b ) << "\n";
   std::cout << multiplier.crunchNumbers( a,b ) << "\n";
   std::cout << divider.crunchNumbers( a,b ) << "\n";

   //Here we invoke the operator() overload that consumes two doubles and returns a double.
   std::cout << "\nLet's use the operator() on our NumberCruncher...\n";
   std::cout << adder( a,b ) << "\n";
   std::cout << multiplier( a,b ) << "\n";
   std::cout << divider( a,b ) << "\n";

   return 0;
}

