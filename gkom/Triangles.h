#ifndef _TRIANGLES
#define _TRIANGLES

#include "Triangle.h"
#include <iostream>

class Triangles
{
	private:
		Triangle** triangles;
		int _size;

		int firstToFill;
	public:
		Triangles(int size) : _size(size), firstToFill(0)
		{
			triangles = new Triangle*[_size];
		}

		~Triangles()
		{
			for(int i = 0; i < _size; ++i)
			{
				delete triangles[i];
			}

			delete[] triangles;
		}

		void addTriangle(Triangle* triangle)
		{
			triangles[firstToFill] = triangle;

			++firstToFill;
		}

		int getSize() const
		{
			return _size;
		}

		Triangle* operator[](size_t i)
		{
			return triangles[i];
		}
};

#endif