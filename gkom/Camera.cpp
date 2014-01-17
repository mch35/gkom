#define _USE_MATH_DEFINES
#include "Camera.h"
#include <cmath>
Camera::Camera() : angleXZ(-90), angleYZ(90)
{
	this->position = new Coordinates(0, 0, 0);
	this->direction = new Coordinates(0, 0, -1);

	this->movingSpeed = 0.1;
	this->rotationSensitivity = 0.5;
}

const Coordinates& Camera::getPosition() const
{
	return *position;
}

void Camera::setPosition(Coordinates& coordinates)
{
	delete this->position;

	this->position = &coordinates;
}
		
const Coordinates& Camera::getDirection() const
{
	return *direction;
}

void Camera::setDirection(Coordinates& coordinates)
{
	delete this->direction;

	this->direction = &coordinates;
}
		
const float Camera::getAngleXZ() const
{
	return this->angleXZ;
}

void Camera::setAngleXZ(float angleXZ)
{
	this->angleXZ = angleXZ;
}
		
const float Camera::getAngleYZ() const
{
	return this->angleYZ;
}

void Camera::setAngleYZ(float angleYZ)
{
	this->angleYZ = angleYZ;
}

void Camera::rotate(float inXZ, float inYZ)
{
	angleXZ -= rotationSensitivity*inXZ;
	angleYZ -= rotationSensitivity*inYZ;

	if(angleXZ < 0) angleXZ += 360;
	else if(angleXZ >= 360) angleXZ -= 360;

	if(angleYZ < 0) angleYZ = 0;
	else if(angleYZ >= 180) angleYZ = 180;
		
	direction->setX(cos(angleXZ * M_PI / 180));
	direction->setY(cos(angleYZ * M_PI / 180));
	direction->setZ(sin(angleXZ * M_PI / 180));
}

void Camera::move(const Camera::Direction dir)
{
		float step = 0.1;
	switch( dir )
    {
			// kursor w lewo
		case Direction::CAMERA_LEFT:
			position->setZ(position->getZ() - step*cos(angleXZ * M_PI / 180));
			position->setX(position->getX() + step*sin(angleXZ * M_PI / 180));
			break;
       
			// kursor w górê
		case Direction::CAMERA_FORWARD:		
			position->setX(position->getX() + step*cos(angleXZ * M_PI / 180));
			position->setY(position->getY() + step*cos(angleYZ * M_PI / 180));
			position->setZ(position->getZ() + step*sin(angleXZ * M_PI / 180));
			break;
       
			// kursor w prawo
		case Direction::CAMERA_RIGHT:        
			position->setZ(position->getZ() + step*cos(angleXZ * M_PI / 180));
			position->setX(position->getX() - step*sin(angleXZ * M_PI / 180));
			break;
       
			// kursor w dó³
		case Direction::CAMERA_BACKWARD:
			position->setX(position->getX() - step*cos(angleXZ * M_PI / 180));
			position->setY(position->getY() - step*cos(angleYZ * M_PI / 180));
			position->setZ(position->getZ() - step*sin(angleXZ * M_PI / 180));
			break;
    }
}