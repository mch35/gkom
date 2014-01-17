#ifndef _LIGHT_SOURCE
#define _LIGHT_SOURCE

#include "Coordinates.h"
#include <windows.h>
#include <GL/gl.h>


class LightSource
{
	private:
		const int id;
		bool isTurnedOn;

		float position[3];
		float ambient[4];
		float diffuse[4];
		float specular[4];

	public:
		LightSource(int id, float* position);
		~LightSource();

		void draw();

		void setPosition(float* position);
		const float* getPosition();

		void setAmbient(float r, float g, float b, float a);
		void setDiffuse(float r, float g, float b, float a);
		void setSpecular(float r, float g, float b, float a);

		void brighter();
		void darker();

		void enable();
		void disable();
};

#endif