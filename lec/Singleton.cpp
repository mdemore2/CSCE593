#include "Singleton.h"
#include <iostream>
#include <sstream>

Singleton* Singleton::s = nullptr;

Singleton::Singleton() {}

Singleton& Singleton::instance()
{
   if( Singleton::s == nullptr )
      Singleton::s = new Singleton();
   return *Singleton::s;
}

std::string Singleton::getVal( const std::string& s )
{
   return Singleton::instance().map[s];
}

std::string Singleton::setVal( const std::string& key, const std::string& val )
{
   auto old = Singleton::instance().map[key];
   Singleton::instance().map[key] = val;
   return std::move(old);
}

std::string Singleton::toString()
{
   std::stringstream ss;
   
   auto& m = Singleton::instance().map;

   for( auto& e : m )
      ss << "map[" << std::get<0>(e) << "] = " << std::get<1>(e) << "\n";
   
   return ss.str();
 
}