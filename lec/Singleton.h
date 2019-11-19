#pragma once
#include <string>
#include <map>

class Singleton
{
public:
   static std::string getVal( const std::string& key );
   static std::string setVal( const std::string& key, const std::string& val );
   static std::string toString();

private:
   Singleton(); //default constructor
   static Singleton& instance();
   Singleton( const Singleton& ) = delete; //copy constructor
   Singleton( Singleton&& ) = delete; //move constructor
   Singleton& operator=( const Singleton& ) = delete; //assignment operator
   Singleton& operator=( Singleton&& ) = delete; //move assignment operator
   
   static Singleton* s; //declared here, but memory storage goes only in one translation unit
   
   std::map< std::string, std::string > map;
};