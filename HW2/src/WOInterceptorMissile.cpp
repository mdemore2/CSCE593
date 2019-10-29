#include "WOInterceptorMissile.h"
#include "Model.h"
#include "WOGJoint.h"
#include <cmath>
#include <iostream>

namespace Aftr
{

WOInterceptorMissile* WOInterceptorMissile::New()
{
   //1. invoke private constructor for a new WOInterceptorMissile. In this factory design static method, you will use the 'new' keyword and not 
   //   use a unique ptr.

	auto missile = new WOInterceptorMissile;


   //2. invoke onCreate method passing in the shared multimedia path (SMM) "/models/DefenseDaemon/Launcher/Missile.3ds", a scale of 1,1,1, and the default shading type (auto).
   //   This approach gives polymorphic behavior to a "constructor". It let's all WO's polymorphically compose the type of their specific Model.

   //missile->onCreate( ManagerEnvironmentConfiguration::getSMM() + "/models/DefenseDaemon/Launcher/Missile.3ds", Vector{ 1,1,1 }, MESH_SHADING_TYPE::mstAUTO );     
	missile->onCreate("../../../shared/mm//models/DefenseDaemon/Launcher/Missile.3ds", { 1,1,1 }, Aftr::MESH_SHADING_TYPE::mstAUTO);
   
	//3. return the WOInterceptorMissile*

   return missile;

}

void WOInterceptorMissile::onCreate( const std::string& modelFileName, const Vector& scale, const MESH_SHADING_TYPE& mstMeshShadingType )
{
   //Do NOT modify this method
   WO::onCreate( modelFileName, scale, mstMeshShadingType );
}

WOInterceptorMissile::WOInterceptorMissile() : IFace( this ), WO()
{
}

WOInterceptorMissile::~WOInterceptorMissile()
{
}

void WOInterceptorMissile::setMotionStrategy( std::unique_ptr< MotionStrategyIFace > strategy, double range_m, double totalTime_sec, double headingDeg )
{
   //populate all related member variables.
   //When a unique_ptr is passed in, you will have to use the move semantics to perform an assignment. This explicitly transfers
   //ownership and enforces an unambiguous owner of the memory pointed to by the unique_ptr.
	this->motionAlgorithm = std::move(strategy);
	this->range_m = range_m;
	this->totalTime_sec = totalTime_sec;
	this->headingDeg = headingDeg;
}

double WOInterceptorMissile::getMotionStrategyLaunchAngleDeg()
{
   //use the strategy object to compute and return the launch angle.
   return this->motionAlgorithm->getLaunchAngle(range_m,totalTime_sec);
}

std::string WOInterceptorMissile::getTrajectoryInfo()
{
   //use the strategy object to compute and return the launch angle.
   //A sample output using the default params in the conf file will look like this:

	std::string output;
   output += "Printing Trajectory info...\n";
   output += this->motionAlgorithm->toString(range_m,totalTime_sec,headingDeg);
   //V_horz( m / s ) is  22.5166
   //V_vert( m / s ) is  12.9983
   //Max Alt( m )  is 8.61134
   //Landing Position is( 59.669, 0.000, 0.000 )

   return output;
}

void WOInterceptorMissile::onUpdateWO()
{
   //This method does NOT change. Do NOT modify this method!
   if( this->state == MISSILE_STATE::FLYING )
      this->updateMissileTrajectory();
   else if( this->state == MISSILE_STATE::DETONATING )
   {
      std::cout << "KAAAABOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM!!!!\n\n\n\n";
      this->state = MISSILE_STATE::INERT;
   }
}

void WOInterceptorMissile::updateMissileTrajectory()
{
   //Read up on Howard Hinnant's new std::chrono library... This library is an excellent example of modern generic programming.
   std::chrono::duration< double, std::micro > deltaT_uSec = std::chrono::system_clock::now() - this->launchTime;
   double deltaTSec = deltaT_uSec.count() / 1'000'000; //convert microseconds to seconds by dividing by a million
   
   Vector newPosition; //will store the new position returned by the strategy evaluated the the current time step
   bool reachedTarget = false; //will store the boolean return value from the strategy

   //Use the strategy pattern to compute the current position of our missile. The strategy will return both a position and a bool. If the bool is false,
   //the missile has not yet reached it target; otherwise, it will be true. Also, pay attention to the return value of the strategy -- read up about 
   //std::tuples and use the std::tie method to accept multiple return values from your strategy (yeah, you can do that w/ tuples and modern C++).
   std::tie(newPosition, reachedTarget) = this->motionAlgorithm->computePosition(deltaTSec,range_m,totalTime_sec,headingDeg);
   

   //At this point, you have the new position and boolean information from your strategy. Do NOT modify the remainder of this method.
   this->setPosition( newPosition );
   if( reachedTarget )
      this->state = MISSILE_STATE::DETONATING;
}

void WOInterceptorMissile::launch()
{
   //Launch the missile, but ensure the MISSILE_STATE remains consistent. See header for state info.
   //If the missile is already launched, it can't be launched again unless it is reset.
   //Also, if a launch occurs, in order to keep track of how much time has elapsed, we must 
   //set this->launchTime to the current system time using a chrono system clock.
	if (this->state == WOInterceptorMissile::MISSILE_STATE::AIMING)
	{
		this->state = WOInterceptorMissile::MISSILE_STATE::FLYING;
		this->launchTime = std::chrono::system_clock::now();
	}
}

void WOInterceptorMissile::reset()
{
   //This resets the missile. Set its position to the origin and ensure it's missile state is reset. 
	this->setPosition({ 0,0,0 });
	this->state = WOInterceptorMissile::MISSILE_STATE::AIMING;
}

} //namespace Aftr
