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
	float x = 0, y = 0, z = 0;	//pos vars to pass
	double sx = 0, sy = 0, sz = 0;	//pos vars to calc
	double vh = 0, vv = 0;	//vel vars to calc

	if (totalTime_sec <= 0) { totalTime_sec = 1; };
	vh = range_m / totalTime_sec;	//initial horizontal velocity
	vv = (GRAVITY * totalTime_sec)/2;	//initial vertical velocity --half time going up

	sx = vh * dt_sec * cos(headingDeg*PI/180);
	sy = vh * dt_sec * sin(headingDeg*PI/180);
	sz = (vv * dt_sec) - ((GRAVITY / 2) * pow(dt_sec, 2));	//changed to minus

	x = static_cast<float>(sx);
	y = static_cast<float>(sy);
	z = static_cast<float>(sz);

	if (dt_sec < totalTime_sec)
		return std::make_tuple(Vector{ x,y,z }, false);
	else
		return std::make_tuple(Vector{ x,y,z }, true);
}


//You've already computed everything you need to find this value in the method above. You can just recompute it here.
double MotionParabolic::getLaunchAngle( double range_m, double totalTime_sec )
{
	double vh = 0, vv = 0;
	double theta = 0;

	vh = range_m / totalTime_sec;	//initial horizontal velocity
	vv = GRAVITY * totalTime_sec;	//initial vertical velocity

	theta = tan(vv/vh);

   return theta;
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
	double vh = 0, vv = 0, maxh = 0, xf = 0, yf = 0;

	vh = range_m / totalTime_sec;	//initial horizontal velocity
	vv = GRAVITY * totalTime_sec;	//initial vertical velocity

	maxh = pow(vv, 2) / (2 * static_cast<double>(GRAVITY));

	xf = range_m * cos(headingDeg*PI/180);
	yf = range_m * sin(headingDeg*PI/180);

   //The output below can eventually be commented in and used, once those variables are declared and populated.

   //use a std::stringstream object to print out the following information
   std::stringstream ss;
   ss << "V_horz (m/s) is  " << vh << "\n";
   ss << "V_vert (m/s) is  " << vv << "\n";
   ss << "Max Alt (m)  is " << maxh << "\n";
   ss << "Landing Position is " << Vector{ static_cast<float>( xf ), static_cast<float>( yf ), 0 }.toString() << "\n";

   return ss.str();
}

} //namespace Aftr

