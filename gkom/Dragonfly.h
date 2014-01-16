#ifndef _DRAGONFLY
#define _DRAGONFLY

#include <windows.h>
#include <GL/gl.h>
#include "glut.h"
#include "Coordinates.h"
#include "Triangles.h"

class Dragonfly
{
	private:
		const Coordinates* coordinates;
		Triangles* bodyTriangles;
		Triangles* wingsTriangles[4];
		Triangles* tailTriangles[5];
		
		Triangles* lathe(float(Dragonfly::*generationFunction)(const float), float begin, float end, int verticalSegments = 10, int horizontalSegments = 36, float radius = 2);

		float edge(const float x);
		float tail(const float x);
		float body(const float x);
		float beak(const float x);
		float eye(const float x);
		Triangles* circle(const float radius, const int segments);
	public:
		Dragonfly();
		~Dragonfly();

		void draw(const int elapsedTimeMilis);

		void setCoordinates(const Coordinates& coordinates);
		const Coordinates& getCoordinates() const;
};

#endif