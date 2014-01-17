#ifndef _LIGHTS_MANAGER
#define _LIGHTS_MANAGER

#include "LightSource.h"

class LightsManager
{
	private:
		LightSource* lights[8];

		float globalAmbient[4];
	public:
		const static int GLOBAL_AMBIENT_DEFAULT = 0;

		LightsManager(float* globalAmbient);
		~LightsManager();

		void drawLights();
		void drawGlobalAmbient();

		void setGlobalAmbientBrighter();
		void setGlobalAmbientDarker();

		void addLight(int id, float* position);

		void enableLighting();
		void disableLighting();

		void enableAllLights();
		void disableAllLights();

		LightSource* getLight(int id);

		void setGlobalAmbient(float* globalAmbient);
		const float* getGlobalAmbient();
};

#endif