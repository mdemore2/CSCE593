#include "WORadarDish.h"
#include "Model.h"
#include "WOGJoint.h"
#include "WORadarSensor.h"
#include <iostream>

namespace Aftr
{

WORadarDish* WORadarDish::New()
{
   WORadarDish* wo = new WORadarDish();
   //model for a Dish is located at /models/DefenseDaemon/Radar/dish.3ds
   wo->onCreate( ManagerEnvironmentConfiguration::getSMM() + "/models/DefenseDaemon/Radar/dish.3ds", Vector{ 1,1,1 }, MESH_SHADING_TYPE::mstAUTO );     
   return wo;
}

void WORadarDish::onCreate( const std::string& modelFileName, const Vector& scale, const MESH_SHADING_TYPE& mstMeshShadingType )
{
   WO::onCreate( modelFileName, scale, mstMeshShadingType );
}

WORadarDish::WORadarDish() : IFace( this ), WO()
{
}

WORadarDish::~WORadarDish()
{
}

void WORadarDish::attachSensor( WORadarSensor* sensor )
{
}

void WORadarDish::onUpdateWO()
{
   WO::onUpdateWO();
}

} //namespace Aftr
