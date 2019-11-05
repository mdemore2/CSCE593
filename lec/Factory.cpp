#include <iostream>
#include <memory>
#include <string>

class Car
{
public:
   void setSerialNumber( int sn ) { this-> sn = sn; }
   virtual std::string getType() = 0;
protected:
   int sn = 0;
   
};

class BlueCar : public Car
{
public:
   virtual std::string getType() { return "Blue Car, Serial Num " + std::to_string(this->sn); }
};

class CarFactory
{
public:
   
   std::unique_ptr< Car > New()
   {
      std::unique_ptr< Car > c = make_car();
      c->setSerialNumber( ++this->serialNum );
      return std::move( c );      
   }   
private:
   virtual std::unique_ptr< Car > make_car() = 0;
   int serialNum = 0;
};

class BlueCarFactory : public CarFactory
{
  virtual std::unique_ptr< Car > make_car()
  {
     std::unique_ptr< BlueCar > bc = std::make_unique< BlueCar >();
     return std::move( bc );
  }
};

int main( int argc, char* argv[] )
{
   std::shared_ptr< CarFactory > factory = std::make_shared<BlueCarFactory>();
   
   for( int i = 0; i < 10; ++i )
   {
      auto c = factory->New();
      std::cout << "c is " << c->getType() << "\n";
   }
   
   return 0;
}