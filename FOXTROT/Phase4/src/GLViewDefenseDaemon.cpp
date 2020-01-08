#include "GLViewDefenseDaemon.h"

#include "WorldList.h" //This is where we place all of our WOs
#include "ManagerOpenGLState.h" //We can change OpenGL State attributes with this
#include "Axes.h" //We can set Axes to on/off with this
#include "PhysicsEngineODE.h"

//Different WO used by this module
#include "WO.h"
#include "WOStatic.h"
#include "WOStaticPlane.h"
#include "WOStaticTrimesh.h"
#include "WOTrimesh.h"
#include "WOHumanCyborg.h"
#include "WOHumanCal3DPaladin.h"
#include "WOWayPointSpherical.h"
#include "WOLight.h"
#include "WOSkyBox.h"
#include "WOCar1970sBeater.h"
#include "Camera.h"
#include "CameraStandard.h"
#include "CameraChaseActorSmooth.h"
#include "CameraChaseActorAbsNormal.h"
#include "CameraChaseActorRelNormal.h"
#include "Model.h"
#include "ModelDataShared.h"
#include "ModelMesh.h"
#include "ModelMeshDataShared.h"
#include "ModelMeshSkin.h"
#include "WONVStaticPlane.h"
#include "WONVPhysX.h"
#include "WONVDynSphere.h"
#include "AftrGLRendererBase.h"
#include "WOInterceptorMissile.h"
#include "WORadarSensor.h"
#include "WORadarDish.h"
#include <boost/algorithm/string.hpp>
#include <typeinfo>
#include "PolyFit.h"
#include <map> 
#include <iterator> 
#include <fstream>
#include <sstream>

//If we want to use way points, we need to include this.
#include "DefenseDaemonWayPoints.h"

using namespace Aftr;

GLViewDefenseDaemon* GLViewDefenseDaemon::New( const std::vector< std::string >& args )
{
   GLViewDefenseDaemon* glv = new GLViewDefenseDaemon( args );
   glv->init( Aftr::GRAVITY, Vector( 0, 0, -1.0f ), "aftr.conf", PHYSICS_ENGINE_TYPE::petODE );
   glv->onCreate();
   return glv;
}


GLViewDefenseDaemon::GLViewDefenseDaemon( const std::vector< std::string >& args ) : GLView( args )
{
   //Generally, for the radar program, you do NOT need to add any code into this method - prefer
   //the loadMap()...
   //Initialize any member variables that need to be used inside of LoadMap() here; alternatively,
   //just initizlie those values in the header file or at the top of loadMap().
   //Note: At this point, the Managers are not yet initialized. The Engine initialization
   //occurs immediately after this method returns (see GLViewDefenseDaemon::New() for
   //reference). Then the engine invoke's GLView::loadMap() for this module.
   //After loadMap() returns, GLView::onCreate is finally invoked.

   //The order of execution of a module startup:
   //GLView::New() is invoked:
   //    calls GLView::init()
   //       calls GLView::loadMap() (as well as initializing the engine's Managers)
   //    calls GLView::onCreate()

   //GLViewDefenseDaemon::onCreate() is invoked after this module's LoadMap() is completed.
}


void GLViewDefenseDaemon::onCreate()
{
   //GLViewDefenseDaemon::onCreate() is invoked after this module's LoadMap() is completed.
   //At this point, all the managers are initialized. That is, the engine is fully initialized.

   if( this->pe != NULL )
   {
      //optionally, change gravity direction and magnitude here
      //The user could load these values from the module's aftr.conf
      this->pe->setGravityNormalizedVector( Vector( 0,0,-1.0f ) );
      this->pe->setGravityScalar( Aftr::GRAVITY );
   }
   //this->setNumPhysicsStepsPerRender( 0 ); //pause physics engine on start up; will remain paused till set to 1
}


GLViewDefenseDaemon::~GLViewDefenseDaemon()
{
   //Implicitly calls GLView::~GLView()
}


void GLViewDefenseDaemon::updateWorld()
{
   GLView::updateWorld(); //Just call the parent's update world first.
                          //If you want to add additional functionality, do it after
                          //this call.

   if( this->camTracker )
   {
      auto [lookAt, pos] = (*this->camTracker)();
      this->cam->setCameraNormalDirection( Vector{ 0,0,1 } );
      this->cam->setCameraLookAtPoint( lookAt );
      this->cam->setPosition( pos );
   }
   if (toggle_radar_move)
	   this->dish->rotateAboutRelZ(sensor->getRotationsPSec()*6*(PI/180));
   if (this->makeTests && this->auto_test->time_has_passed(std::chrono::system_clock::now())) {
	   this->end_test();
   }
}


void GLViewDefenseDaemon::onResizeWindow( GLsizei width, GLsizei height )
{
   GLView::onResizeWindow( width, height ); //call parent's resize method.
}


void GLViewDefenseDaemon::onMouseDown( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseDown( e );
}


void GLViewDefenseDaemon::onMouseUp( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseUp( e );
}


