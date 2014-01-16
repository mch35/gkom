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
		static long vertexCounter;

		Vertex(const float x, const float y, const float z,
			   const float nx, const float ny, const float nz,
			   const float cr = 0, const float cg = 0, const float cb = 0,
			   const float tx = 0, const float ty = 0);

		~Vertex();
		
		const float getX() const;		
		const float getY() const;
		const float getZ() const;
		const float getNormalX() const;		
		const float getNormalY() const;
		const float getNormalZ() const;
		const float getColorR() const;
		const float getColorG() const;
		const float getColorB() const;
		const float getTextureX() const;
		const float getTextureY() const;
};

#endif