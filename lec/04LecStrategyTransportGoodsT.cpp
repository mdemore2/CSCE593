#include <iostream>

struct Walk
{ void move() { std::cout << "I am walking...\n"; } };

struct Drive
{ void move() { std::cout << "I am driving...\n"; } };

struct Fly
{ void move() { std::cout << "I am flying...\n"; } };

template< typename TransportMethod >
class Shipment
{
public:
   Shipment() = default;
   virtual ~Shipment() = default;
   virtual float getBill() { return 50.00f; }
   
   void transport() { this->strategy.move(); }
private:
   TransportMethod strategy;
};

class IFaceInsurance
{public:
   virtual float getInsurancePremium( float totalPriceWithoutInsurance ) = 0;
};

template< typename T >
class SafeShipment : public Shipment<T>, private IFaceInsurance
{
public:   
   SafeShipment() {};
   virtual ~SafeShipment() = default;   
   virtual float getBill() override { return this->getTotalWithoutInsurance() + this->getInsurancePremium( this->getTotalWithoutInsurance() ); }
private:
   virtual float getInsurancePremium( float totalPriceWithoutInsurance ) override { return 0.25f * totalPriceWithoutInsurance; }
   float getTotalWithoutInsurance() { return this->Shipment<T>::getBill() + 7.00; }
};

template< typename T >
void processShipment( Shipment<T>& shipment )
{
   shipment.transport();
}

int main()
{
   std::cout << "hi...\n";
   
   SafeShipment<Fly> shipment;
   processShipment( shipment );
   std::cout << "Please pay " << shipment.getBill() << "\n";
   
   return 0;
}