void GLViewDefenseDaemon::onMouseMove( const SDL_MouseMotionEvent& e )
{
   GLView::onMouseMove( e );
}

/// <summary>Generates Red Missiles then adds them to the world list, red_missiles global vector, and sensor target list</summary>
/// <param name="num_missiles"> The number of red missles to make</param>  
/// <returns>Void</returns> 
void GLViewDefenseDaemon::generateRedMissles(const int num_missiles)
{
	for (int i = 0; i < num_missiles; i++)
	{
		auto missile = WOInterceptorMissile::New();
		red_missiles.push_back(missile);
		worldLst->push_back(missile);
		sensor->addPossibleTargetToRadarScanList(missile);
	}
}


void GLViewDefenseDaemon::onKeyDown( const SDL_KeyboardEvent& key )
{
   GLView::onKeyDown( key );
  
   // Lighting
   if( key.keysym.sym == SDLK_l )
   {
      const Uint8* state = SDL_GetKeyboardState( nullptr );
      if( state[SDL_SCANCODE_LSHIFT] )
      {
         this->light->isDirectionalLight( !this->light->isDirectionalLight() );
         std::cout << "Light is now set to ";
         this->light->isDirectionalLight() ? std::cout << "directional " : std::cout << "point ";
         std::cout << "light...\n";
      }
      else
      {
         if( this->light->isDirectionalLight() )
         {
            std::cout << "Setting Light Direction to " << this->cam->getLookDirection() << "\n";
            this->light->getModel()->setLookDirection( this->cam->getLookDirection() );
         }
         else
         {
            std::cout << "Setting Light Position to " << this->cam->getPosition() << "\n";
            this->light->setPosition( this->cam->getPosition() );
         }
      }
   }
   // Repopulate Config file
   else if( key.keysym.sym == SDLK_r )
   {
      const Uint8* state = SDL_GetKeyboardState( nullptr );
      if( state[SDL_SCANCODE_LSHIFT] )
      {
         std::cout << "Repopulating configuration data from ./aftr.conf...\n";
         this->repopulateFromConfFile();
      }
   }
   // Launch Red Missiles
   else if (key.keysym.sym == SDLK_1)
   {
	   std::cout << "Launching Interceptor using params from conf file...\n";
	   if (this->red_missiles.empty())
	   {
		   std::cout << "NO MISSLES TO LAUNCH. GENERATE USING KEYS 1-9\n";
	   }
	   else
	   {
		   for (auto missile : red_missiles)
		   {
			   missile->launch();
		   }
	   }
   }
   // Reset the missiles
   else if (key.keysym.sym == SDLK_2)
   {
	   std::cout << "Resetting Interceptor back to origin...\n";
	   //if (this->interceptor != nullptr)
		  // this->interceptor->reset();

	   if (!this->red_missiles.empty())
	   {
		   for (auto missile : this->red_missiles)
			   worldLst->eraseViaWOptr(missile);
		   red_missiles.clear();
	   }

	   if (!this->blue_missiles.empty())
	   {
		   for (auto missile : this->blue_missiles)
			   worldLst->eraseViaWOptr(missile);
		   blue_missiles.clear();
	   }

	   if (!this->way_points.empty())
	   {
		   for (auto way_pt : way_points)
			   worldLst->eraseViaWOptr(way_pt);
			way_points.clear();
		}

	   this->sensor->resetMissilePoints();
   }
   // Print out trajectory info
   else if (key.keysym.sym == SDLK_p)
   {
	   std::cout << "Printing Trajectory info...\n";
	   std::cout << "Camera Position: " << this->cam->getPosition() << "\n";
	   //std::cout << this->interceptor->getTrajectoryInfo() << "\n";
   }
   // Start test Run
   else if( key.keysym.sym == SDLK_t )
   {
	   std::ofstream test_data;
	   test_data.open("test_data.csv");
	   test_data << "Rot Per Sec (Deg),";
	   test_data << "Wedge Angle (Deg),";
	   test_data << "Scan Range (m),";
	   test_data << "Qty Inbound Red,";
	   test_data << "Time of Red Arrival (sec),";
	   test_data << "Time to Lock (sec),";
	   test_data << "Success?\n";
	   generateRedMissles(4);
	   for (int i = 1; i <= 11; i++)
	   {
		   auto rot = sensor->getRotationsPSec();
		   auto wedge_angle = sensor->getHorzFOVDeg();
		   auto scan_range = sensor->getScanRangeMeters();
		   auto qty_inbound = std::size(red_missiles);

	   }

   }
   // Print out sensor info
   else if (key.keysym.sym == SDLK_d)
   {
	   std::cout << "Printing Sensor info...\n";
	   std::cout << this->sensor->getSensorInfo() << "\n";
   }
   // Move Radar manually
   else if (key.keysym.sym == SDLK_s)
   {
	   const Uint8* state = SDL_GetKeyboardState(nullptr);
	   if (state[SDL_SCANCODE_LSHIFT])
	   {
		   // Rotat the Radar dish by 5 deg CW
		   this->dish->rotateAboutRelZ(-((PI / 180) * 5));
	   }
	   else {
		   // Rotate the Radar dish by 5 deg CCW
		   this->dish->rotateAboutRelZ((PI/180)*5);
	   }

   }
   // Toggle radar automatic movement
   else if (key.keysym.sym == SDLK_a)
   {
		this->toggle_radar_move = !this->toggle_radar_move;

   }
   // Toggle Radar Wedge Visibility
   else if (key.keysym.sym == SDLK_v)
   {
	   // Toggle Visibility of radar wedge
	   if (this->sensor->isVisible)
	   {
		   this->sensor->makeMeAndAllMyChildrenInvisible();
	   }
	   else
	   {
		   this->sensor->makeMeAndAllMyChildrenVisible();
	   }
   } 
   // Create 1 red missile
   else if (key.keysym.sym == SDLK_3) {

		// To reset the stage for the new setup, we need to
		// first remove all of the missiles from the world list
		// and then we need to clear the missile vector
		if (!this->red_missiles.empty())
		{
			for(auto missile : this->red_missiles)
				this->worldLst->eraseViaWOptr(missile);
		}
		red_missiles.clear();

		int num_missles = 1;
		generateRedMissles(num_missles);
		repopulateFromConfFile();
		//this->cam->setPosition(Vector(200, 200, 200));
		//this->cam->setCameraLookAtPoint(Vector(0, 0, 0));

	}
	// Create 2 red missiles
   else if (key.keysym.sym == SDLK_4) {

	   // To reset the stage for the new setup, we need to
	   // first remove all of the missiles from the world list
	   // and then we need to clear the missile vector
	   if (!this->red_missiles.empty())
	   {
		   for (auto missile : this->red_missiles)
			   this->worldLst->eraseViaWOptr(missile);
	   }
		red_missiles.clear();

	   int num_missles = 2;

	   generateRedMissles(num_missles);
	   repopulateFromConfFile();
	   this->cam->setPosition(Vector(200, 200, 200));
	   this->cam->setCameraLookAtPoint(Vector(0, 0, 0));
	}
	// Create 4 red missiles
   else if (key.keysym.sym == SDLK_5)
   {

	   // To reset the stage for the new setup, we need to
	   // first remove all of the missiles from the world list
	   // and then we need to clear the missile vector
	   if (!this->red_missiles.empty())
	   {
		   for (auto missile : this->red_missiles)
			   this->worldLst->eraseViaWOptr(missile);
	   }
		red_missiles.clear();
	   int num_missles = 4;

	   generateRedMissles(num_missles);
	   repopulateFromConfFile();
	   this->cam->setPosition(Vector(200, 200, 200));
	   this->cam->setCameraLookAtPoint(Vector(0, 0, 0));
   }
   // Create 8 red missiles
   else if (key.keysym.sym == SDLK_6)
   {

	   // To reset the stage for the new setup, we need to
	   // first remove all of the missiles from the world list
	   // and then we need to clear the missile vector
	   if (!this->red_missiles.empty())
	   {
		   for (auto missile : this->red_missiles)
			   this->worldLst->eraseViaWOptr(missile);
	   }
		red_missiles.clear();

	   int num_missles = 8;

	   generateRedMissles(num_missles);
	   repopulateFromConfFile();
	   this->cam->setPosition(Vector(200, 200, 200));
	   this->cam->setCameraLookAtPoint(Vector(0, 0, 0));
   }
   // Create 16 red mssiles
   else if (key.keysym.sym == SDLK_7)
   {

	   // To reset the stage for the new setup, we need to
	   // first remove all of the missiles from the world list
	   // and then we need to clear the missile vector
	   if (!this->red_missiles.empty())
	   {
		   for (auto missile : this->red_missiles)
			   this->worldLst->eraseViaWOptr(missile);
	   }
		red_missiles.clear();

	   int num_missles = 16;

	   generateRedMissles(num_missles);
	   repopulateFromConfFile();
	   this->cam->setPosition(Vector(200, 200, 200));
	   this->cam->setCameraLookAtPoint(Vector(0, 0, 0));
   }
   // Create 32 red missiles
   else if (key.keysym.sym == SDLK_8)
   {

	   // To reset the stage for the new setup, we need to
	   // first remove all of the missiles from the world list
	   // and then we need to clear the missile vector
	   if (!this->red_missiles.empty())
	   {
		   for (auto missile : this->red_missiles)
			   this->worldLst->eraseViaWOptr(missile);
	   }
		red_missiles.clear();

	   int num_missles = 32;

	   generateRedMissles(num_missles);
	   repopulateFromConfFile();
	   this->cam->setPosition(Vector(200, 200, 200));
	   this->cam->setCameraLookAtPoint(Vector(0, 0, 0));
   }
   // Create 64 red missiles
   else if (key.keysym.sym == SDLK_9)
   {

	   // To reset the stage for the new setup, we need to
	   // first remove all of the missiles from the world list
	   // and then we need to clear the missile vector
	   if (!this->red_missiles.empty())
	   {
		   for (auto missile : this->red_missiles)
			   this->worldLst->eraseViaWOptr(missile);
	   }
	   red_missiles.clear();

	   int num_missles = 64;

	   generateRedMissles(num_missles);
	   repopulateFromConfFile();
	   this->cam->setPosition(Vector(200, 200, 200));
	   this->cam->setCameraLookAtPoint(Vector(0, 0, 0));
   }
}

