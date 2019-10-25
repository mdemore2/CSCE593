#pragma once

#include "WO.h"
#include <chrono>
#include "MotionStrategy.h"

namespace Aftr
{

class AftrGeometryPlane;

/** 
   \class WORadarSensor
   \author Scott Nykl
   \brief A Sensor that returns ranging information of inbound projectiles. This sensor is typically
          mounted at the focal point of a radar dish. Essentially, it is "part of" a radar. In terms 
          of the AftrBurner engine, this sensor should be a child WO of the parent WORadarDish.
   \{
*/

/// Function pointer to the user's internal data structures that deal with missile tracking and processing.
/// This function is called by the WORadarSensor when positioning information specific to an inbound object
/// has been fully processed by the radar. The unsigned int corresponds to the inbound object's unique signature.
/// The vector< Vector > contains the sensed positioning information after it has already been filtered via the filter callback.
using RadarCallback_onScanDataAvailable = std::function< void( unsigned int, 
                                                               const std::chrono::system_clock::time_point&,
                                                               std::vector< Vector >&& ) >;

/// Function pointer called by the sensor when raw data has been received and needs to be filtered. Filtering is performed
/// before this data is published to any subscribers. In other words, this filter method is called before the scan data available
/// callback is invoked.
using RadarCallback_onFilterRawScanData = std::function< std::tuple< unsigned int, std::vector< Vector > >( const WO*, const std::chrono::system_clock::time_point& ) >;

class WORadarSensor : public WO
{
public:
   /**
      WORadarSensor Factory constructor.
   */
   static WORadarSensor* New();

   /**
      All inherited member data deleted by the implicit call to WO's destructor.
      Any member data allocated in this subclass must be properly deallocated here.
   */
   virtual ~WORadarSensor();

   /// DO NOT MODIFY THIS METHOD - manufacturer supplied
   /// Called once per main loop. If this class is a child of a parent that overrides this class. The parent is responsible
   /// for calling WO::onUpdateWO() to ensure the children invoke this method. See WO::onUpdateWO() for details.
   virtual void onUpdateWO();
   
   /// DO NOT MODIFY THIS METHOD - manufacturer supplied
   /// Any WO added via this method will be tracked by the radar. When the WO falls within the wedge, the radar will track and
   /// report its position. If a WO is not placed in this list, the radar will not track this object.
   void addPossibleTargetToRadarScanList( WO* objToSearchFor );

   /// DO NOT MODIFY THIS METHOD - manufacturer supplied
   /// This method consumes a function with the prototype defined above (see definition of RadarCallback_onFilterRawScanData).
   /// This method is invoked within processScanData(). It is called for each WO* residing within the wedge (the WO* must
   /// also reside within the Radar Scan List (this->possibleTargets). That is, if there are 3 WOs that reside in
   /// this->possibleTargets, and 2 of them are located in the wedge, the callback will be invoked twice - once for each
   /// WO residing in the wedge.
   /// The filter method is designed to take raw sensor data and filter it. Since our virtual world has perfect truth data
   /// this filtering method actually takes a perfect position and will filter it to add realistic noise / uncertainty about
   /// that data point. For Phase 2, however, the filtering algorithm simply converts the Cartesian x,y,z coordinate of the
   /// tracked object to a Spherical coordinate system such that:
   /// .x = Range (in meters)
   /// .y = Azimuth (Heading in degrees 0=North, 90=East)
   /// .z = Elevation Angle (Degrees)
   void setFilteringOfRawScanDataAlgorithm( const RadarCallback_onFilterRawScanData& filterFunc );

   /// DO NOT MODIFY THIS METHOD - manufacturer supplied
   /// This method receives an std::function that is invoked after a tracked object's tracking data (including filtering) has 
   /// completed and is available. The data passed via the std::function contains a unique id of the tracked object as well
   /// as the final positioning estimates from the most recent pulse. Zero or more position estimates will be available.
   /// The std::vector< Vector > passed into the std::function<>'s parameter list contain 3 components:
   /// .x = Range (in meters)
   /// .y = Azimuth (Heading in degrees 0=North, 90=East)
   /// .z = Elevation Angle (Degrees)
   void subscribeToRadarTrackingInfo( const RadarCallback_onScanDataAvailable& processedDataFunc );
   
