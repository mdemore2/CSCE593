#pragma once

#include "MotionStrategy.h"

namespace Aftr
{

//Use physics, calculus, algebra, and grit to compute the new position...
//Also, ensure the bool return parameter gets set to TRUE once the missile reaches its target; until that time, it needs to be FALSE.
//Read up on std::tuple and std::tie to learn how C++ can return multiple parameters.
//Also, ensure this code doesn't divide by zero or behave unexpectedly if bad input is given.
//Also, you are computing stuff with doubles. At the end of this method use static_cast<float>( myDouble ) to cast a double to 
//a float.
std::tuple<Vector,bool> MotionParabolic::computePosition( double dt_sec, double range_m, double totalTime_sec, double headingDeg )
{
   return std::make_tuple( Vector{0,0,0}, false );
}


//You've already computed everything you need to find this value in the method above. You can just recompute it here.
double MotionParabolic::getLaunchAngle( double range_m, double totalTime_sec )
{
   return 0;
}

std::string MotionParabolic::toString( double range_m, double totalTime_sec, double headingDeg ) const
{
   //Expected output looks like this:
   //Printing Trajectory info...
   //V_horz( m / s ) is  22.5166
   //V_vert( m / s ) is  12.9983
   //Max Alt( m )  is 8.61134
   //Landing Position is( 59.669, 0.000, 0.000 )

   //do some math here...


   //The output below can eventually be commented in and used, once those variables are declared and populated.

   //use a std::stringstream object to print out the following information
   std::stringstream ss;
   //ss << "V_horz (m/s) is  " << V_horz << "\n";
   //ss << "V_vert (m/s) is  " << V_vert << "\n";
   //ss << "Max Alt (m)  is " << maxAltM << "\n";
   //ss << "Landing Position is " << Vector{ static_cast<float>( xComp ), static_cast<float>( yComp ), 0 }.toString() << "\n";

   return ss.str();
}

} //namespace Aftr

