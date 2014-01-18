#include "LightSource.h"

#include <iostream>

LightSource::LightSource(int id, float* position) : id(id)
{
	setPosition(position);

	disable();
	this->isTurnedOn = false;

	setAmbient(0,0,0,1);
	setDiffuse(1,1,1,1);
	setSpecular(1,1,1,1);

	float dir[] = {0, 0, -1};
	setDirection(dir);

	setSpotCutOff(180);
}

LightSource::~LightSource()
{
	
}

void LightSource::draw()
{
	glLightfv(id, GL_AMBIENT, ambient);
	glLightfv(id, GL_DIFFUSE, diffuse);
	glLightfv(id, GL_SPECULAR, specular);
	glLightfv(id, GL_SPOT_DIRECTION, this->direction);
	glLightf(id, GL_SPOT_CUTOFF, this->spotCutOff);
	glLightfv(id, GL_POSITION, position);
}

void LightSource::setPosition(float* position)
{
	this->position[0] = position[0];
	this->position[1] = position[1];
	this->position[2] = position[2];
	this->position[3] = position[3];
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
}

void LightSource::setDiffuse(float r, float g, float b, float a)
{
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = a;
}

void LightSource::setSpecular(float r, float g, float b, float a)
{
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
	specular[3] = a;
}

void LightSource::setDirection(float* direction)
{
	this->direction[0] = direction[0];
	this->direction[1] = direction[1];
	this->direction[2] = direction[2];
}

void LightSource::setSpotCutOff(float angle)
{
	this->spotCutOff = angle;
}

void LightSource::brighter()
{
	this->diffuse[0] += 0.1f;
	this->diffuse[1] += 0.1f;
	this->diffuse[2] += 0.1f;
	//diffuse[3] += 0.1;
}

void LightSource::darker()
{
	diffuse[0] -= 0.1f;
	diffuse[1] -= 0.1f;
	diffuse[2] -= 0.1f;
	//diffuse[3] -= 0.1;
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