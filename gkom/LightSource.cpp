#include "LightSource.h"

LightSource::LightSource(int id, float* position) : id(id)
{
	this->position[0] = position[0];
	this->position[1] = position[1];
	this->position[2] = position[2];

	disable();
	this->isTurnedOn = false;

	setAmbient(0,0,0,1);
	setDiffuse(1,1,1,1);
	setSpecular(1,1,1,1);
}

LightSource::~LightSource()
{
	delete position;
}

void LightSource::draw()
{
	glLightfv(id, GL_POSITION, position);
}

void LightSource::setPosition(float* position)
{
	this->position[0] = position[0];
	this->position[1] = position[1];
	this->position[2] = position[2];
}

const float* LightSource::getPosition()
{
	return position;
}

void LightSource::setAmbient(float r, float g, float b, float a)
{
	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
	ambient[3] = a;
	glLightfv(id, GL_AMBIENT, ambient);
}

void LightSource::setDiffuse(float r, float g, float b, float a)
{
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = a;
	glLightfv(id, GL_DIFFUSE, diffuse);
}

void LightSource::setSpecular(float r, float g, float b, float a)
{
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
	specular[3] = a;
	glLightfv(id, GL_SPECULAR, specular);
}

void LightSource::enable()
{
	glEnable(id);
	this->isTurnedOn = true;
}

void LightSource::disable()
{
	glDisable(id);
	this->isTurnedOn = false;
}