#include "Dragonfly.h"

void Dragonfly::lathe(float(*generationFunction)(const float), float begin, float end, int verticalSegments = 10, int horizontalSegments = 36, float radius = 2)
{

}

float Dragonfly::edge(const float x)
{
	return -x*x*0.3;
}

float Dragonfly::tail(const float x)
{
	return 2+x/(x+0.5)*(-0.2*x);
}

float Dragonfly::body(const float x)
{
	return -(x*x*x*x*x*x)+1.5;
}

float Dragonfly::beak(const float x)
{
	return 5+x/(x+0.5)*(-6*x);
}

float Dragonfly::eye(const float x)
{
	return cos(x);
}

Dragonfly::Dragonfly()
{
	this->coordinates = new Coordinates(0, 0, 0);
	this->triangles = new std::list<Triangle*>();
}

Dragonfly::~Dragonfly()
{
	delete this->coordinates;
	delete triangles;
}

void Dragonfly::draw() const
{

}

void Dragonfly::setCoordinates(const Coordinates& coordinates)
{
	delete this->coordinates;

	this->coordinates = &coordinates;
}

const Coordinates& Dragonfly::getCoordinates() const
{
	return *(this->coordinates);
}