#include <iostream>
#include <memory>


class IFaceBinaryOperator
{public:
   virtual double run( double a, double b ) = 0;
};

class AddNums : public IFaceBinaryOperator
{public:
   double run( double a, double b ) override { return a + b; }
};

struct MultiplyNums : public IFaceBinaryOperator
{  
   double run( double a, double b ) override { return a * b; }
};

class DivideNums : public IFaceBinaryOperator
{public:   
   double run( double a, double b ) override { return a / b; }
};

class NumberCruncher
{
public:
   NumberCruncher( std::unique_ptr< IFaceBinaryOperator> strategy ) : algorithm( std::move( strategy ) ) {}
   ~NumberCruncher() = default;
   
   double operator() ( double a, double b ) { return this->algorithm->run( a, b ); }
   double crunchNumbers( double a, double b ) { return (*this)( a, b ); }
      
private:
   std::unique_ptr< IFaceBinaryOperator > algorithm = nullptr;
};

int main( int argc, char* argv[] )
{
   std::cout << "Hello World...\n";
   double a = 9;
   double b = 4;
   
   NumberCruncher adder{ std::make_unique< AddNums >() };
   NumberCruncher multiplier{ std::make_unique< MultiplyNums >() };
   NumberCruncher divider{ std::make_unique< DivideNums >() };
   
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

