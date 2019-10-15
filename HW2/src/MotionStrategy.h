#pragma once

#include "Vector.h"

namespace Aftr
{

struct MotionStrategyIFace
{
   MotionStrategyIFace() = default;
   virtual ~MotionStrategyIFace() = default;
   virtual std::tuple<Vector,bool> computePosition( double dt_sec, double range_m, double totalTime_sec, double headingDeg ) = 0;
   virtual double getLaunchAngle( double range_m, double totalTime_sec ) = 0;
   virtual std::string toString( double range_m, double totalTime_sec, double headingDeg ) const = 0;
};

struct MotionParabolic : public MotionStrategyIFace
{
   MotionParabolic() = default;
   virtual ~MotionParabolic() = default;
   virtual std::tuple<Vector,bool> computePosition( double dt_sec, double range_m, double totalTime_sec, double headingDeg ) override;
   virtual double getLaunchAngle( double range_m, double totalTime_sec ) override;
   virtual std::string toString( double range_m, double totalTime_sec, double headingDeg ) const override;
};

} //namespace Aftr