   /// After scanForTargetsWithinCurrentWedge() has returned, this method is designed to consume the former's return
   /// values. That is, it consumes a list of targets, the time at which the observation was made, and it invokes the
   /// filtering and publishing callbacks. Internally, this method invokes the callback for the filterFunction, retrieves
   /// the result, and then invokes the callback onScanDataAvailable. It also consumes a timestamp associated with the
   /// observation time of the targets vector.
   void processScanData( const std::vector< WO* >& targets, const std::chrono::system_clock::time_point& t ) const;
   
   /// Returns the hyper plane representing the current "left" side of the radar's wedge. That is, Face A, as seen below.
   ///             \        /
   ///              \      /
   /// Normal vec <--\    /--> Normal vec  (Note: real normals are perpendicular to each face, not parallel with each other).
   ///        Face A  \  / Face B
   ///                 \/
   ///  
   AftrGeometryPlane getLeftHyperplane() const;

   /// Returns the hyper plane representing the current "right" side of the radar's wedge. That is, Face B, as seen above.
   AftrGeometryPlane getRightHyperplane() const;

   /// Sets the sensor's scan wedge horizontal field of view, in degrees. This adjusts the radar's FOV immediately.
   void setHorzFOVDeg( float horzFieldOfViewDeg );
   /// Sets the maximum range the wedge extents from the apex. This adjusts the radars range immediately.
   void setScanRangeMeters( float scanRangeMeters );

   /// Returns a string containing the current geometric state of the scanning wedge. Including
   /// what targets are illuminated.
   std::string getSensorInfo() const;

protected:
   WORadarSensor();
   virtual void onCreate();

private:

   /// Returns a list of all targets that reside within the current wedge. Only WOs that have already been added via
   /// addPossibleTargetToRadarScanList(...) will be considered. Internally, this uses the current Left & Right hyperplanes
   /// as well as the current scan range of the radar to include candidates.
   std::vector< WO* > scanForTargetsWithinCurrentWedge() const;

   /// DO NOT MODIFY THIS METHOD - manufacturer supplied
   /// This method constructs the physical geometry of the wedge based on the field of view and 
   /// other parameters specified in the conf file.
   std::tuple< std::vector< Vector >, std::vector< unsigned int >, float, float > computeSensedGeometry( float horzFieldOfViewDeg, float rangeMeters ) const;
   
   /// DO NOT MODIFY THIS METHOD - manufacturer supplied
   /// This method is simply a convenience method that accepts a tuple and forwards to the non-tupled parameter version of this
   /// method. It is convenient because this->computeSensedGeometry returns this exact tuple.
   void updateModelGeometry( std::tuple< std::vector< Vector >, std::vector< unsigned int >, float, float >& geomData );
   
   /// DO NOT MODIFY THIS METHOD - manufacturer supplied
   /// This method creates the graphical representation that is actually rendered within the virtual world.
   void updateModelGeometry( const std::vector< Vector >& verts, const std::vector< unsigned int >& indices, float horzFieldOfView, float rangeMeters );

   float horzFieldOfView = 20.0f; /// Default horizontal field of view for radar sensing wedge
   float scanRangeMeters = 400; /// Default range radar can sense. Defined to be from wedge's apex to this distance away, within hFOV

   /// Stores a list of all objects that undergo collision detection with the radar's "field of view" wedge. Only
   /// Objects in this list can ever be seen by the radar.
   std::vector< WO* > possibleTargets;

   RadarCallback_onFilterRawScanData callback_onFilterRawScanData; /// Function pointer. Called when raw data is available to filter.
   RadarCallback_onScanDataAvailable callback_onScanDataAvailable; /// Function pointer. Called when processed data is available to user.
};

/** \} */

} //namespace Aftr

