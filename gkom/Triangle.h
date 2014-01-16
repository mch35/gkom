#ifndef _TRIANGLE
#define _TRIANGLE

#include "Vertex.h"

class Triangle
{
	private:
		Vertex** vertices;

		const float normalX;
		const float normalY;
		const float normalZ;

	public:
		Triangle(Vertex* vertexX, Vertex* vertexY, Vertex* vertexZ,
			   const float nx = 0, const float ny = 0, const float nz = 0) : normalX(nx), normalY(ny), normalZ(nz)
		{
			vertices = new Vertex*[3];
			vertices[0] = vertexX;
			vertices[1] = vertexY;
			vertices[2] = vertexZ;
		}

		~Triangle()
		{
			delete vertices[0];
			delete vertices[1];
			delete vertices[2];

			delete[] vertices;
		}

		Vertex** getVertices() const
		{
			return vertices;
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
};

#endif