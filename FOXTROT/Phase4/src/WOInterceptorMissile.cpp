#include "WOInterceptorMissile.h"
#include "Model.h"
#include "WOGJoint.h"
#include <cmath>
#include <iostream>
#include "MGLTrackingRibbon.h"
#include "WOTrackingRibbonBase.h"

namespace Aftr
{

	WOInterceptorMissile* WOInterceptorMissile::New()
	{
		WOInterceptorMissile* wo = new WOInterceptorMissile();
		wo->onCreate(ManagerEnvironmentConfiguration::getSMM() + "/models/DefenseDaemon/Launcher/Missile.3ds", Vector{ 1,1,1 }, MESH_SHADING_TYPE::mstAUTO);
		wo->missileRibbon = WOTrackingRibbonBase::New(wo);
		wo->missileRibbon->setParentWorldObject(wo);
		wo->getChildren().push_back(wo->missileRibbon);
		return wo;
	}

	void WOInterceptorMissile::onCreate(const std::string& modelFileName, const Vector& scale, const MESH_SHADING_TYPE& mstMeshShadingType)
	{
		WO::onCreate(modelFileName, scale, mstMeshShadingType);
	}

	WOInterceptorMissile::WOInterceptorMissile() : IFace(this), WO()
	{
	}

	WOInterceptorMissile::~WOInterceptorMissile()
	{
	}

	void WOInterceptorMissile::setMotionStrategy(std::unique_ptr< MotionStrategyIFace > strategy, double range_m,
		double totalTime_sec, double headingDeg, const Vector& launchPos)
	{
		this->motionAlgorithm = std::move(strategy);
		strategy = nullptr;

		this->range_m = range_m;
		this->totalTime_sec = totalTime_sec;
		this->headingDeg = headingDeg;
		this->launchPosition = launchPos;
	}

	void WOInterceptorMissile::onCollision() 
	{
		this->state = MISSILE_STATE::DETONATING;
		this->missileRibbon->setRibbonColor(Vector{ 0, 5, 0 });
	}

	double WOInterceptorMissile::getMotionStrategyLaunchAngleDeg()
	{
		double angle = this->motionAlgorithm->getLaunchAngle(this->range_m, this->totalTime_sec);
		return angle;
	}

	std::string WOInterceptorMissile::getTrajectoryInfo()
	{
		if (this->motionAlgorithm != nullptr)
			return std::move(this->motionAlgorithm->toString(this->range_m, this->totalTime_sec, this->headingDeg, this->launchPosition));
		return "Motion Algorithm is empty, cannot compute trajectory info...\n";
	}

	void WOInterceptorMissile::onUpdateWO()
	{
		if (this->state == MISSILE_STATE::FLYING) {
			if (this->callback_onCollisionDetected != nullptr && callback_onCollisionDetected(this->id, this)) {
				this->onCollision();
			}

			this->updateMissileTrajectory();
			//update the ribbon
			auto currentTrackInfo = new TrackInfo(this->getPosition(), this->getPosition(), 0);
			this->missileRibbon->addTrackPoint(*currentTrackInfo);

			if (this->detectedRadarPulse)
				this->missileRibbon->setRibbonColor(Vector{ 10,0,0 });
			else
				this->missileRibbon->setRibbonColor(Vector{ 0,0,10 });
		}
		else if (this->state == MISSILE_STATE::DETONATING)
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

		this->setPositionIgnoringAllChildren(newPosition);
		//this->setPosition(newPosition);

		if (this->totalTime_sec < deltaTSec) {
			this->state = MISSILE_STATE::DETONATING;
		}
	}

	void WOInterceptorMissile::launch()
	{
		this->state = MISSILE_STATE::FLYING;
		this->rotateToIdentity();
		this->rotateAboutRelZ(sin(PI / 2 + (this->headingDeg) * PI / 180) - atan(this->getLookDirection().x / this->getLookDirection().y));
		this->launchTime = std::chrono::system_clock::now();
		this->missileRibbon->clear();
		this->missileRibbon->start();
	}

	void WOInterceptorMissile::reset()
	{
		this->setPosition(this->launchPosition); //Set by the config file
		this->state = MISSILE_STATE::INERT;
		this->missileRibbon->clear();
	}

