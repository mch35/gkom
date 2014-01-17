#ifndef _COORDINATES
#define _COORDINATES

class Coordinates
{
	private:
		float x;
		float y;
		float z;
	public:
		Coordinates(const float x, const float y, const float z) : x(x), y(y), z(z)
		{

		}

		~Coordinates()
		{

		}

		const float getX() const
		{
			return x;
		}

		void setX(float x)
		{
			this->x = x;
		}

		const float getY() const
		{
			return y;
		}

		void setY(float y)
		{
			this->y = y;
		}

		const float getZ() const
		{
			return z;
		}

		void setZ(float z)
		{
			this->z = z;
		}

};

#endif