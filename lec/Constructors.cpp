#include <iostream>
#include <sstream>
#include <vector>

//Nasty macros
#define LINE (std::string(__FILE__ " ") +  __FUNCTION__ + " " + std::to_string(__LINE__))

void out( const std::string& s )
{ std::cout << s << "\n"; }

class Student
{
   public:
   Student()  { out(LINE); }
   ~Student() { out(LINE); }
   Student( const Student& toCopy ) { out(LINE); *this = toCopy; }
   Student& operator=( const Student& toCopy )
   {
      if( this != &toCopy ) //give example where we remove this protection
      {
         out( LINE );
         this->name = toCopy.name;
         this->age = toCopy.age;
         this->loans = toCopy.loans;
      }
   }
   Student( Student&& toMove ) noexcept { out(LINE); *this = std::move( toMove ); }
   Student& operator=( Student&& toMoveAssign ) noexcept{
      out(LINE);
      if( this != &toMoveAssign ) //give example where we remove this protection
      {
         this->name = std::move( toMoveAssign.name );
         this->age = std::move( toMoveAssign.age );
         this->loans = std::move( toMoveAssign.loans );
      }
   }

   std::string toString() const { std::stringstream ss; ss << name << ", age " << age << ", $"; 
                                  for( auto& v : loans ) ss << v << " "; ss << "\n"; return ss.str(); }

      std::string name = "unnammed";
      int age = -1;
      std::vector< long long int > loans = { 99, 88, 7777777 };
};


int main( int argc, char* argv[] )
{
   Student a; a.name = "jenny"; a.age = 22; a.loans.push_back(87654);
   std::cout << a.toString();
   Student b;
   Student c( std::move(b));
   Student d = a;
   std::cout << d.toString();
   std::cout << a.toString();
   b = std::move( a ); //a is now "undefined", don't rely on any variables inside it...
   std::cout << b.toString();
   std::cout << a.toString();
   out(LINE); b = b;
   out(LINE); b = std::move(b);
   std::cout << b.toString();

   return 0;
}