#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <regex>
#include <algorithm>
#include <optional>
#include <sstream>

//DUE on 18 Oct 2019 @ 0930 WPAFB time.

//Use the dictionary that already exists in your repo at: ../01_palindromes/randomDict.txt
//Simply use the above as the relative path to the dictionary location to save hard drive space.

//To compile on g++: g++ -std=c++17 spellChecker.cpp
//  To run on linux: ./a.out ../01_palindromes/randomDict.txt

//To run in debug mode and find the dictionary, set your solution to use command line arguments:
//MSVC: Solution Explorer -> Right click on spellChecker -> Properties -> Configuration Properties -> Debugging -> Command Arguments: ../../01_palindromes/randomDict.txt

/*SAMPLE INPUT STRING YOU CAN COPY/PASTE: 
      fibrilation pseudotrimerous testing filee to try to amphicentric
*/
class SpellingResult
{
   public:
   SpellingResult() {};
   SpellingResult( const std::string& lower, const std::string& upper, const std::string& origWord )
   {
      //Populate the private member variables in this constructor
	   lowerBound_suggestion = lower;
	   upperBound_suggestion = upper;
	   orig = origWord;
   }

   std::string toString() const noexcept
   {
      //Write code that matches this pattern. Your output and behavior should match my output and behavior.
      //Note the difference between found and not found.
      //Also note the first suggestion is the word lexicograpically before the missing word
      //and the second suggestion is the word immediately after where the missing word would go.
         // Word: fibrilation was found...
         // Word: pseudotrimerous was found...
         // Word: testing *NOT* found. Suggestions: testingly or testitis
         // Word: filee *NOT* found. Suggestions: filelike or filemaker
         // Word: to *NOT* found. Suggestions: toa or toadback
         // Word: try *NOT* found. Suggestions: trye or tryer's
         // Word: to *NOT* found. Suggestions: toa or toadback
         // Word: amphicentric was found...

	   std::string formatOut = "Word: ";

	   if (lowerBound_suggestion != "") {
		   formatOut += orig;
		   formatOut += "*NOT* found. Suggestions: ";
		   formatOut += lowerBound_suggestion;
		   formatOut += " or ";
		   formatOut += upperBound_suggestion;
	   }
	   else {
		   formatOut += orig;
		   formatOut += " was found...";
	   }

	   return formatOut;
   }

   std::string lowerBound_suggestion;
   std::string upperBound_suggestion;
   std::string orig;
};

std::string trimWhiteSpace(const std::string& s); //utility function that removes leading and trailing white space

//This class encapsulates a dictionary and provides methods to check if a word exists or to check spelling.
class Dictionary
{
   public:
      static std::optional<Dictionary> New( const std::string& filePathToWords )
      {
         //This static ::New method is an exmplar of a Factory Design pattern...
         //Read up on std::optional<...>, you will use this to return a value, or optionally,
         //a null value known as nullopt. nullopt shall be returned when the file could not be properly
         //parsed.
         //Open are parse the file, creatings a std::vector<std::string> (just like last time)
         //the sort all the words and move them into a Dictionary instance.
         //return the dictionary.

		  std::vector<std::string> wordSet;
		  std::ifstream inFile;
		  std::string line;

		  inFile.open(filePathToWords);

		  if (!inFile) {
			  std::cerr << "Unable to open file " << filePathToWords;
			  exit(1);   // call system to stop
		  }

		  while (std::getline(inFile, line))
		  {
			  line = trimWhiteSpace(line);
			  wordSet.push_back(line);
		  }
		  std::sort(wordSet.begin(), wordSet.end());

		
		  Dictionary(wordSet);
		  if (Dictionary)
		  {
			  return Dictionary;
		  }
		  else
		  {
			  return {};
		  }

      }

      //Move constructor employs move semantics to efficiently emplace the in passed
      //large data base of words without performing a copy. Also, ownership is fully
      //transferred explicitly by virtue of the move semantics.
      Dictionary( std::vector<std::string>&& wordSet )
      {
         //use move semantics to move wordSet into this->words
		  this->words = wordSet;
      }

      //Destructor, called when the dictionary is destroyed / leaves scope.
      ~Dictionary() {}

      //Conducts an Order log N search for word w. Iff found, returns true; false, otherwise.
      SpellingResult exists_OrderLogN( const std::string& w ) const noexcept
      {
         //read up on std::lower_bound to efficiently perform a binary search
         //with only one line of code. Note the dictionary has already sorted
         //out words, which means a binary search will properly locate a word
         //if it exists; otherwise, it will located the spot adjacent.

         //This method will return a SpellingResult. A spelling result contains
         //the original word, and if not found, it will return a the word
         //lexicographically below and above where that word ought to reside.

         return SpellingResult{ "", "", w };
      }

      std::vector<SpellingResult> spellCheckLine( const std::string& input ) const noexcept
      {
         //first time the input's white space.
         std::string line = trimWhiteSpace( input );

         //This is a regular expression that divides the input at each space.
         //For example, the input: fibrilation pseudotrimerous testing filee to try to amphicentric
         //is broken into 8 individual words. Each word is checked individually.
         std::regex reg( "[ \t]+" );
         std::sregex_token_iterator it( line.begin(), line.end(), reg, -1);
         std::sregex_token_iterator end; //iterator that initializes itself to end; used for comparison against i.
         //we've created a regular expression to split the string each time we find a white space like a space or tab...

         //now iterate using for( ; it != end; it++ )... create a vector of Spelling Result
         // and invoke exists_OrderLogN. Push the spelling result of each word into the vector.
         //For testing and debugging, print out the value of the iterator at each loop iteration.
         
      }

      std::string toString() const noexcept
      {
         //use a std::stringstream to build an output that looks like:
         //"This dictionary contains 273642 words...\n". Obviously make the number
         //specific to your dictionary.
		  std::cout << "This dictionary contains " << this->words.size() << "words...\n";
      }

   private:
      std::vector<std::string> words; //must be alphabetically sorted
};


//Do *NOT* touch any code in main. But read through it carefully and learn it.
//You, of course, may modify the code for testing, but it *MUST* be identical
//to its current state when you submit it...
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

   std::optional<Dictionary> dictionary = Dictionary::New( args[1] );
   if( !dictionary )
   {
      std::cout << "Failed to create dictionary, exiting now...\n";
      return -1;
   }
      
   std::cout << dictionary->toString();

   std::cout << "Please input a string to spellcheck:\n";
   std::string input;
   std::getline( std::cin, input );

   auto results = dictionary->spellCheckLine( input );
   for( auto& w : results )
      std::cout << w.toString();
}

//Removes leading and trailing white space including
//spaces, tabs, \r, \n, etc (any non-printable ASCII character).
std::string trimWhiteSpace(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while( it != s.end() && std::isspace(*it) )
        it++;

    std::string::const_reverse_iterator rit = s.rbegin();
    while( rit.base() != it && isspace(*rit) )
        rit++;

    return std::string( it, rit.base() );
}