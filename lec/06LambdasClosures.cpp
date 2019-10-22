#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>


class GreatDane
{
   public:
   GreatDane( std::string const& dogName, int heightInches ) : name( dogName ), heightInches( heightInches ) {}
   std::string toString() const;   
   int getHeight() const { return this->heightInches; }
   std::string const& getName() const { return this->name; }
   private:
   std::string name = "";
   int heightInches = 0;
};

std::function< int( GreatDane const&, GreatDane const& ) > makeFunctionCapturingNoState()
{
   std::function< int( GreatDane const&, GreatDane const& ) > measureHeightAgainstEachOther = []( GreatDane const& a, GreatDane const& b )
   {
      std::cout << "Comparing " << a.getName() << " against " << b.getName() << "\n";
      return a.getHeight() - b.getHeight();
   };
   return std::move( measureHeightAgainstEachOther );
}

std::function< int( GreatDane const& ) > makeFunctionCapturingScope()
{
   GreatDane winston{ "Winston", 42 };
   GreatDane kate{ "Kate", 45 };
   GreatDane scooby{ "Scooby", 46 };

   std::function< int( GreatDane const& ) > heightAgainstKate = [&kate]( GreatDane const& d )
   {
      std::cout << "Comparing " << d.getName() << " against " << kate.getName() << "\n";
      return d.getHeight() - kate.getHeight();
   };

   //Let's test our height function:
   std::cout << heightAgainstKate( winston ) << "\n";
   std::cout << heightAgainstKate( kate ) << "\n";
   std::cout << heightAgainstKate( scooby ) << "\n";

   return std::move( heightAgainstKate );
} //what happens to ALL local variables when this functions returns???

int main()
{
   GreatDane henry{ "Henry", 4 };
   GreatDane indy{ "Indiana Jones", 8 };

   //What if we put "kate" back? This isn't really the captured Kate...
   //GreatDane kate{ "Kate", 99 };

   auto heightAgainstKate = makeFunctionCapturingScope();
   std::cout << "Now back in main()... Let's use our heighting function...\n";
   //std::cout << heightAgainstKate( henry );

   //auto heightAgainstEachOther = makeFunctionCapturingNoState();
   //std::cout << heightAgainstEachOther( henry, indy ) << "\n";
   
   return 0;
}