//CameraMotionLambda GLViewDefenseDaemon::createMyCameraTrackingBehavior()
//{
//   //This is a higher order function. That is, this method returns a lambda
//   //(function) that computes the current camera location based on the current
//   //Missiles pose.
//
//   //This method creates and returns a CameraMotionLambda. 
//	CameraMotionLambda f = [=]() {
//		auto cameraPos = this->interceptor->getPosition();
//		cameraPos.x -= 5 * this->interceptor->getLookDirection().x; 
//		cameraPos.y -= 5 * this->interceptor->getLookDirection().y;
//		cameraPos.z += 1;
//		return std::make_tuple(this->interceptor->getPosition(), cameraPos);*/ };
//	return f;
//}

void GLViewDefenseDaemon::onKeyUp( const SDL_KeyboardEvent& key )
{
   GLView::onKeyUp( key );
}

void GLViewDefenseDaemon::automated_test()
{
	ManagerEnvironmentConfiguration::populateFromFile();
	float horzFOVDeg = AftrUtilities::toFloat(ManagerEnvironmentConfiguration::getVariableValue("radarFieldOfViewDeg"));
	float scanRangeMeters = AftrUtilities::toFloat(ManagerEnvironmentConfiguration::getVariableValue("radarScanRangeMeters"));
	int baseProtectionRadius = AftrUtilities::toInt(ManagerEnvironmentConfiguration::getVariableValue("baseProtectionRadius"));

	auto test_params = this->auto_test->params_new_test();
	if (std::get<0>(test_params) == 0) {
		std::ofstream outputFile("programdata.txt");
		outputFile << this->test_output.str();
		outputFile.close();
		this->makeTests = false;
	}
	else {
		this->test_output << "Quantity Inboud: " << std::to_string(std::get<1>(test_params)) << " Time to Arrival: " << std::to_string(std::get<0>(test_params)) << " Rotations: " << std::to_string(std::get<2>(test_params)) << "\n";
		std::cout << "test: Quantity Inboud : " << std::to_string(std::get<1>(test_params)) << " Time to Arrival : " << std::to_string(std::get<0>(test_params)) << " Rotations : " << std::to_string(std::get<2>(test_params)) << "\n";
		toggle_radar_move = true;
		automated_test_helper(std::get<1>(test_params), std::get<0>(test_params), std::get<2>(test_params), scanRangeMeters, horzFOVDeg, baseProtectionRadius);
	}
}

