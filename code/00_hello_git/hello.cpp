#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/// HOW TO COMPILE ME:
/// user@zz:/mnt/c/repos/courses/593/2019/prof593/code/00_hello_git$ g++ -std=c++17 hello.cpp


/// This method creates a string of length N that is the value i, right justified and padded by
/// the in passed pad_char character. For example, if N=3 and i == 2, the return is 002. If
/// N=5 and i == 233, the return value is 00233.
std::string pad( int i, int N, char pad_char = '0' )
{
   std::string s = std::to_string(i);

   if( s.length() > N )
      return s;
   
   while( s.length() < N )
      s = pad_char + s;

   return s;
}

int main( int argc, char* argv[] )
{
   std::cout << "GIT is a great tool, it's also open source, and written by Linus Torvalds...\n"
             << "This program will open a file passed as the first parameter and print its contents\n"
             << "to stdout (the console)... It will insert line numbers at the beginning of each line...\n";

   std::cout << "argc=" << argc << "\n";
   std::vector< std::string > params( argv, argv + argc );
   for( auto& s : params )
      std::cout << "\"" << s <<"\"\n";

   if( params.size() < 2 )
   {
      std::cout << "User did not specify a file to open. Exiting now...\n";
      return -1;
   }

   std::ifstream fin( params[1] );
   if( !fin )
   {
      std::cout << "WARNING: Failed to open file " << params[1] << "... Does it exist and is it in the same directory as this executable?\n";
      return -1;
   }

   std::string line = "";
   for( int i = 0; std::getline( fin, line, '\n' ); ++i )
      std::cout << pad( i, 3, 'x' ) << ": " << line << "\n";      

   return 0;
}