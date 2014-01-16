#ifndef _DRAGONFLY
#define _DRAGONFLY

#include <list>
#include "Coordinates.h"
#include "Triangle.h"

class Dragonfly
{
	private:
		const Coordinates* coordinates;
		const std::list<Triangle*>* triangles;
		
		void lathe(float(*generationFunction)(const float), float begin, float end, int verticalSegments = 10, int horizontalSegments = 36, float radius = 2);

		float edge(const float x);
		float tail(const float x);
		float body(const float x);
		float beak(const float x);
		float eye(const float x);
	public:
		Dragonfly();
		~Dragonfly();

		void draw() const;

		void setCoordinates(const Coordinates& coordinates);
		const Coordinates& getCoordinates() const;
};

#endif