void  GLViewDefenseDaemon::end_test() {
	this->test_output << "Missiles Killed: " << this->sensor->getKilledMissiles() << "\n";
	if (!this->red_missiles.empty())
	{
		for (auto missile : this->red_missiles)
			worldLst->eraseViaWOptr(missile);
		red_missiles.clear();
	}

	if (!this->blue_missiles.empty())
	{
		for (auto missile : this->blue_missiles)
			worldLst->eraseViaWOptr(missile);
		blue_missiles.clear();
	}

	if (!this->way_points.empty())
	{
		for (auto way_pt : way_points)
			worldLst->eraseViaWOptr(way_pt);
		way_points.clear();
	}
	this->red_missiles.clear();
	this->blue_missiles.clear();
	this->sensor->resetMissilePoints();
	this->automated_test();
}


void GLViewDefenseDaemon::automated_test_helper(int qty, int time, float rotations, float scanRangeMeters, float horzFOVDeg, int baseProtectionRadius) {
	this->sensor->setRotationSpeed(rotations);
	generateRedMissles(qty);
	//need to set motion strategy for the red missiles
	float heading = 0, missile_distance = 0;
	for (size_t i = 1; i <= std::size(red_missiles);i++)
	{
		// Each missile will need a strategy
		auto strat = std::make_unique< MotionParabolic >();

		auto missile = red_missiles.at(i - 1);
		// The angle between missiles will be equal
		float angle_from_origin = ((360.0 / std::size(red_missiles)) * (i - 1)) * (PI / 180);
		// Missile is launch 10m past the scan radius
		missile_distance = scanRangeMeters + 10;
		// Find x & y of the missile launch locatoin
		float launch_pos_y = sin(angle_from_origin) * missile_distance;
		float launch_pos_x = cos(angle_from_origin) * missile_distance;
		Vector launch_pos = Vector(launch_pos_x, launch_pos_y, 0);

		missile->setPosition(launch_pos);

		// Generate a random x position for the missile to launch to
		auto rand_x = rand() % (baseProtectionRadius * 2) + 1 - baseProtectionRadius;
		// Generates a random y based one where the x was generated
		// The 0 case must be taken into account since the below
		// modulus would fail.
		auto rand_y = 0;
		if (rand_x != baseProtectionRadius)
		{
			// Uses the equation of a circle to get the limits
			int y_range = sqrt(pow(baseProtectionRadius, 2) - pow(rand_x, 2));
			rand_y = rand() % (y_range * 2) + 1 - y_range;
		}

		// Find the angle from the the red missile location to the randomly generated spot
		float loc_angle = atan2((rand_y - launch_pos_y), (rand_x - launch_pos_x)) * (180 / PI);
		loc_angle *= -1; // Your guess is better than mine

		missile->setMotionStrategy(std::move(strat), missile_distance, time, loc_angle, launch_pos);
		missile->launch();
		this->auto_test->set_current_time(std::chrono::system_clock::now());
	}
}


