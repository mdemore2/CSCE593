#include "WORadarSensor.h"
#include "Model.h"
#include "WOGJoint.h"
#include <cmath>
#include <iostream>
#include "MGLIndexedGeometry.h"
#include "IndexedGeometryTriangles.h"
#include "AftrGeometryPlane.h"

namespace Aftr
{

WORadarSensor* WORadarSensor::New()
{
   WORadarSensor* wo = new WORadarSensor();
   wo->onCreate();     
   return wo;
}

void WORadarSensor::onCreate()
{
   WO::onCreate();

   this->model = MGLIndexedGeometry::New( this ); //we must set the geometry of this object. This
            //is computed and set via computeSensedGeometry and updateModelGeometry, respectively.
   
   //Create two hyper planes defining a wedge shape. This shape will define the sensor's field of view.
   //use the computeSensedGeometry and updateModelGeometry method to instantiate a wedge
   auto tup = this->computeSensedGeometry( this->horzFieldOfView, this->scanRangeMeters );
   this->updateModelGeometry( tup );
}

WORadarSensor::WORadarSensor() : IFace( this ), WO()
{
}

WORadarSensor::~WORadarSensor()
{
}

void WORadarSensor::onUpdateWO()
{
   //Do NOT modify this code - manufacturer supplied
   WO::onUpdateWO();
   std::vector<WO*> targets = this->scanForTargetsWithinCurrentWedge();
   this->processScanData( targets, std::chrono::system_clock::now() );
}

void WORadarSensor::addPossibleTargetToRadarScanList( WO* objToSearchFor )
{
	this->possibleTargets.push_back(objToSearchFor);
}

void WORadarSensor::setFilteringOfRawScanDataAlgorithm( const RadarCallback_onFilterRawScanData& filterFunc )
{
	this->callback_onFilterRawScanData = filterFunc;
	//convert between cartesion and polar
	//radius = sqrt(xsq+ysq+zsq)
	//azimuth = atan2(y,x)
	//inclination = acos(z/r)
}

void WORadarSensor::subscribeToRadarTrackingInfo( const RadarCallback_onScanDataAvailable& processedDataFunc )
{
	this->callback_onScanDataAvailable = processedDataFunc;
	//phase2: just print out data
	//future phases, this interfaces with missile launcher stuff (what do you want to do now that position known)
}

std::vector< WO* > WORadarSensor::scanForTargetsWithinCurrentWedge() const
{
   std::vector< WO* > insideWedge;
   //perform collision detection between all possible targets and the radar's wedge
   auto leftNormal = this->getLeftHyperplane().getNormal();
   auto rightNormal = this->getRightHyperplane().getNormal();

   for (auto missile : this->possibleTargets) {
	   if (sqrt(pow(missile->getPosition().x, 2) + pow(missile->getPosition().y, 2)) <= this->scanRangeMeters) {
		   float planeLeftVal = leftNormal.x*missile->getPosition().x + leftNormal.y * missile->getPosition().y + this->getLeftHyperplane().getDisplacement();
		   float planeRightVal = rightNormal.x * missile->getPosition().x + rightNormal.y *  missile->getPosition().y + this->getRightHyperplane().getDisplacement();
		   if (planeLeftVal < 0 && planeRightVal < 0) {
			   insideWedge.push_back(missile);
			}
	   }
   }
   return std::move( insideWedge );
}

void WORadarSensor::processScanData( const std::vector< WO* >& targets, const std::chrono::system_clock::time_point& t ) const
{
	for (auto target : targets)
	{

		std::vector< Vector > polar_coord;
		unsigned int id;

		std::tie(id, polar_coord) = this->callback_onFilterRawScanData(target, t);
		this->callback_onScanDataAvailable(id, t, std::move(polar_coord));
		
	}
}

void WORadarSensor::setHorzFOVDeg( float horzFieldOfViewDeg )
{
	this->horzFieldOfView = horzFieldOfViewDeg;
}

void WORadarSensor::setScanRangeMeters( float scanRangeMeters )
{	
	this->updateModelGeometry(this->computeSensedGeometry(this->horzFieldOfView, scanRangeMeters));
	this->scanRangeMeters = scanRangeMeters;
}

std::string WORadarSensor::getSensorInfo() const
{
   std::stringstream ss;
   ss << "Apex Pos: " << this->getPosition() << "\n"
      << "Horz FOV: " << this->horzFieldOfView << " deg\n"
      << "Scan Rng: " << this->scanRangeMeters << " m\n"
      << "Curr Hdg: " << this->getLookDirection().getTheta() * Aftr::RADtoDEG << " deg\n"
      << "A call to scanForTargetsWithinCurrentWedge() returns the following:\n";
   auto v = this->scanForTargetsWithinCurrentWedge();
   for( auto& w : v )
      ss << "   woID: " << w->getID() << " at Pos " << w->getPosition() << "\n";

   return ss.str();
}

AftrGeometryPlane WORadarSensor::getLeftHyperplane() const
{
   AftrGeometryPlane p;
   Vector vecPlane = this->getLookDirection();// rotate({ 0,0,1 }, Aftr::DEGtoRAD * ((this->horzFieldOfView / 2) + 90));
   vecPlane = vecPlane.rotate({ 0,0,1 }, -Aftr::DEGtoRAD * ((this->horzFieldOfView / 2) + 90));
   vecPlane.normalize();
   p.setNormal(vecPlane);
   float disp = (vecPlane.x * 10) + (vecPlane.y * 0) + (vecPlane.z * 0);
   disp = disp / sqrt(pow(vecPlane.x, 2) + pow(vecPlane.y, 2) + pow(vecPlane.z, 2));
   //disp *= -1;
   p.setDisplacement(disp);
   return p;
}

AftrGeometryPlane WORadarSensor::getRightHyperplane() const
{
   AftrGeometryPlane p;
   Vector vecPlane = this->getLookDirection();
   vecPlane = vecPlane.rotate({ 0,0,1 }, Aftr::DEGtoRAD * ((this->horzFieldOfView / 2) + 90));
   vecPlane.normalize();
   p.setNormal(vecPlane);
   float disp = (vecPlane.x * 10) + (vecPlane.y * 0) + (vecPlane.z * 0);
   disp = disp / sqrt(pow(vecPlane.x, 2) + pow(vecPlane.y, 2) + pow(vecPlane.z, 2));
   //disp *= -1;
   p.setDisplacement(disp);
   return p;
}

std::tuple< std::vector< Vector >, std::vector< unsigned int >, float, float > WORadarSensor::computeSensedGeometry( float horzFieldOfViewDeg, float rangeMeters ) const
{
   //Do NOT modify this code - manufacturer supplied. However, I recommend you carefully read through this code and
   //understand how it works... This may be your first exposure to modern OpenGL indexed geometry. If you've ever played
   //a 3D game, an approach similar to this code was used to create the geometry rendered by the graphics card.

   //             \        /
   //              \      /
   // Normal vec <--\    /--> Normal vec  (Note: real normals are perpendicular to each face, not parallel with each other).
   //        Face A  \  / Face B
   //                 \/
   //          

   constexpr float H = 15.0f;

   //a is the face A side
   Vector a = Vector{ 1,0,0 }.rotate( Vector{ 0,0,1 }, horzFieldOfViewDeg / 2.0f * Aftr::DEGtoRAD );
   a *= rangeMeters;

   //b is the face B side
   Vector b = Vector{ 1,0,0 }.rotate( Vector{ 0,0,1 }, -horzFieldOfViewDeg / 2.0f * Aftr::DEGtoRAD );
   b *= rangeMeters;

   Vector faceA_TopNear = { 0, 0, H };
   Vector  faceA_TopFar = { a.x, a.y, H };
   Vector faceA_BotNear = { 0,0,0 };
   Vector  faceA_BotFar = { a.x, a.y, 0 };

   Vector faceB_TopNear = { 0, 0, H };
   Vector  faceB_TopFar = { b.x, b.y, H };
   Vector faceB_BotNear = { 0,0,0 };
   Vector  faceB_BotFar = { b.x, b.y, 0 };

   //vertex order is:
   //faceA top/near, faceA top far, faceA bottom near, faceA bottom far,
   //faceB top/near, faceB top far, faceB bottom near, faceB bottom far
   std::vector< Vector > verts = { faceA_TopNear, faceA_TopFar, faceA_BotNear, faceA_BotFar,
                                   faceB_TopNear, faceB_BotNear, faceB_TopFar, faceB_BotFar };

   //Index determines the order that the triangles in Vertex are rendered - we use counter clockwise notation.
   std::vector< unsigned int > indices = {0,1,2,   //TriA1 Normal points left-ish
                                          2,1,3,   //TriA2 Normal points left-ish
                                          4,5,6,   //TriA2 Normal points right-ish
                                          5,7,6 }; //TriA2 Normal points right-ish

   return std::make_tuple( verts, indices, horzFieldOfViewDeg, rangeMeters );
}


void WORadarSensor::updateModelGeometry( std::tuple< std::vector< Vector >, std::vector< unsigned int >, float, float >& geomData )
{
   //Do NOT modify this code - manufacturer supplied
   this->updateModelGeometry( std::get<0>( geomData ), std::get<1>( geomData ), std::get<2>( geomData ), std::get<3>( geomData ) );
}

void WORadarSensor::updateModelGeometry( const std::vector< Vector >& verts, const std::vector< unsigned int >& indices, float horzFieldOfView, float rangeMeters )
{
   //Do NOT modify this code - manufacturer supplied
   this->horzFieldOfView = horzFieldOfView;
   this->scanRangeMeters = rangeMeters;

   IndexedGeometryTriangles* geom = IndexedGeometryTriangles::New( verts, indices );
   this->getModelT<MGLIndexedGeometry>()->setIndexedGeometry( geom );
}

} //namespace Aftr
