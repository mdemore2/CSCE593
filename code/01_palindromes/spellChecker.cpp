#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <random>
#include <algorithm>

int main( int argc, char* argv[] )
{
   std::vector<std::string> args( argv, argv + argc );
   if( args.size() < 2 )
   {
      std::cout << "Spell Checker Program\n"
                << "Usage: ./spellChecker dictionary.txt textDocument.txt\n"
                << "This program will build a dictionary from the dictionary.txt file\n"
                << "In the future, this program will then spellcheck a textDocument.txt\n"
                << "and report all misspelled words.\n";
      return -1;
   }

   std::cout << "Spell checker will use dictionary file: " << args[1] << "\n";

   //create a vector containing strings called words, this will store all the words in the dictionary.
   
   std::vector<std::string> words;

   //read the in passed dictionary file and place each word into one element of the vector
  
   std::ifstream inFile;
   inFile.open(args[1]);
   if (!inFile) {
	   std::cerr << "Unable to open file " << args[1];
	   exit(1);   // call system to stop
   }

   std::string line;
   while (std::getline(inFile, line))
   {
	   words.push_back(line);
   }

      
   //After reading in the file and populating the vector, print out the size
   //verify you've read the same number of words as there are lines in the dictionary file.

   std::cout << "Read in " << words.size() << " words...\n";


   //now create a vector of strings to store the palindromes that may exist in the vector of words.
   //name this vector palindromes

   std::vector<std::string> palindromes;

   //create a lambda function that is a unary predicate operator that consumes a const std::string& s
   //and returns true iff s is a palindrome; false, otherwise.
   //name this lamda is_palindrome
   


   //use std::copy_if as your algorithm to traverse the words, evaluate the unary predicate determining
   //if the current word is a palindrome. If it is a palindrome, then copy into your vector listing
   //the palindromes. Note you will use an std::back_inserter( your_palindrome_vec ) as the 3rd argument
   //to copy_if. The 4th argument will be the unary predicate. The first 2 arguments will be the begin
   //and end iterators of the dictionary words.
   

   //lastly, use std::sort, to lexigraphcally sort your discovered palindromes (in alphabetical order).
   

   std::cout << "The palindromes, in sorted order, are:\n";

   //iterate over your sorted palindromes and print each one, one per line. Run my example for
   //expected output.

   

   return 0;
}