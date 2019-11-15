#include "Prototype.h"
#include <vector>

void testCopyAssign()
{
   WO* a = new WO();
   a->id = 73;
   a->name = "Aladdin";
   std::cout << "WO a is ";
   a->draw( std::cout );

   WO* b = new WO( *a ); //copy constructor
   std::cout << "WO b is ";
   b->draw( std::cout );
   b->id = 19;

   *a = *b; //assignment operator

}

void testMoveAssign()
{
   WO* c = new WO();
   c->id = 233;
   c->name = "Chucky";
   auto m = std::make_unique<FancyModel>();
   c->setModel( std::move( m ) );
   std::cout << "WO c is ";
   c->draw( std::cout );

   WO* d = new WO();
   d->id = 45;
   std::cout << "WO d is ";
   d->draw( std::cout );

   *d = std::move( *c );
   std::cout << "After move assignment...\n";
   d->draw( std::cout );
   c->draw( std::cout );

   
}

void testClone()
{
   std::vector< std::unique_ptr< WO > > wo;

   {
      std::unique_ptr< WO > w = std::make_unique<WO>();
      std::unique_ptr<QuickModel> m = std::make_unique<QuickModel>();
      w->setModel( std::move( m ) );
      w->id = 1;
      w->name = "Harry";
      wo.push_back( std::move( w ) );
      
   }

   {
      std::unique_ptr< WO > w = std::make_unique<WO>();
      std::unique_ptr<FancyModel> m = std::make_unique<FancyModel>();
      w->setModel( std::move( m ) );
      w->id = 2;
      w->name = "Gary";
      wo.push_back( std::move( w ) );
   }

   for( auto& w : wo )
      w->draw( std::cout );



   std::unique_ptr< WO > a = wo[0]->clone();
   std::unique_ptr< WO > b = wo[1]->clone();
   std::cout << "Just finished cloning...\n";
   a->draw(std::cout);b->draw(std::cout);
   wo.push_back( std::move( a ) );
   wo.push_back( std::move( b ) );

   for( auto& w : wo )
      w->draw( std::cout );

   std::cout << "Move constructor time...\n";

   WO* c = new WO();
   std::unique_ptr<FancyModel> m = std::make_unique<FancyModel>();
   c->setModel( std::move( m ) );
   c->id = 3;
   c->name = "Chuck";
   c->draw( std::cout );

   std::unique_ptr<WO> x = std::make_unique<WO>( std::move( *c ) );
   x->draw( std::cout );

   c->draw( std::cout );
}

int main( int argc, char* argv[] )
{
//   testCopyAssign();

  // testMoveAssign();


   testClone();   

   return 0;
}