#include "CommandPatternMementoShapes.h"
#include <iostream>
#include <memory>

int main( int argc, char* argv[] )
{
   std::cout << "Command Pattern and Memento Pattern...\n";

   std::cout << "First we show how we can serialize hierarchial objects...\n";
   auto c = std::make_shared<Circle>();
   c->radius = 3.14f; c->color = 18; c->pos = 38.7f; c->orient = 12.0f;
   auto t = std::make_shared<Triangle>();
   t->base = 10.0f; t->height = 15.0f; t->color = 19; t->pos = 48.7f; t->orient = 22.0f;

   std::stringstream ss;
   c->toStream( ss );
   t->toStream( ss );
   std::cout << ss.str() << "\n";

   std::cout << "Second we show how we can store the serialized hierarchial objects in a Memento/serialized instance...\n";
   Memento< std::stringstream > circState;
   Memento< std::stringstream > triState;
   circState.save( *c );
   triState.save( *t );

   auto printShape = [](const Shape& s ) { std::stringstream ss; s.toStream( ss ); return ss.str(); };

   c = std::make_shared<Circle>();
   t = std::make_shared<Triangle>();
   std::cout << "Now state is\n" << printShape(*c) << printShape(*t) << "\n";

   std::cout << "Third deserialize from our data store and verify the objects are back to their original state\n";
   circState.restore( *c );
   triState.restore( *t );
   ss = std::stringstream();
   c->toStream( ss );
   t->toStream( ss );
   std::cout << "Lastly state is\n" << printShape(*c) << printShape(*t) << "\n";

   std::cout << "\n\n\nNow we can use our memento patter with the command pattern!\n";
   auto cmdPos = std::make_shared<CmdSetShapePosition>( c, 101.0f );
   cmdPos->execute();
   std::cout << "After execute: " << printShape( *c ) << "\n";
   cmdPos->unexecute();
   std::cout << "After unexecute: " << printShape( *c ) << "\n";

   auto cmdTriHeight = std::make_shared<CmdSetTriangleHeight>( t, 9999.0f );
   cmdTriHeight->execute();
   std::cout << "After execute: " << printShape( *t ) << "\n";
   cmdTriHeight->unexecute();
   std::cout << "After unexecute: " << printShape( *t ) << "\n";

   return 0;
}