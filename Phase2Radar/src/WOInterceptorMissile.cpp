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
}

double WOInterceptorMissile::getMotionStrategyLaunchAngleDeg()
{
   return 0;
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
}

void WOInterceptorMissile::launch()
{
}

void WOInterceptorMissile::reset()
{
}

} //namespace Aftr
