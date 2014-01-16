#ifndef _VERTEX
#define _VERTEX

class Vertex
{
	private:
		const float x;
		const float y;
		const float z;

		const float normalX;
		const float normalY;
		const float normalZ;

		const float colorR;
		const float colorG;
		const float colorB;

		const float textureX;
		const float textureY;

	public:
		Vertex(const float x, const float y, const float z,
			   const float nx, const float ny, const float nz,
			   const float cr = 0, const float cg = 0, const float cb = 0,
			   const float tx = 0, const float ty = 0) : x(x), y(y), z(z),
												 normalX(nx), normalY(ny), normalZ(nz),
												 colorR(cr), colorG(cg), colorB(cb),
												 textureX(tx), textureY(ty)
		{

		}

		~Vertex()
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

		const float getNormalX() const
		{
			return normalX;
		}
		
		const float getNormalY() const
		{
			return normalY;
		}

		const float getNormalZ() const
		{
			return normalZ;
		}

		const float getColorR() const
		{
			return colorR;
		}
		
		const float getColorG() const
		{
			return colorG;
		}

		const float getColorB() const
		{
			return colorB;
		}

		const float getTextureX() const
		{
			return textureX;
		}

		const float getTextureY() const
		{
			return textureY;
		}
};

#endif