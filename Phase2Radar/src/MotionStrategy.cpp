#pragma once

#include "MotionStrategy.h"

namespace Aftr
{

   std::tuple<Vector,bool> MotionParabolic::computePosition( double dt_sec, double range_m, double totalTime_sec, double headingDeg, const Vector& initialPos )
   {
      bool targetReached = false;
      if( dt_sec >= totalTime_sec )
      {
         dt_sec = totalTime_sec;
         targetReached = true;
      }

      //Ensure we do not do any dividing by zero. If invalid params are passed in, simply
      //print a warning and return 0,0,0.
      if( totalTime_sec <= 0 )
      {
         std::cout << "WARNING: " << AFTR_FILE_LINE_STR << ": totalTime_sec is <= 0. Invalid value. Returning 0,0,0.\n";
         return std::make_tuple( Vector{ 0,0,0 }, false );
      }

      //compute initial horizontal velocity
      double V_horz = range_m / totalTime_sec;

      //compute initial vertical velocity
      double V_vert = 9.81 * (totalTime_sec / 2.0);

      //find launch angle
      double launchAngleRad = std::atan( V_vert / V_horz );

      //compute horz position at deltaT
      double horzPos = V_horz * dt_sec;

      double xComp = horzPos * std::sin( (headingDeg+90) * Aftr::DEGtoRAD );
      double yComp = horzPos * std::cos( (headingDeg+90) * Aftr::DEGtoRAD );

      //compute vertical position at deltaT
      double vertPos = 0.5 * -9.81 * dt_sec * dt_sec + V_vert * dt_sec;

      Vector pos = { static_cast<float>( xComp ), static_cast<float>( yComp ), static_cast<float>( vertPos ) };
      pos += initialPos;

      return std::make_tuple( pos, targetReached );
   }

   double MotionParabolic::getLaunchAngle( double range_m, double totalTime_sec )
   {
      //compute initial horizontal velocity
      double V_horz = range_m / totalTime_sec;

      //compute initial vertical velocity
      double V_vert = 9.81 * ( totalTime_sec / 2.0 );

      //find launch angle
      double launchAngleRad = std::atan( V_vert / V_horz ) * Aftr::RADtoDEGd;
      return launchAngleRad;
   }

   std::string MotionParabolic::toString( double range_m, double totalTime_sec, double headingDeg, const Vector& initialPos ) const
   {
      //use a std::stringstream object to print out the following information
      std::stringstream ss;
      
      //compute initial horizontal velocity
      double V_horz = range_m / totalTime_sec;
      //compute initial vertical velocity
      double V_vert = 9.81 * ( totalTime_sec / 2.0 );
      double maxAltM= 0.5 * -9.81 * (totalTime_sec/2.0) * (totalTime_sec/2.0) + V_vert * (totalTime_sec/2.0);

      double horzPos = V_horz * totalTime_sec;
      double xComp = horzPos * std::sin( ( headingDeg + 90 ) * Aftr::DEGtoRAD );
      double yComp = horzPos * std::cos( ( headingDeg + 90 ) * Aftr::DEGtoRAD );

      Vector pos = Vector{ static_cast<float>( xComp ), static_cast<float>( yComp ), 0 };
      pos += initialPos;

      ss << "V_horz (m/s) is  " << V_horz << "\n";
      ss << "V_vert (m/s) is  " << V_vert << "\n";
      ss << "Max Alt (m)  is " << maxAltM << "\n";
      ss << "Landing Position is " << pos.toString() << "\n";

      return ss.str();
   }

} //namespace Aftr

