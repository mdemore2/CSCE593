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

   //DO NOT MODIFY THIS CODE. When 't' is pressed and we are tracking a missile, this is
   //how the lambda is used, see declaration of CameraMotionLambda and 
   //createMyCameraTrackingBehavior and camTracker.
   if( this->camTracker )
   {
      auto [lookAt, pos] = (*this->camTracker)();
      this->cam->setCameraNormalDirection( Vector{ 0,0,1 } );
      this->cam->setCameraLookAtPoint( lookAt );
      this->cam->setPosition( pos );
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


void GLViewDefenseDaemon::onKeyDown( const SDL_KeyboardEvent& key )
{
   GLView::onKeyDown( key );

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
   else if( key.keysym.sym == SDLK_r )
   {
      const Uint8* state = SDL_GetKeyboardState( nullptr );
      if( state[SDL_SCANCODE_LSHIFT] )
      {
         std::cout << "Repopulating configuration data from ./aftr.conf...\n";
         this->repopulateFromConfFile();
      }
   }
   else if( key.keysym.sym == SDLK_t )
   {
      static bool lockCameraToMissile = false; //simply encode state toggling here
      lockCameraToMissile = !lockCameraToMissile;
      if( this->interceptor && lockCameraToMissile )
      {
         std::cout << "Camera is tracking missile during launch and detonation...\n";
         //Assign the camTracker function to the output of the higher order function createMyCameraTrackingBehavior.
      }
      else
      {
         std::cout << "Camera is in free look mode...\n";
         //Assign the camTracker function to std::nullopt, disable all tracking
      }
   }
   //lots of key presses to implement... generally, they are only a few lines and
   //simply call methods that actually do the work. If you're doing math or non-trivial
   //stateful logic here, you will likely want to re-examine your approach.

}


CameraMotionLambda GLViewDefenseDaemon::createMyCameraTrackingBehavior()
{
   //This is a higher order function. That is, this method returns a lambda
   //(function) that computes the current camera location based on the current
   //Missiles pose.

   //This method creates and returns a CameraMotionLambda. Below is simply
   //a stub so the code compiles, it does not do anything useful. Study
   //the usage in GLViewDefenseDaemon::updateWorld to figure out what to compute
   //and return.
   CameraMotionLambda f = []() {return std::make_tuple( Vector{}, Vector{} ); };
   return f;
}


void GLViewDefenseDaemon::onKeyUp( const SDL_KeyboardEvent& key )
{
   GLView::onKeyUp( key );
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

      //enable shadow mapping, it will perform an orthographic projection from the camera's position, in its look direction, into a
      //texture who's size matches the frustum lengths. In this case 8192 / 16.
      this->light->isShadowGenerator( true );         //Toggle ME  to turn shadows ON or OFF!!!! False = off, True = on
      this->glRenderer->isUsingShadowMapping( true ); //Toggle ME  to turn shadows ON or OFF!!!! False = off, True = on
      if( this->glRenderer->isUsingShadowMapping() )
      {
         this->glRenderer->getShadowMapCamera().setCameraOrthographicSymmetricFrustumLengths( static_cast<float>( 8192 / 16 ), static_cast<float>( 8192 / 16 ) );
         this->glRenderer->getShadowMapCamera().setCameraNearClippingPlaneDistance( 1.0f );
         this->glRenderer->getShadowMapCamera().setCameraNearClippingPlaneDistance( 1000.0f );
      }
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
      this->interceptor = WOInterceptorMissile::New();
      this->interceptor->setPosition( Vector{ 25,100,0 } );
      std::cout << "Missile size is " << this->interceptor->getModel()->getBoundingBox().getlxlylz().toString() << "\n";
      this->worldLst->push_back( this->interceptor );
   }

   {
      //create a tower centered at {-10,0,0}. Its feet must perfectly lie on the ground plane. /models/DefenseDaemon/Radar/tower.3ds
      //Hint use the wo->getModel()->getBoundingBox().getLxLyLz() to determine the x,y, and z extents of each model



      //Place a dish on top of the tower centered on the tower's platform.
      //Hint use the wo->getModel()->getBoundingBox().getLxLyLz() to determine the x,y, and z extents of each model



      //Place the radar sensor at the exact location of the dish. The sensor is embedded in the dish at this location.
      //Follow these 5 steps to join the sensor with the dish via a joint (rigid transform).
      //1. Set the dish as the sensor's parent object.
      //2. Set the sensor's position equal to the dish's position
      //3. Lock the sensor's position w.r.t the dish via a joint
      //4. Push the sensor into the parent's child list
      //5. Be sure to invoke dish->attachSensor( sensor ).



      //Create lambdas to define:
      //   1. The onFilterFunc function used by the RadarSensor to filter raw data
      //   2. The onScanDataAvailable function used by the RadarSensor to make processed positional data available to subscribers.
      //Add targets, such as the interceptor and perhaps for ease of debugging, the camera, to the sensor's radar scan list
      //Set the filtering function and subscribe to receive tracking info.

      //Set the callback functions to simple stubs that simply display the information without useful processing (just for phase 2)
      
     
   }

   {
      WO* wo = WO::New( ManagerEnvironmentConfiguration::getSMM() + "/models/DefenseDaemon/Doppler/doppler.3ds", Vector{ 1,1,1 }, MESH_SHADING_TYPE::mstAUTO );
      float z = wo->getModel()->getBoundingBox().getlxlylz().z / 2.0f;
      wo->setPosition( Vector{ -10,30,z } );
      std::cout << "Doppler size is " << wo->getModel()->getBoundingBox().getlxlylz().toString() << "\n";
      this->worldLst->push_back( wo );
   }
   
   createDefenseDaemonWayPoints();
   this->repopulateFromConfFile();

}

void GLViewDefenseDaemon::repopulateFromConfFile()
{
   ManagerEnvironmentConfiguration::populateFromFile();

   double headingDeg = AftrUtilities::toDouble( ManagerEnvironmentConfiguration::getVariableValue( "headingDeg" ) );
   // lots of other conf params to read in... Hint, the  Vector class takes a string as a constructor parameter...
   //...

   std::cout << "Repopulated from conf file... New values are:\n";
   std::cout << "headingDeg = " << headingDeg << "\n";
   // it's helpful to print all the newly parsed values to sanity check them
   //...

   //finally, we get to use those parameters

   //you now have enough information to set a strategy pattern for the interceptor based on conf
   //you also have enough info to update your Radar sensors scanning parameters
}


void GLViewDefenseDaemon::createDefenseDaemonWayPoints()
{
   // Create a waypoint with a radius of 3, a frequency of 5 seconds, activated by GLView's camera, and is visible.
   WayPointParametersBase params(this);
   params.frequency = 5000;
   params.useCamera = true;
   params.visible = true;
   WOWayPointSpherical* wayPt = WOWP1::New( params, 3 );
   wayPt->setPosition( Vector( 50, 0, 3 ) );
   worldLst->push_back( wayPt );
}
