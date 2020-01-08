#include "WORadarSensor.h"
#include "Model.h"
#include "WOGJoint.h"
#include "WO.h"
#include <cmath>
#include <iostream>
#include "MGLIndexedGeometry.h"
#include "IndexedGeometryTriangles.h"
#include "AftrGeometryPlane.h"
#include "PolyFit.h"
#include <typeinfo>
#include <tuple>
#include <iterator>

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
   if ((this->targetedMissiles.size() > 0)) {
	   auto unassignedMissiles = this->getUnassignedMissileId();
	   for (int i = 0; i < unassignedMissiles.size(); i++) {
		   this->callback_onNewBlueMissile(unassignedMissiles[i]);//pass in id for target missile it has to hit? and add assigned missile in the lambda
	   }
   }
}

void WORadarSensor::setRotationSpeed(float rotationsPerSecond)
{
	this->rotPerSecond = rotationsPerSecond;
}

float WORadarSensor::getRotationsPSec()
{
	return this->rotPerSecond;
}

void WORadarSensor::addPossibleTargetToRadarScanList(WO* objToSearchFor)
{
	//this->callback_onObjectIlluminatedByRadar = onRadarIlluminationOccurredCallback;
	this->possibleTargets.push_back(objToSearchFor);
}

void WORadarSensor::setIlluminationFunc(const RadarCallback_onObjectIlluminatedByRadar& onRadarIlluminationOccurredCallback)
{
	this->callback_onObjectIlluminatedByRadar = onRadarIlluminationOccurredCallback;
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

void WORadarSensor::subscribeToUpdateBlueMissiles(const RadarCallback_onNewBlueMissile& addBlueMissileCallback) {
	this->callback_onNewBlueMissile = addBlueMissileCallback;
}

std::vector< WO* > WORadarSensor::scanForTargetsWithinCurrentWedge() const
{
   std::vector< WO* > insideWedge;
   //perform collision detection between all possible targets and the radar's wedge
   Vector pointOnPlane = this->getPosition();//while tower is at {-10,0,0}  this point will be on both planes 
   auto leftNormal = this->getLeftHyperplane().getNormal();
   auto rightNormal = this->getRightHyperplane().getNormal();

   for (auto totalMissile : this->possibleTargets) {
	   auto missile = (totalMissile);
	   auto position = missile->getPosition();
	   auto thisPos = this->getPosition();

	   if (sqrt(pow(missile->getPosition().x - this->getPosition().x, 2) + pow(missile->getPosition().y - this->getPosition().y, 2)) <= this->scanRangeMeters) {
		   float planeLeftVal = leftNormal.x*missile->getPosition().x + leftNormal.y * missile->getPosition().y + this->getLeftHyperplane().getDisplacement();
		   float planeRightVal = rightNormal.x * missile->getPosition().x + rightNormal.y *  missile->getPosition().y + this->getRightHyperplane().getDisplacement();
		   if (planeLeftVal < 0 && planeRightVal < 0) {
			   insideWedge.push_back(missile);
			   this->callback_onObjectIlluminatedByRadar(std::move(missile), true);
		   }
		   else
			   this->callback_onObjectIlluminatedByRadar(std::move(missile), false);
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
		//this->polarPoints->push_back(std::tie(id, polar_coord));
		std::tie(id, polar_coord) = this->callback_onFilterRawScanData(target, t);
		this->callback_onScanDataAvailable(id, t, std::move(polar_coord));
	}
}

void WORadarSensor::setHorzFOVDeg( float horzFieldOfViewDeg )
{
	this->horzFieldOfView = horzFieldOfViewDeg;
}

float WORadarSensor::getHorzFOVDeg()
{
	return this->horzFieldOfView;
}

void WORadarSensor::setScanRangeMeters( float scanRangeMeters )
{
	this->scanRangeMeters = scanRangeMeters;
	updateModelGeometry(computeSensedGeometry(horzFieldOfView, scanRangeMeters));
}

float WORadarSensor::getScanRangeMeters()
{
	return this->scanRangeMeters;
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
   //wikipedia has a great page on these... so does wolfram... welcome to grad school :) // pass vector of planes normal vector
   Vector vecPlane = this->getLookDirection();// rotate({ 0,0,1 }, Aftr::DEGtoRAD * ((this->horzFieldOfView / 2) + 90));
   vecPlane = vecPlane.rotate({ 0,0,1 }, -Aftr::DEGtoRAD * ((this->horzFieldOfView / 2) + 90));
   vecPlane.normalize();
   p.setNormal(vecPlane);
   float disp = -vecPlane.x * this->getPosition().x - vecPlane.y * this->getPosition().y;
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
   float disp = -vecPlane.x * this->getPosition().x - vecPlane.y * this->getPosition().y;
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

void WORadarSensor::resetMissilePoints() {
	this->missilePoints.clear();
	this-> targetedMissiles.clear();
	this->assignedMissiles.clear();
	this->areMissilesAssigned.clear();
	this->possibleTargets.clear();
	this->missileKills = 0;
	this->newMissile = true;
}

std::optional<std::vector<std::pair<float, float>>> WORadarSensor::makePredictedPoints(unsigned int id) {
		std::vector<float > x_pts;
		std::vector<float > y_pts;

		auto range = this->missilePoints.equal_range(id);

		for (auto i = range.first; i != range.second; ++i) {
			x_pts.push_back(sqrt(pow((std::get<0>(i->second).y - this->getPosition().y), 2) + pow((std::get<0>(i->second).x - -this->getPosition().y), 2)));
			y_pts.push_back(std::get<0>(i->second).z);
		}
		if (std::size(x_pts) > 1)
		{
			auto coefficients = PolyFit::PolyFit(x_pts, y_pts, 2); //get the coefficients
			auto root1 = (-1 * coefficients[1] + sqrt(pow(coefficients[1], 2) - (4 * coefficients[0] * coefficients[2]))) / (2 * coefficients[2]); //get the roots
			auto root2 = (-1 * coefficients[1] - sqrt(pow(coefficients[1], 2) - (4 * coefficients[0] * coefficients[2]))) / (2 * coefficients[2]);

			std::vector<float>all_x_pts;
			for (int i = root1; i <= root2; i++) { //from root to root, loop through all possible x values 
				all_x_pts.push_back(i);
			}

			auto all_y_pts = PolyFit::PolyVal(coefficients, all_x_pts);//generate y values using polyval

			std::vector<std::pair<float, float> > xy_pts; //make pairs of predicted x and y vals
			for (int i = 0; i < all_x_pts.size(); i++) {
				xy_pts.push_back(std::pair< float, float>(all_x_pts[i], all_y_pts[i]));
			}

			return xy_pts;
		}
		else
		{
			return std::nullopt;
		}
	}

/*
std::vector<std::pair<float, float>> WORadarSensor::makeObservedPoints() {
		std::vector<std::pair<float, float> > xy_pts;
		for (const auto& myPair : this->missilePoints) {
			float dist = sqrt(pow((myPair.(std::get<0>(second).y) - this->getPosition().y), 2) + pow((myPair.second.x - -this->getPosition().y), 2));
			xy_pts.push_back(std::pair< float, float>(dist, myPair.second.z));
		}
		return xy_pts;
	}
*/

std::optional<std::pair<std::tuple<double, double>, std::tuple<Vector, Vector>>> WORadarSensor::getTimesAndPoints(unsigned int id) {
	std::tuple<Vector, Vector> points = {};
	std::tuple<double, double> times = {};
	if (std::find(this->targetedMissiles.begin(), this->targetedMissiles.end(), id) != this->targetedMissiles.end()){
		// Get an iterator for the missile points
		auto range = this->missilePoints.equal_range(id);
		// Find how many missle points we have for this missile
		auto count = std::distance(range.first, range.second);
		// If we have more than 2 points
		if ( count >= 2)
		{
			auto firstElement = range.first;
			std::advance(range.first, 1);
			auto secondElement = range.first;
			// First element contains (id, missile points) missile points being (point, time)
			Vector first_point = std::get<0>(firstElement->second);
			Vector second_point = std::get<0>(secondElement->second);

			points = std::make_tuple(first_point, second_point);
			double first_time = std::get<1>(firstElement->second);
			double second_time = std::get<1>(secondElement->second);
			times = std::make_tuple(first_time, second_time);
			
			// Return a tuple where time=(t1, t2) and points=(p1, p2) and missile is at p1 at t1
			return std::make_pair(times, points);
		}		
	}
	
	return std::nullopt;
}

//std::pair<std::vector<Vector>, std::vector<double>> WORadarSensor::getMissilePoints(unsigned int id)
//{
//	// Points collected of missile
//	std::vector<Vector> missile_points;
//	// Times of points collected
//	std::vector<double> missile_times;
//
//	auto range = this->missilePoints.equal_range(id);
//	for (auto i = range.first; i != range.second; ++i) 
//	{
//		missile_points.push_back(std::get<0>(i->second));
//	}
//}


void WORadarSensor::addTargetMissile(unsigned int id) { //
	if (std::find(this->targetedMissiles.begin(), this->targetedMissiles.end(), id) == this->targetedMissiles.end() || this->targetedMissiles.size() ==0) {
		if (std::find(this->assignedMissiles.begin(), this->assignedMissiles.end(), id) == this->assignedMissiles.end() || this->assignedMissiles.size() == 0) {
			this->targetedMissiles.push_back(id);
			this->areMissilesAssigned.push_back(std::make_tuple(id, false));
		}
	}
}

void WORadarSensor::addAssignedMissile(unsigned int id) { //this will be called whenever a blue missile is created
	this->assignedMissiles.push_back(id);
}

void WORadarSensor::removeAssignedMissile(unsigned int id)
{
	for (int i=0; i < std::size(assignedMissiles); i++)
	{
		if (assignedMissiles.at(i) == id)
			assignedMissiles.erase(assignedMissiles.begin()+i);
	}
}

bool WORadarSensor::isNewMissile() const {
	return this->newMissile;
}

void WORadarSensor::addMissilePoints(std::pair<unsigned int, std::tuple<Vector, double> > newPoints) {
	this->missilePoints.insert(newPoints);
}

void WORadarSensor::addRedMissileAssigned(unsigned int id) {
	for (int i = 0; i < this->areMissilesAssigned.size(); i++) {
		if (std::get<0>(this->areMissilesAssigned[i]) == id)
			std::get<1>(this->areMissilesAssigned[i]) = true;
	}
}

std::vector<unsigned int> WORadarSensor::getUnassignedMissileId() {
	std::vector<unsigned int> returnVector;
	for (int i = 0; i < this->areMissilesAssigned.size(); i++) {
		if (!std::get<1>(this->areMissilesAssigned[i]))
			returnVector.push_back(std::get<0>(this->areMissilesAssigned[i]));
	}
	return returnVector;
}

bool WORadarSensor::checkForCollision(unsigned int blueID, unsigned int redID) 
{
	auto blueIt = this->missilePoints.equal_range(blueID);
	auto numBlue = std::distance(blueIt.first, blueIt.second);
	for (auto i = 0; i < numBlue-1; i++)
		std::advance(blueIt.first, 1);

	auto redIt = this->missilePoints.equal_range(redID);
	auto numRed = std::distance(redIt.first, redIt.second);
	for (auto i = 0; i < numRed-1; i++)
		std::advance(redIt.first, 1);

	if ((blueIt.first != this->missilePoints.end()) && (redIt.first != this->missilePoints.end())) {
		Vector bluePoints = std::get<0>(blueIt.first->second);
		Vector redPoints = std::get<0>(redIt.first->second);
		auto dist = sqrt(pow((bluePoints.x - redPoints.x),2) + pow((bluePoints.y - redPoints.y),2) + pow((bluePoints.z - redPoints.z),2));
		//auto dist = sqrt(pow((bluePoints.x - redPoints.x), 2) + pow((bluePoints.y - redPoints.y), 2));
		std::cout << "Dist between missiles: " << dist << "\n";
		if (dist < this->blastRaduis)
			return true;
		else
			return false;
	}
	return false;
}

int WORadarSensor::getKilledMissiles() {
	return this->missileKills;
}
void WORadarSensor::increaseKilledMissiles() {
	this->missileKills++;
}

} //namespace Aftr
