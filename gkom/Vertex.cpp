#include "Vertex.h"

Vertex::Vertex(const float x, const float y, const float z,
			   const float nx, const float ny, const float nz,
			   const float cr, const float cg, const float cb,
			   const float tx, const float ty) : x(x), y(y), z(z),
												 normalX(nx), normalY(ny), normalZ(nz),
												 colorR(cr), colorG(cg), colorB(cb),
												 textureX(tx), textureY(ty)
		{
			++vertexCounter;
		}

Vertex::~Vertex()
{
	--vertexCounter;
}
		
const float Vertex::getX() const
{
	return x;
}
		
const float Vertex::getY() const
{
	return y;
}

const float Vertex::getZ() const
{
	return z;
}

const float Vertex::getNormalX() const
{
	return normalX;
}
		
const float Vertex::getNormalY() const
{
	return normalY;
}

const float Vertex::getNormalZ() const
{
	return normalZ;
}

const float Vertex::getColorR() const
{
	return colorR;
}
		
const float Vertex::getColorG() const
{
	return colorG;
}

const float Vertex::getColorB() const
{
	return colorB;
}

const float Vertex::getTextureX() const
{
	return textureX;
}

const float Vertex::getTextureY() const
{
	return textureY;
}

long Vertex::vertexCounter = 0;