#include <iostream>
#include <memory>
#include <string>

class Engine
{public: virtual std::string toString() const = 0; };

class A : public Engine
{public: virtual std::string toString() const { return "Engine A"; }; };

class B : public Engine
{public: virtual std::string toString() const { return "Engine B"; }; };

class Car
{
public:   
   virtual ~Car() { delete e; e = nullptr; };
   virtual void initEngine()
   {
      this->e = new A();
      std::cout << "Car::initEngine: " << this->e->toString() << "...\n";
   }
   void setSerialNumber( int vin ) { this->vin = vin; }
   virtual std::string toString() const { return "Car (" + std::to_string(vin) + ")..." + (e? e->toString() : "null") + "\n"; };
protected:
   Car() { }//this->initEngine(); }
   int vin = 0;
   Engine* e = nullptr;
};

class MyCar : public Car
{
public:
   static MyCar* New()
   {
      MyCar* c = new MyCar();
      c->initEngine();
      return c;
   }
   MyCar() : Car()
   {
      //this->initEngine();
   }
   virtual ~MyCar() { }
   virtual void initEngine() //toggle virtual
   {
      this->e = new B();
      std::cout << "MyCar::initEngine: " << this->e->toString() << "...\n";
   }
   virtual std::string toString() const { return "MyCar..." + Car::toString() + "\n"; } //tog vir
};

int main( int argc, char* argv[] )
{
   //Car* c = new MyCar();
   //Car* cc = new Car();
   Car* c = MyCar::New();
   //c->initEngine();
   std::cout << c->toString() << "\n";
   //std::cout << cc->toString() << "\n";
   return 0;
}
