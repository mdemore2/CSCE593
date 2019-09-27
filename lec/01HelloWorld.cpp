#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main(int argc, char *argv[])
{
   std::cout << "Hello World... Let's process arguments from the command\n"
             << "line passed into this program\n";

   std::cout << "Number of arguments is " << argc << "\n";

   std::vector<std::string> args(argv, argv + argc);

   //for( int i = 0; i < argc; ++i )
   //   args.push_back( std::string{argv[i]} );

   for ( auto const& s : args)
      std::cout << s << "\n";

   for (int i = 0; i < args.size(); ++i)
      std::cout << "args[" << i << "] = " << args[i] << "\n";

   auto printVec = [] ( std::vector<std::string> const& v )
   {
      for (int i = 0; i < v.size(); ++i)
         std::cout << "v[" << i << "] = " << v[i] << "\n";
   };

   // //Now let's sort the input params lexigraphically
   // //std::function< bool(std::string const&, std::string const&) > pred =  [](auto const& a, auto const& b )
   auto pred = [](auto const& a, auto const& b )
   {
      if( a < b ) return true;
         return false; 
   };

   std::sort( std::begin( args ), std::end( args ), pred );

   printVec( args );
   return 0;
}