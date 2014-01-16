#ifndef _COORDINATES
#define _COORDINATES

class Coordinates
{
	private:
		const float x;
		const float y;
		const float z;
	public:
		Coordinates(const float x, const float y, const float z) : x(x), y(y), z(z)
		{

		}

		~Coordinates()
		{

		}

		const float getX()
		{
			return x;
		}

		const float getY()
		{
			return y;
		}

		const float getZ()
		{
			return z;
		}
};

#endif