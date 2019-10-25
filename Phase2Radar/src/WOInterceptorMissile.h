#pragma once

#include "WO.h"
#include <chrono>
#include "MotionStrategy.h"

namespace Aftr
{

/** 
   \class WOInterceptorMissile
   \author Scott Nykl
   \brief A missile that can target & detonate at a specific coordinate

   \{
*/
class WOInterceptorMissile : public WO
{
public:
   /**
      WOInterceptorMissile constructor.
      \param lxlylz The length in the X,Y, and Z directions of the WOBoundingShell.
      \param mass The mass, in kilograms, of the WOInterceptorMissile.
      \see WOBoundingShell
   */
   static WOInterceptorMissile* New( );

   /**
      Destructor of WOInterceptorMissile. Only directly deletes the animations; everything else is deleted
      by the implicit call to WO's destructor.
   */
   virtual ~WOInterceptorMissile();

   virtual void onUpdateWO();

   void updateMissileTrajectory();
   void launch();
   void reset();

   void setMotionStrategy( std::unique_ptr< MotionStrategyIFace > strategy, double range_m, 
                           double totalTime_sec, double headingDeg, const Vector& launchPos );
   double getMotionStrategyLaunchAngleDeg();
   std::string getTrajectoryInfo();

protected:
   WOInterceptorMissile();
   virtual void onCreate( const std::string& modelFileName, const Vector& scale, const MESH_SHADING_TYPE& mstMeshShadingType );

   //When AIMING, the launch() method will transition to the interceptor to FLYING.
   //After FLYING to the target location, the interception will begin DETONATING. This will
   //last exactly one update, after which it will become INERT.
   enum class MISSILE_STATE { AIMING, FLYING, DETONATING, INERT };
   MISSILE_STATE state = MISSILE_STATE::AIMING;

   std::chrono::system_clock::time_point launchTime; //set when this->launch() is invoked

   /// Pointer to strategy pattern encapsulating newtonian motion
   std::unique_ptr< MotionStrategyIFace > motionAlgorithm = nullptr;

   //These member variables are stored in the interceptor so the Strategy algorithm is
   //stateless. In general, strategy objects, function objects, functors, etc should be stateless
   //so they can be copy constructed without side effects.
   double range_m = 0; //total horizontal distance covered
   double totalTime_sec = 0; //total time until impact
   double headingDeg = 0;
   Vector launchPosition; //set from conf file and used in reset() method
};

/** \} */

} //namespace Aftr

