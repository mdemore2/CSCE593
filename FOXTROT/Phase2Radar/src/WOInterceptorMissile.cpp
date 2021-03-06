#include "WOInterceptorMissile.h"
#include "Model.h"
#include "WOGJoint.h"
#include <cmath>
#include <iostream>

namespace Aftr
{

WOInterceptorMissile* WOInterceptorMissile::New()
{
   WOInterceptorMissile* wo = new WOInterceptorMissile();
   wo->onCreate( ManagerEnvironmentConfiguration::getSMM() + "/models/DefenseDaemon/Launcher/Missile.3ds", Vector{ 1,1,1 }, MESH_SHADING_TYPE::mstAUTO );     
   return wo;
}

void WOInterceptorMissile::onCreate( const std::string& modelFileName, const Vector& scale, const MESH_SHADING_TYPE& mstMeshShadingType )
{
   WO::onCreate( modelFileName, scale, mstMeshShadingType );
}

WOInterceptorMissile::WOInterceptorMissile() : IFace( this ), WO()
{
}

WOInterceptorMissile::~WOInterceptorMissile()
{
}

void WOInterceptorMissile::setMotionStrategy( std::unique_ptr< MotionStrategyIFace > strategy, double range_m,
                                              double totalTime_sec, double headingDeg, const Vector& launchPos )
{
	this->motionAlgorithm = std::move(strategy);
	strategy = nullptr;

	this->range_m = range_m;
	this->totalTime_sec = totalTime_sec;
	this->headingDeg = headingDeg;
	this->launchPosition = launchPos;
}

double WOInterceptorMissile::getMotionStrategyLaunchAngleDeg()
{
	double angle = this->motionAlgorithm->getLaunchAngle(this->range_m, this->totalTime_sec); 
	return angle;
}

std::string WOInterceptorMissile::getTrajectoryInfo()
{
   if( this->motionAlgorithm != nullptr )
      return std::move( this->motionAlgorithm->toString( this->range_m, this->totalTime_sec, this->headingDeg, this->launchPosition ) );
   return "Motion Algorithm is empty, cannot compute trajectory info...\n";
}

void WOInterceptorMissile::onUpdateWO()
{
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
	std::chrono::duration< double, std::micro > deltaT_uSec = std::chrono::system_clock::now() - this->launchTime;
	double deltaTSec = deltaT_uSec.count() / 1'000'000; //convert microseconds to seconds by dividing by a million

	Vector newPosition; //will store the new position returned by the strategy evaluated the the current time step
	bool reachedTarget = false; //will store the boolean return value from the strategy
	
	newPosition = std::get<0>(this->motionAlgorithm->computePosition(deltaTSec, this->range_m, this->totalTime_sec, this->headingDeg, this->launchPosition));
	this->getModel()->setLookDirection(Vector{ this->getXYZ() });

	this->setPosition(newPosition);
	if (this->totalTime_sec < deltaTSec)
		this->state = MISSILE_STATE::DETONATING;
}

void WOInterceptorMissile::launch()
{
	this->state = MISSILE_STATE::FLYING;
	this->launchTime = std::chrono::system_clock::now();
}

void WOInterceptorMissile::reset()
{
	this->setPosition(this->launchPosition); //Set by the config file
	this->state = MISSILE_STATE::INERT;
}

Vector WOInterceptorMissile::getXYZ() {
	std::chrono::duration< double, std::micro > deltaT_uSec = std::chrono::system_clock::now() - this->launchTime;
	double deltaTSec = deltaT_uSec.count() / 1'000'000; //convert microseconds to seconds by dividing by a million

	double V_vert = 9.81 * (this->totalTime_sec / 2.0);
	double V_horz = this->range_m / this->totalTime_sec;
	auto xComp = V_horz * std::sin((headingDeg + 90) * Aftr::DEGtoRAD);
	auto yComp = V_horz * std::cos((this->headingDeg + 90) * Aftr::DEGtoRAD);
	auto zComp = (V_vert - 9.81 * deltaTSec);
	Vector v = Vector{ static_cast<float>(xComp), static_cast<float>(yComp), static_cast<float>(zComp) };
	return v;

}

} //namespace Aftr
