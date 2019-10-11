#include <iostream>
#include <string>

class IFaceTransportMethod
{public:   
   virtual void move() = 0; 
};

class Walk : public IFaceTransportMethod
{public:
   virtual void move() override { std::cout << "I am walking...\n"; }
};

class Drive : public IFaceTransportMethod
{public:
   virtual void move() override { std::cout << "I am driving...\n"; }
};

class Fly : public IFaceTransportMethod
{public:
   virtual void move() override { std::cout << "I am flying...\n"; }
};

class Shipment
{
public:
   Shipment( IFaceTransportMethod& method ) : strategy(method) {}
   virtual ~Shipment() = default;
   virtual float getBill() { return 50.00f; }
   
   void transport() { this->strategy.move(); }
private:
   IFaceTransportMethod& strategy;
};

class IFaceInsurance
{public:
   virtual float getInsurancePremium( float totalPriceWithoutInsurance ) = 0;
};

class SafeShipment : public Shipment, private IFaceInsurance
{
public:   
   SafeShipment( IFaceTransportMethod& method ) : Shipment( method ) {}
   virtual ~SafeShipment() = default;   
   virtual float getBill() override { return this->getTotalWithoutInsurance() + this->getInsurancePremium( this->getTotalWithoutInsurance() ); }
   //std::string getBillingInfo() { std::string s = "W/O Insurance: " + std::to_string( this->getTotalWithoutInsurance() ) + ". Premium Cost: " + std::to_string( this->getInsurancePremium( this->getTotalWithoutInsurance() ) ); return s; }
private:
   virtual float getInsurancePremium( float totalPriceWithoutInsurance ) override { return 0.25f * totalPriceWithoutInsurance; }
   float getTotalWithoutInsurance() { return this->Shipment::getBill() + 7.00; }
};

void processShipment( Shipment& shipment )
{
   shipment.transport();
}

int main()
{
   //Walk walk;
   //Drive drive;
   Fly fly;
   
   //Shipment shipmentA{ drive };
   SafeShipment shipmentB{ fly };
   //processShipment( shipmentA );
   //std::cout << "Please pay " << shipmentA.getBill() << "\n";
   processShipment( shipmentB );   
   std::cout << "Please pay " << shipmentB.getBill() << "\n";
   //std::cout << shipmentB.getBillingInfo() << "\n";
   
   return 0;
}
