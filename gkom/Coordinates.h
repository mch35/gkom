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

		const float getX() const
		{
			return x;
		}

		const float getY() const
		{
			return y;
		}

		const float getZ() const
		{
			return z;
		}
};

#endif