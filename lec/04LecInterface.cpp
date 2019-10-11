#include <iostream>
#include <memory>

class Trainable
{
public:   
   Trainable() = default;
   virtual ~Trainable() = default; //purely abstract class
   virtual void sit() = 0; //abstract method
   virtual void come() = 0; //abstract method
   virtual void retrainBehaviors() = 0;
};

class Dog : public Trainable
{
public:
   Dog() = default;
   virtual ~Dog() = default;   
   void bark() { std::cout << "Woof!\n"; }
   void digHole() { std::cout << "Only if I can get muddy before going in the house...\n"; }
   virtual void sit() { std::cout << this->currentState_onSit; }
   virtual void come() { std::cout << this->currentState_onCome; }
   
   virtual void retrainBehaviors() { this->currentState_onCome = "Roof... OMW...\n"; 
                                     this->currentState_onSit = "Sitting down like a good pup...\n"; }
   
private:
   std::string currentState_onCome = "Grrr!\n";
   std::string currentState_onSit = "Um... No!\n";
};

int main( int argc, char* argv[] )
{
   std::cout << "Hello World...\n";
   std::unique_ptr< Dog > d = std::make_unique< Dog >();   
   d->bark();
   d->come();
   d->come();
   d->sit();
   d->retrainBehaviors();
   d->come();
   d->sit();

   return 0;
}

