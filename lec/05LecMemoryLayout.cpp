#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <memory>
#include <array>

class Vector
{
   public:
   float x,y,z = 0;
   Vector() : x(0), y(0), z(0) {}
   Vector( float x, float y, float z ) : x(x), y(y), z(z) {}
   std::string toString() const
   {
      std::stringstream ss;
      //only print 2 sig digits, using fixed notation as opposed to scientific notation.
      ss << std::setprecision(2) << std::fixed << "("<<x<<","<<y<<","<<z<<")";
      return ss.str();      
   }
};

void foo( const Vector& v )
{
   std::cout << "foo's v is " << v.toString() << "\n";
}

void fooPtr( Vector* p )
{
   std::cout << "I am an old school C function... Modern programmers think I'm antiquated, but I like my horse and buggy\n";
   std::cout << "PS (get it? buggy??? lmao), p is " << p->toString() << "\n";
}

void bar( std::unique_ptr< Vector > v )
{
   std::cout << "I OWN V, when i exit, v is destroyed... muhahaha... ha\n";
   std::cout << "   let v have one last toString " << v->toString() << "\n";
}

int main( int argc, char* argv[] )
{
   std::cout << "Okay folks, let's get started...\n";
   
   Vector a;
   Vector* pA = &a;
   pA->x = 1.0f;
   
   std::cout << "a  is " << a.toString() << "\n";
   std::cout << "pA is " << pA->toString() << "\n";
   
   std::cout << "mem addr of a is      " << &a << "\n";
   std::cout << "pA points at mem addr " << pA << "\n";
   std::cout << "mem addr of pA is     " << &pA << "\n";
   
   foo( a );
   foo( *(&a) );
   foo( *pA );
   
   
   std::cout << "a  is " << a.toString() << "\n";
   std::array<float,3> ary = {3.0,4.0,5.0};
   std::copy( std::begin( ary ), std::end( ary ), &pA->x );
   std::cout << "a  is " << a.toString() << "\n";
   std::cout << "\n";
   
   // ary = {4.77,4.88,4.99};
   // std::copy( std::begin( ary ), std::begin( ary ) + 2, &pA->x );
   // std::cout << "a  is " << a.toString() << "\n";
   // std::cout << "\n";
   
   
   // float* p = &a.y;
   // std::cout << "p is " << *p << "\n";
   // --p; //move left by sizeof( float ) bytes
   // Vector* cast = reinterpret_cast< Vector* >( p );
   // std::cout << "cast is " << cast->toString() << "\n";
   // std::cout << "\n";
   
   
   // auto uPtr = std::make_unique< Vector >( 9,8,7 );
   // if( uPtr != nullptr )
      // std::cout << "Happy uPtr is " << uPtr->toString() << "\n";
   
   // bar( std::move( uPtr ) );
   
   // if( uPtr == nullptr )
      // std::cout << "uPtr is no more... :(\n";
   
   
   // fooPtr( &a );
   // fooPtr( pA );
   
   return 0;
}