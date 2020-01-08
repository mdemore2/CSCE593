#pragma once

#include "GLView.h"
#include "WOTrackingRibbonBase.h"
#include "..\..\gnuplot-iostream\gnuplot-iostream.h"
#include "ActivePlotter.h"

namespace Aftr
{
	class Camera;
	class WOLight;
	class WOInterceptorMissile;
	class WORadarDish;
	class WORadarSensor;

	/**
	   \class GLViewDefenseDaemon
	   \author Scott Nykl
	   \brief A child of an abstract GLView. This class is the top-most manager of the module.

	   Read \see GLView for important constructor and init information.

	   \see GLView

		\{
	*/

	using CameraMotionLambda = std::function< std::tuple<Vector, Vector>() >;

	class GLViewDefenseDaemon : public GLView
	{
	public:
		static GLViewDefenseDaemon* New(const std::vector< std::string >& outArgs);
		virtual ~GLViewDefenseDaemon();
		virtual void updateWorld(); ///< Called once per frame
		virtual void loadMap(); ///< Called once at startup to build this module's scene
		virtual void createDefenseDaemonWayPoints();
		virtual void onResizeWindow(GLsizei width, GLsizei height);
		virtual void onMouseDown(const SDL_MouseButtonEvent& e);
		virtual void onMouseUp(const SDL_MouseButtonEvent& e);
		virtual void onMouseMove(const SDL_MouseMotionEvent& e);
		virtual void onKeyDown(const SDL_KeyboardEvent& key);
		virtual void onKeyUp(const SDL_KeyboardEvent& key);

		//This is a higher order function -- it creates and returns a function.
		CameraMotionLambda createMyCameraTrackingBehavior();

		void repopulateFromConfFile();
		using InParam = std::vector<std::pair<float, float>>; //What type should go in the ...
		ActivePlotter<InParam>::Worker_Func_Prototype asyncPlotFunc;
		std::unique_ptr< ActivePlotter<InParam> > realTimePlotter = nullptr;


	protected:
		GLViewDefenseDaemon(const std::vector< std::string >& args);
		virtual void onCreate();

		/// Convenience pointer to useful WOs
		WOLight* light = nullptr;
		WOInterceptorMissile* interceptor = nullptr;
		WORadarSensor* sensor = nullptr;
		WORadarDish* dish = nullptr;
		std::optional<CameraMotionLambda> camTracker = std::nullopt;

	};

	/** \} */

} //namespace Aftr