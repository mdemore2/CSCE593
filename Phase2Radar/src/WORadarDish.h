#pragma once

#include "WO.h"
#include <chrono>
#include "MotionStrategy.h"

namespace Aftr
{

class WORadarSensor;

/** 
   \class WORadarDish
   \author Scott Nykl
   \brief A Dish model that houses the sensor at the parabola's focal point. For phase 2, you can manually create
   the sensor child in the loadMap() method of the GLView and add as a child there.
   This class itself does not do anything special other than serve as a parent to the sensor.
   \{
*/
class WORadarDish : public WO
{
public:
   /**
      WORadarDish Factory constructor.
   */
   static WORadarDish* New();

   /**
      All inherited member data deleted by the implicit call to WO's destructor.
      Any member data created in this subclass must be properly destroyed here.
   */
   virtual ~WORadarDish();

   virtual void onUpdateWO();

   /// The in passed sensor must be child of this radar dish (the sensor should not appear in the worldLst directly, but only
   /// as a child of this object. The loadMap() in the GLView can call this->getChildren().push_back( sensor ).
   void attachSensor( WORadarSensor* sensor );
   WORadarSensor* getSensor() { return this->sensor; }


protected:
   WORadarDish();

   /// This method is responsible for loading the "/models/DefenseDaemon/Radar/dish.3ds" object.
   virtual void onCreate( const std::string& modelFileName, const Vector& scale, const MESH_SHADING_TYPE& mstMeshShadingType );

private:

   /// Convenience pointer to the sensor child of this dish.
   WORadarSensor* sensor = nullptr;
};

/** \} */

} //namespace Aftr

