#include <iostream>
#include <memory>
#include <string>

class Car
{
public:

template< typename T >
static T* New()
{
   static int serialNum = 0;
   T* car = new T();
   car->setSerialNumber( ++serialNum );
   car->createEngine();
   return car;
}

void setSerialNumber( int sn ) { this-> sn = sn; }
virtual std::string getType() { return "Default Car"; }
virtual void createEngine()
{
   std::cout << "Building default engine...\n";
}
int sn = 0;
   
};

class BlueCar : public Car
{
public:
   virtual std::string getType() { return "Blue Car, Serial Num " + std::to_string(this->sn); }
   virtual void createEngine()
   {
      std::cout << "Building Warp Drive engine...\n";
   }
};




int main( int argc, char* argv[] )
{
   
   Car* c = Car::New<Car>();
   BlueCar* b = Car::New<BlueCar>();
   
   return 0;
}