void Aftr::GLViewDefenseDaemon::loadMap()
{
   this->worldLst = new WorldList(); //WorldList is a 'smart' vector that is used to store WO*'s
   this->actorLst = new WorldList();
   this->netLst = new WorldList();

   ManagerOpenGLState::GL_CLIPPING_PLANE = 10000.0;
   ManagerOpenGLState::GL_NEAR_PLANE = 1.0f;
   ManagerOpenGLState::enableFrustumCulling = false;
   Axes::isVisible = true;
   this->glRenderer->isUsingShadowMapping( false ); //set to TRUE to enable shadow mapping, must be using GL 3.2+

   this->setActorChaseType( STANDARDEZNAV ); //Default is STANDARDEZNAV mode
   this->cam->setPosition( 15, 15, 10 );
   this->cam->setPosition( 25.0f, -15.5f, 2.6f );
   this->cam->setCameraLookDirection( Vector{ -0.612123f, 0.765189f, 0.199524f } );

   //SkyBox Textures readily available
   std::vector< std::string > skyBoxImageNames; //vector to store texture paths
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/2KRes/Sunset_Happy122.png" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/2KRes/InTheClouds_Happy85.png" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/2KRes/InTheCloudsAfternoon_Happy112.png" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/2KRes/BlueGreenCloudy_Happy124.png" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/2KRes/CumulusBlueAfternoon_Happy11.png" );
   skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/2KRes/GreenSkyOverOceanBlue_Happy72.png" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_water+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_dust+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_mountains+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_winter+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/early_morning+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_afternoon+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_cloudy+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_cloudy3+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_day+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_day2+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_deepsun+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_evening+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_morning+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_morning2+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_noon+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_warp+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_Hubble_Nebula+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_gray_matter+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_easter+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_hot_nebula+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_ice_field+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_lemon_lime+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_milk_chocolate+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_solar_bloom+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_thick_rb+6.jpg" );

   {  //Setup lighting and shadows
      float ga = 0.2f; //Global Ambient Light level for this module
      ManagerLight::setGlobalAmbientLight( aftrColor4f( ga, ga, ga, 1.0f ) );
      this->light = WOLight::New();
      this->light->isDirectionalLight( true );
      //Set the light's display matrix such that it casts light in a direction parallel to the -z axis (ie, downwards as though it was "high noon")
      //for shadow mapping to work, this->glRenderer->isUsingShadowMapping( true ), must be invoked.
      this->light->getModel()->setDisplayMatrix( Mat4::rotateIdentityMat( { 0, 1, 0 }, 90.0f * Aftr::DEGtoRAD ) );
      this->light->isPointLight( true );
      this->light->setPosition( Vector{ 157.132f, -41.497f, 307.859f } ); //place light in the sky roughly where the sun aligns w/ the skybox...
      this->light->setLabel( "Light" );
      worldLst->push_back( this->light );
   }

   {  //if your graphics card doesn't support shadowing. Then comment out this block of code and run again.

      ////enable shadow mapping, it will perform an orthographic projection from the camera's position, in its look direction, into a
      ////texture who's size matches the frustum lengths. In this case 8192 / 16.
      //this->light->isShadowGenerator( true );         //Toggle ME  to turn shadows ON or OFF!!!! False = off, True = on
      //this->glRenderer->isUsingShadowMapping( true ); //Toggle ME  to turn shadows ON or OFF!!!! False = off, True = on
      //if( this->glRenderer->isUsingShadowMapping() )
      //{
      //   this->glRenderer->getShadowMapCamera().setCameraOrthographicSymmetricFrustumLengths( static_cast<float>( 8192 / 16 ), static_cast<float>( 8192 / 16 ) );
      //   this->glRenderer->getShadowMapCamera().setCameraNearClippingPlaneDistance( 1.0f );
      //   this->glRenderer->getShadowMapCamera().setCameraNearClippingPlaneDistance( 1000.0f );
      //}
   }

   {  //Create the SkyBox
      WO* wo = WOSkyBox::New( skyBoxImageNames.at( 0 ), this->getCameraPtrPtr() );
      wo->setPosition( Vector( 0, 0, 0 ) );
      wo->setLabel( "Sky Box" );
      wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
      worldLst->push_back( wo );
   }

   {  //Create the infinite grass plane (the floor)
      std::string grass( ManagerEnvironmentConfiguration::getSMM() + "/models/grassFloor400x400_pp.wrl" );
      WO* wo = WO::New( grass, Vector( 8, 8, 8 ), MESH_SHADING_TYPE::mstFLAT );
      wo->setPosition( Vector( 0, 0, 0 ) );
      wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
      ModelMeshSkin& grassSkin = wo->getModel()->getModelDataShared()->getModelMeshes().at( 0 )->getSkins().at( 0 );
      grassSkin.getMultiTextureSet().at( 0 )->setTextureRepeats( 40.0f );
      grassSkin.setAmbient( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Color of object when it is not in any light
      grassSkin.setDiffuse( aftrColor4f( 1.0f, 1.0f, 1.0f, 1.0f ) ); //Diffuse color components (ie, matte shading color of this object)
      grassSkin.setSpecular( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Specular color component (ie, how "shiny" it is)
      grassSkin.setSpecularCoefficient( 10 ); // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull)
      wo->setLabel( "Grass" );
      worldLst->push_back( wo );
   }

   {
      WO* wo = WO::New( ManagerEnvironmentConfiguration::getSMM() + "/models/DefenseDaemon/Launcher/RLauncher.3ds", Vector{ 1,1,1 }, MESH_SHADING_TYPE::mstAUTO );
      float z = wo->getModel()->getBoundingBox().getlxlylz().z / 2.0f;
      wo->setPosition( Vector{ 0,10,z } );
      std::cout << "RLauncher size is " << wo->getModel()->getBoundingBox().getlxlylz().toString() << "\n";
      this->worldLst->push_back( wo );
   }

   {
	   //Place a dish on top of the tower centered on the tower's platform.
	   //Hint use the wo->getModel()->getBoundingBox().getLxLyLz() to determine the x,y, and z extents of each model
	   this->dish = WORadarDish::New();
	   this->worldLst->push_back(this->dish);

      //create a tower centered at {-10,0,0}. Its feet must perfectly lie on the ground plane. /models/DefenseDaemon/Radar/tower.3ds
      //Hint use the wo->getModel()->getBoundingBox().getLxLyLz() to determine the x,y, and z extents of each model
	   WO* tower = WO::New(ManagerEnvironmentConfiguration::getSMM() + "/models/DefenseDaemon/Radar/tower.3ds", Vector{ 1,1,1 }, MESH_SHADING_TYPE::mstAUTO);
	   dish->getChildren().push_back(tower);
	   float z = tower->getModel()->getBoundingBox().getlxlylz().z / 2.0f;
	   auto dishPos = dish->getPosition();
	   tower->setPosition(Vector(dishPos.x - 1.5, dishPos.y, dishPos.z - 14.034));
	   tower->setParentWorldObject(dish);
	   this->sensor = WORadarSensor::New();
	   sensor->setParentWorldObject(dish);
	   sensor->setPosition(dish->getPosition());
	   sensor->lockWRTparent();
	   dish->attachSensor(sensor);
	   
	   // Processes raw sensor data. Given a target, it creates polar coordinates
	   auto onFilterFunc = [dish_pos = dish->getPosition(), this, tower](const WO* world_o, const std::chrono::system_clock::time_point& t)
	   {
		   unsigned int id = world_o->getID();
		   std::vector< Vector > polar_coords;
		   auto obj_pos = world_o->getPosition() - dish_pos;
		   auto now_ms = std::chrono::time_point_cast<std::chrono::microseconds>(t);

		   auto value = now_ms.time_since_epoch();
		   double time = (value.count() *.000001);
		   this->sensor->addMissilePoints(std::make_pair(id, std::tuple<Vector, double>(Vector{ obj_pos.x, obj_pos.y, obj_pos.z }, time)));
		   this->sensor->addTargetMissile(id);
		   
		   Vector polar;
		   float RangeinM, AzimuthDeg, ElevationDeg;
		   RangeinM = sqrt(pow(obj_pos.x,2) + pow(obj_pos.y,2) + pow(obj_pos.z,2));
		   AzimuthDeg = atan2(obj_pos.y, obj_pos.x) * (PI/180);
		   ElevationDeg = acos(obj_pos.z/RangeinM) * (PI / 180);

		   polar.x = RangeinM;
		   polar.y = AzimuthDeg;
		   polar.z = ElevationDeg;

		   polar_coords.push_back(polar);

		   return std::tuple(id, polar_coords);
	   };

	   auto onScanDataAvailable = [](unsigned int world_o, const std::chrono::system_clock::time_point& t, std::vector< Vector >&& polar_coords)
	   {
		   //std::cout << "Missle Sig " << world_o << " at " << polar_coords[0] << "\n";
	   };

	   auto onObjectIlluminatedByRadar = [](WO* obj, bool inWedge)
	   {
		   if (static_cast<WOInterceptorMissile*>(obj) != nullptr) {
			   WOInterceptorMissile* missile = (WOInterceptorMissile*)obj;
			   if (!missile->currentlyDetectsRadarBeams() && inWedge) {
				   missile->currentlyDetectsRadarBeams(true);
			   }
			   else if (missile->currentlyDetectsRadarBeams() && !inWedge)
				   missile->currentlyDetectsRadarBeams(false);
		   }
	   };

	   auto alertMissileCollision = [=](unsigned int ID, WO* blue_missile) {
		   //want to use this to alert a missile if it has collided with another missile
		   bool ret_val = false;
		   if (this->blue_missiles.size() < 1)
			   ret_val = false;

		   for (auto red_missile : red_missiles) 
		   {//so loop through red missiles. maybe add blue missiles later
			   //std::cout << "Blue Missile: " << blue_missile->getPosition() << "\n";
			   //std::cout << "Red Missile: " << red_missile->getPosition() << "\n";
			   auto x = pow((blue_missile->getPosition().x - red_missile->getPosition().x), 2);
			   auto y = pow((blue_missile->getPosition().y - red_missile->getPosition().y), 2);
			   auto z = pow((blue_missile->getPosition().z - red_missile->getPosition().z), 2);
			   auto distance = sqrt(x + y + z);
				  
			   //std::cout << "X Diff: " << pow((blue_missile->getPosition().x - red_missile->getPosition().x), 2) << "\n";
			   //std::cout << "Y Diff: " << pow((blue_missile->getPosition().y - red_missile->getPosition().y), 2) << "\n";
			   //std::cout << "Z Diff: " << pow((blue_missile->getPosition().z - red_missile->getPosition().z), 2) << "\n";
			   //std::cout << "Distance: " << distance << "\n";

			   if (distance <= red_missile->blast_radius)
			   {//if they are close, draw an explosion, send the red missile far far away and and return true

				   WayPointParametersBase params(this);
				   params.frequency = 5000;
				   params.useCamera = true;
				   params.visible = true;
				   WOWayPointSpherical* wayPt = WOWP1::New(params, red_missile->blast_radius);
				   wayPt->setPosition(red_missile->getPosition());
				   worldLst->push_back(wayPt);
				   way_points.push_back(wayPt);
				   this->sensor->increaseKilledMissiles();
				   //red_missile->makeMeAndAllMyChildrenInvisible();
				   //red_missile->setPosition(Vector(1000,1000,1000));

				   //blue_missile->makeMeAndAllMyChildrenInvisible();
				   //blue_missile->setPosition(Vector(1000, 1000, 1000));
				   
				   red_missile->onCollision();
				   //worldLst->eraseViaWOptr(this->red_missiles[i]);
				   //this->red_missiles.erase(red_missiles.begin() + i);

				   //this->red_missiles[i]->setPosition(Vector{ -10000, -10000, -10000 });
				   ret_val = true;
			   }
		   }
		   
		   return ret_val;

	   };

	   auto generateBlueMissle = [this, alertMissileCollision](unsigned int redMissileID)
	   {
			auto timesAndPoints = this->sensor->getTimesAndPoints(redMissileID);
			if(timesAndPoints.has_value())
			{
				// Create New Blue Missile
				auto missile = WOInterceptorMissile::New();
				// Sensor will track blue missile
				sensor->addPossibleTargetToRadarScanList(missile);

				// Calculate Blue missile trajectory to intercept red missile
				auto launchInfo = missile->getBlueTrajectory(*timesAndPoints, this->base_protection_rad);

				// TEST ONLY: Create waypoint for the predicted collsion point
				//WayPointParametersBase params(this);
				//params.frequency = 5000;
				//params.useCamera = true;
				//params.visible = true;
				//WOWayPointSpherical* wayPt = WOWP1::New(params, 3);
				//wayPt->setPosition(Vector(launchInfo[3],launchInfo[4], launchInfo[5]));
				//std::cout << "Setting point at " << Vector(launchInfo[3], launchInfo[4], launchInfo[5]) << "\n";
				//worldLst->push_back(wayPt);

				// Motion strategy for blue missile
				auto strat = std::make_unique< MotionParabolic >();
				missile->setMotionStrategy(std::move(strat), launchInfo[2], launchInfo[1], launchInfo[0], Vector(0,0,0));

				// Add blue missile to GLView's blue missiles
				blue_missiles.push_back(missile);
				// Blue missile has a sensor that will be alerted if it collides with something
				// Then executes the defined behaviour for a collision
				missile->subscribeToCollisionDetection(alertMissileCollision);
				// Add blue missile to sensor's blue list
				sensor->addAssignedMissile(missile->getID());
				// Let sensor know Red has been assigned
				sensor->addRedMissileAssigned(redMissileID);
				worldLst->push_back(missile);
				missile->launch();
			}

	   };

	   sensor->setFilteringOfRawScanDataAlgorithm(onFilterFunc);
	   sensor->subscribeToRadarTrackingInfo(onScanDataAvailable);
	   sensor->setIlluminationFunc(onObjectIlluminatedByRadar);
	   sensor->subscribeToUpdateBlueMissiles(generateBlueMissle);
   }

   {
      WO* wo = WO::New( ManagerEnvironmentConfiguration::getSMM() + "/models/DefenseDaemon/Doppler/doppler.3ds", Vector{ 1,1,1 }, MESH_SHADING_TYPE::mstAUTO );
      float z = wo->getModel()->getBoundingBox().getlxlylz().z / 2.0f;
      wo->setPosition( Vector{ -10,30,z } );
      std::cout << "Doppler size is " << wo->getModel()->getBoundingBox().getlxlylz().toString() << "\n";
      this->worldLst->push_back( wo );
   }

   createDefenseDaemonWayPoints();
   if (this->makeTests) {
	   this->auto_test = Tester::New();
	   this->automated_test();
   }
   else
	   this->repopulateFromConfFile();


}

void GLViewDefenseDaemon::repopulateFromConfFile()
{
   ManagerEnvironmentConfiguration::populateFromFile();
   double timeToImpact = AftrUtilities::toDouble(ManagerEnvironmentConfiguration::getVariableValue("timeToImpact"));
   int baseProtectionRadius = AftrUtilities::toInt(ManagerEnvironmentConfiguration::getVariableValue("baseProtectionRadius"));
   
   std::string dishPos_in = ManagerEnvironmentConfiguration::getVariableValue("radarDishPosition");
	// Boost lib takes in this type
	std::vector<std::string> dish_Pos;
	// Split by parenthesis, commas, and spaces
	boost::split(dish_Pos, dishPos_in, boost::is_any_of("() ,"));
	// Assign parsed values to vector and convert strs to doubles
	Vector dishPos = Vector(AftrUtilities::toDouble(dish_Pos[1]), AftrUtilities::toDouble(dish_Pos[2]), AftrUtilities::toDouble(dish_Pos[3]));
   dish->setPosition(dishPos);

   //finally, we get to use those parameters

   //you now have enough information to set a strategy pattern for the interceptor based on conf

   float horzFOVDeg = AftrUtilities::toFloat(ManagerEnvironmentConfiguration::getVariableValue("radarFieldOfViewDeg"));
   float scanRangeMeters = AftrUtilities::toFloat(ManagerEnvironmentConfiguration::getVariableValue("radarScanRangeMeters"));
   float rotPerSecond = AftrUtilities::toFloat(ManagerEnvironmentConfiguration::getVariableValue("rotationsPerSecond"));

   this->sensor->setHorzFOVDeg(horzFOVDeg);
   this->sensor->setScanRangeMeters(scanRangeMeters);
   this->sensor->setRotationSpeed(rotPerSecond);
   this->base_protection_rad = baseProtectionRadius;


   float heading = 0, missile_distance = 0;
   for (size_t i=1; i<=std::size(red_missiles);i++)
   {
		// Each missile will need a strategy
		auto strat = std::make_unique< MotionParabolic >();

	    auto missile = red_missiles.at(i - 1);
		// The angle between missiles will be equal
		float angle_from_origin = ((360.0 / std::size(red_missiles)) * (i - 1)) * (PI / 180);
		// Missile is launch 10m past the scan radius
		missile_distance = scanRangeMeters + 10;
		// Find x & y of the missile launch locatoin
		float launch_pos_y = sin(angle_from_origin) * missile_distance;
		float launch_pos_x = cos(angle_from_origin) * missile_distance;
		Vector launch_pos = Vector(launch_pos_x+this->sensor->getPosition().x, launch_pos_y+this->sensor->getPosition().y, 0);

		missile->setPosition(launch_pos);

		// Generate a random x position for the missile to launch to
		auto rand_x = rand() % (baseProtectionRadius * 2)+1 - baseProtectionRadius;
		// Generates a random y based one where the x was generated
		// The 0 case must be taken into account since the below
		// modulus would fail.
		auto rand_y = 0;
		if (rand_x != baseProtectionRadius)
		{
			// Uses the equation of a circle to get the limits
			int y_range = sqrt(pow(baseProtectionRadius, 2) - pow(rand_x, 2));
			rand_y = rand() % (y_range * 2) + 1 - y_range;
		}

		// Find the angle from the the red missile location to the randomly generated spot
		float loc_angle = atan2((rand_y-launch_pos_y), (rand_x-launch_pos_x))*(180/PI);
		loc_angle *= -1; // Your guess is better than mine

		missile->setMotionStrategy(std::move(strat), missile_distance, timeToImpact, loc_angle, launch_pos);
   }

   std::cout << "Repopulated from conf file... New values are:\n";
   std::cout << "headingDeg = " << heading << "\n";
   std::cout << "rangeMeters = " << missile_distance << "\n";
   std::cout << "timeToImpact = " << timeToImpact << "\n";

}


void GLViewDefenseDaemon::createDefenseDaemonWayPoints()
{
   // Create a waypoint with a radius of 3, a frequency of 5 seconds, activated by GLView's camera, and is visible.
   WayPointParametersBase params(this);
   params.frequency = 5000;
   params.useCamera = true;
   params.visible = true;
   WOWayPointSpherical* wayPt = WOWP1::New( params, this->base_protection_rad );
   wayPt->setPosition(Vector(0,0,0) );
   worldLst->push_back( wayPt );
}
