#ifndef _CAMERA
#define _CAMERA

#include "Coordinates.h"



class Camera
{
	private:
		float angleXZ;
		float angleYZ;

		float movingSpeed;
		float rotationSensitivity;

		// miejsce gdzie stoi obserwator
		Coordinates* position;
		// w jakim kierunku patrzy
		Coordinates* direction;
		

	public:
		enum Direction
		{
			CAMERA_LEFT = 0,
			CAMERA_RIGHT = 1,
			CAMERA_FORWARD = 2,
			CAMERA_BACKWARD = 3
		};

		Camera();
		~Camera();

		const Coordinates& getPosition() const;
		void setPosition(Coordinates& coordinates);
		
		const Coordinates& getDirection() const;
		void setDirection(Coordinates& coordinates);
		
		const float getAngleXZ() const;
		void setAngleXZ(float angleXZ);
		
		const float getAngleYZ() const;
		void setAngleYZ(float angleYZ);

		void rotate(float inXZ, float inYZ);

		void move(const Camera::Direction dir);
};

#endif