	Vector WOInterceptorMissile::getDirection() {
		double V_horz = this->range_m / this->totalTime_sec;
		double horzPos = V_horz * this->totalTime_sec;
		double xComp = horzPos * std::sin((this->headingDeg + 90) * Aftr::DEGtoRAD);
		double yComp = horzPos * std::cos((this->headingDeg + 90) * Aftr::DEGtoRAD);

		Vector pos = Vector{ static_cast<float>(xComp), static_cast<float>(yComp), 0 };

		return pos;
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

	void WOInterceptorMissile::subscribeToCollisionDetection(MissileCallback_onCollisionDetected detectedMissileCollision) {
		this->callback_onCollisionDetected = detectedMissileCollision;
	}

	std::vector<double> WOInterceptorMissile::getBlueTrajectory(std::pair<std::tuple<double, double>, std::tuple<Vector, Vector>> timepoints, double baseRad)
	{
		std::tuple<double, double> red_times;
		std::tuple<Vector, Vector> red_points;
		std::tie(red_times, red_points) = timepoints;

		double red_time1, red_time2;
		std::tie(red_time1, red_time2) = red_times;
		Vector red_point1, red_point2;
		std::tie(red_point1, red_point2) = red_points;	//split out points and times


		auto red_horz_disp = sqrt(pow((red_point1.x - red_point2.x), 2) + pow(red_point1.y - red_point1.y, 2));
		auto red_horz_vel = red_horz_disp / (red_time2 - red_time1);

		auto red_vert_disp = abs(red_point1.z - red_point2.z);


		auto red_vert_vel = ((red_point2.z - red_point1.z) / (red_time2 - red_time1)) - (0.5 * Aftr::GRAVITY * (red_time2 - red_time1));



		// atan2 return an angle from [-pi, pi]
		auto red_heading = atan2(red_point2.y - red_point1.y, red_point2.x - red_point1.x);

		// Finding the component velocities requires the angle from the direction of travel, which is not returned by atan2
		auto horz_angle = red_heading;
		if (red_heading > PI / 2)
			horz_angle = red_heading - (PI / 2);
		else if (red_heading < (-PI / 2))
			horz_angle = abs(red_heading + (PI / 2));
		else if (red_heading < 0)
			horz_angle = abs(red_heading);

		auto red_horz_vel_x = (red_point2.x - red_point1.x) / (red_time2 - red_time1);
		auto red_horz_vel_y = (red_point2.y - red_point1.y) / (red_time2 - red_time1);

		bool limitFlag = true;

		auto pred_x = baseRad;
		auto pred_y = baseRad;
		auto pred_z = 0;
		auto pred_t = 0.0;
		auto new_z = 0.0;

		
		
			
		pred_t += 1;
		new_z = (red_vert_vel * pred_t) - ((0.5) * (Aftr::GRAVITY) * pow(pred_t, 2)) + red_point2.z;

		
		
		pred_z = new_z;

		 pred_x = red_horz_vel_x * pred_t + red_point2.x;
		pred_y = red_horz_vel_y * pred_t + red_point2.y;



		auto blue_horz_vel_x = pred_x / pred_t;
		auto blue_horz_vel_y = pred_y / pred_t;
		auto blue_vert_vel = (pred_z / pred_t) + (.5 * Aftr::GRAVITY * pred_t);

		auto blue_speed = sqrt(pow(blue_horz_vel_x,2) + pow(blue_horz_vel_y, 2) + pow(blue_vert_vel, 2));

		if (blue_speed < 60)
		{
			limitFlag = false;
		}
		while (limitFlag == true)
		{
			pred_t += 0.01;

			pred_x = red_horz_vel_x * pred_t + red_point2.x;
			pred_y = red_horz_vel_y * pred_t + red_point2.y;
			pred_z = (red_vert_vel * pred_t) - ((0.5) * (Aftr::GRAVITY) * pow(pred_t, 2)) + red_point2.z;

			

			 blue_horz_vel_x = pred_x / pred_t;
			 blue_horz_vel_y = pred_y / pred_t;
			 blue_vert_vel = (pred_z / pred_t) + (.5 * Aftr::GRAVITY * pred_t);

			 if (pred_z > 0) {
				 blue_speed = sqrt(pow(blue_horz_vel_x, 2) + pow(blue_horz_vel_y, 2) + pow(blue_vert_vel, 2));
			 }
			 else {
				 blue_speed = 0;
				 limitFlag = false;
			 }

			if (blue_speed < 60)
			{
				limitFlag = false;
			}

		}
		
		auto blue_time = 2 * (blue_vert_vel / Aftr::GRAVITY );
		auto blue_range = sqrt(pow(blue_horz_vel_x,2)+pow(blue_horz_vel_y,2)) * blue_time;
		auto blue_heading = 0 - Aftr::RADtoDEG*(atan2(blue_horz_vel_y, blue_horz_vel_x));
		



		std::vector<double> blue_traj;
		blue_traj.push_back(blue_heading);
		blue_traj.push_back(blue_time);
		blue_traj.push_back(blue_range);
		blue_traj.push_back(pred_x);
		blue_traj.push_back(pred_y);
		blue_traj.push_back(pred_z);
		



		return blue_traj;

	}
} //namespace Aftr
