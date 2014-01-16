#define _USE_MATH_DEFINES
#include <cmath>
#include "Dragonfly.h"
#include <iostream>

void drawTriangles(Triangles* toDraw)
{
	int howMuch = toDraw->getSize();
	for(int i = 0; i < howMuch; ++i)
	{
		Vertex** vertices = (*toDraw)[i]->getVertices();

		glBegin(GL_TRIANGLES);
			glNormal3f((*toDraw)[i]->getNormalX(), (*toDraw)[i]->getNormalY(), (*toDraw)[i]->getNormalZ());
			glVertex3f(vertices[0]->getX(), vertices[0]->getY(), vertices[0]->getZ());
			glVertex3f(vertices[1]->getX(), vertices[1]->getY(), vertices[1]->getZ());
			glVertex3f(vertices[2]->getX(), vertices[2]->getY(), vertices[2]->getZ());
		glEnd();
	} 
}

Triangles* Dragonfly::lathe(float(Dragonfly::*generationFunction)(const float), float begin, float end, int verticalSegments, int horizontalSegments, float radius)
{
	// krok pionowo
	float vertStep = abs(end - begin) / verticalSegments;
	// krok poziomo w radianach
	float horizStep = 360 / horizontalSegments * M_PI / 180;
	
	Triangles* toReturn = new Triangles(verticalSegments * horizontalSegments * 2);

	float upperY;
	float lowerY;

	float currentX = begin;
	for(int verSegIter = 0; verSegIter < verticalSegments; ++verSegIter)
	{
		upperY = (this->*generationFunction)(currentX);

		if((currentX + vertStep) >= end)
		{
			lowerY = (this->*generationFunction)(end);
		}
		else
		{
			lowerY = (this->*generationFunction)(currentX + vertStep);
		}

		for(int angleStep = 0; angleStep < horizontalSegments; ++angleStep)
		{
			// angle to kat obrotu danych wierzcholkow od poczatku obrotu
			float angle = angleStep * horizStep;
			// upperRadius to promien bryly na wyzszym poziomie
			float upperRadius = radius + upperY;
			// lowerRadius to promien bryly na nizszym poziomie
			float lowerRadius = radius + lowerY;

			float v1[3];
			v1[0] = upperRadius * cos(angle);
			v1[1] = currentX;
			v1[2] = upperRadius * sin(angle);

			float v2[3];
			v2[0] = lowerRadius * cos(angle);
			v2[1] = currentX+vertStep;
			v2[2] = lowerRadius * sin(angle);
			
			angle = (angleStep+1) * horizStep;
			float v3[3];
			v3[0] = upperRadius * cos(angle);
			v3[1] = currentX;
			v3[2] = upperRadius * sin(angle);

			float b[3];
			b[0] = v1[0] - v2[0];
			b[1] = v1[1] - v2[1];
			b[2] = v1[2] - v2[2];

			float a[3];
			a[0] = v3[0] - v1[0];
			a[1] = v3[1] - v1[1];
			a[2] = v3[2] - v1[2];

			float xn = a[1] * b[2] - a[2] * b[1];
			float yn = a[2] * b[0] - a[0] * b[2];
			float zn = a[0] * b[1] - a[1] * b[0];
						
			Vertex* vertex1 = new Vertex(v1[0], v1[1], v1[2], 0, 0, 0);
			Vertex* vertex2 = new Vertex(v2[0], v2[1], v2[2], 0, 0, 0);
			Vertex* vertex3 = new Vertex(v3[0], v3[1], v3[2], 0, 0, 0);

			Triangle* triangle = new Triangle(vertex1, vertex2, vertex3, xn, yn, zn);
			toReturn->addTriangle(triangle);

			v1[0] = upperRadius * cos(angle);
			v1[1] = currentX;
			v1[2] = upperRadius * sin(angle);

			v2[0] = lowerRadius * cos(angle);
			v2[1] = currentX+vertStep;
			v2[2] = lowerRadius * sin(angle);
			
			angle = angleStep * horizStep;
			v3[0] = lowerRadius * cos(angle);
			v3[1] = currentX+vertStep;
			v3[2] = lowerRadius * sin(angle);

			b[0] = v2[0] - v1[0];
			b[1] = v2[1] - v1[1];
			b[2] = v2[2] - v1[2];

			a[0] = v3[0] - v1[0];
			a[1] = v3[1] - v1[1];
			a[2] = v3[2] - v1[2];

			xn = a[1] * b[2] - a[2] * b[1];
			yn = a[2] * b[0] - a[0] * b[2];
			zn = a[0] * b[1] - a[1] * b[0];

			vertex1 = new Vertex(v1[0], v1[1], v1[2], 0, 0, 0);
			vertex2 = new Vertex(v2[0], v2[1], v2[2], 0, 0, 0);
			vertex3 = new Vertex(v3[0], v3[1], v3[2], 0, 0, 0);

			triangle = new Triangle(vertex1, vertex2, vertex3, xn, yn, zn);
			toReturn->addTriangle(triangle);
		}

		currentX += vertStep;
	}

	return toReturn;
}

float Dragonfly::edge(const float x)
{
	return -x*x*0.3;
}

float Dragonfly::tail(const float x)
{
	return 2+x/(x+0.5)*(-0.2*x);
}

float Dragonfly::body(const float x)
{
	return -(x*x*x*x*x*x)+1.5;
}

float Dragonfly::beak(const float x)
{
	return 5+x/(x+0.5)*(-6*x);
}

float Dragonfly::eye(const float x)
{
	return cos(x);
}

Triangles* Dragonfly::circle(const float radius, const int segments)
{
	float step = 360 / segments;
	Triangles* toReturn = new Triangles(segments);

	for(int angleStep = 0; angleStep < segments; ++angleStep)
	{
		// angle to kat obrotu danych wierzcholkow od poczatku obrotu
		float angle = angleStep * step * M_PI / 180;
		
		float v1[3];
		v1[0] = 0;
		v1[1] = 0;
		v1[2] = 0;

		float v2[3];
		v2[0] = 0;
		v2[1] = radius * sin(angleStep * step * M_PI / 180);
		v2[2] = radius * cos(angleStep * step * M_PI / 180);
			
		float v3[3];
		v3[0] = 0;
		v3[1] = radius * sin((angleStep+1) * step * M_PI / 180);
		v3[2] = radius * cos((angleStep+1) * step * M_PI / 180);

		float b[3];
		b[0] = v1[0] - v2[0];
		b[1] = v1[1] - v2[1];
		b[2] = v1[2] - v2[2];

		float a[3];
		a[0] = v3[0] - v1[0];
		a[1] = v3[1] - v1[1];
		a[2] = v3[2] - v1[2];

		float xn = a[1] * b[2] - a[2] * b[1];
		float yn = a[2] * b[0] - a[0] * b[2];
		float zn = a[0] * b[1] - a[1] * b[0];

		Vertex* vertex1 = new Vertex(v1[0], v1[1], v1[2], 0, 0, 0);
		Vertex* vertex2 = new Vertex(v2[0], v2[1], v2[2], 0, 0, 0);
		Vertex* vertex3 = new Vertex(v3[0], v3[1], v3[2], 0, 0, 0);

		Triangle* triangle = new Triangle(vertex1, vertex2, vertex3, xn, yn, zn);
		toReturn->addTriangle(triangle);
	}

	return toReturn;
}

Dragonfly::Dragonfly()
{
	this->coordinates = new Coordinates(0, 0, 0);

	// korpus
	bodyTriangles = lathe(&Dragonfly::body, -1.06, 1.06, 20, 36, 0.5);

	//skrzydla
	wingsTriangles[0] = circle(2, 24);
	wingsTriangles[1] = circle(2, 24);
	wingsTriangles[2] = circle(2, 24);
	wingsTriangles[3] = circle(2, 24);

	// ogon
	tailTriangles[0] = lathe(&Dragonfly::tail, -0.4, 2.5, 50, 36, 0);
	tailTriangles[1] = lathe(&Dragonfly::tail, -0.4, 2.1, 50, 36, 0);
	tailTriangles[2] = lathe(&Dragonfly::tail, -0.4, 2.1, 50, 36, 0);
	tailTriangles[3] = lathe(&Dragonfly::tail, -0.4, 2.1, 50, 36, 0);
	tailTriangles[4] = lathe(&Dragonfly::tail, -0.4772, 2, 50, 36, 0);
}

Dragonfly::~Dragonfly()
{
	delete this->coordinates;
	
	delete bodyTriangles;

	delete wingsTriangles[3];
	delete wingsTriangles[2];
	delete wingsTriangles[1];
	delete wingsTriangles[0];
		
	delete tailTriangles[4];
	delete tailTriangles[3];
	delete tailTriangles[2];
	delete tailTriangles[1];
	delete tailTriangles[0];
}

void Dragonfly::draw(const int elapsedTimeMilis)
{
	glRotatef(-90, 0, 0, 1);
		glPushMatrix();		
			glColor3f(0.02, 0.25, 0.42);
			glTranslatef(0, 2.25, 0);
			glPushMatrix();		
				glTranslatef(0.1, -0.1, 0);
				glRotatef(90, 0, 0, 1);
				glScalef(0.6, 1, 0.6);
				glScalef(0.3, 0.3, 0.3);
				glutSolidSphere(1, 24, 24);
			glPopMatrix();
		
			glColor3f(0.74, 0.22, 0.19);
			glPushMatrix();		
				glTranslatef(0, 0, -0.15);
				glRotatef(15, 0, 1, 0);
				glRotatef(80, 0, 0, 1);
				glScalef(0.6, 1, 0.6);
				glScalef(0.3, 0.3, 0.3);
				glutSolidSphere(1, 24, 24);
			glPopMatrix();

			glPushMatrix();		
				glTranslatef(0, 0, 0.15);
				glRotatef(-15, 0, 1, 0);
				glRotatef(80, 0, 0, 1);
				glScalef(0.6, 1, 0.6);
				glScalef(0.3, 0.3, 0.3);
				glutSolidSphere(1, 24, 24);
			glPopMatrix();
		glPopMatrix();

		// korpus
		glColor3f(0.06, 0.52, 0.9);
		
		glPushMatrix();
			glScalef(0.2, 0.7, 0.2);
			glTranslatef(0, 1.85, 0);
			drawTriangles(bodyTriangles);
		glPopMatrix();

		//skrzydla
		glColor4f(0, 0, 0, 0.5);
		glPushMatrix();
			glTranslatef(-0.36, 1.5, 0.17);
			glRotatef(-10, 1, 0, 0);
			glScalef(1, 0.15, 1);
			//glRotatef(frame_no % 40 - 20, 0, 1, 0);
			glTranslatef(0, 0, 2);			
			drawTriangles(wingsTriangles[0]);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.36, 1.2, 0.17);
			glRotatef(10, 1, 0, 0);
			glScalef(1, 0.15, 1);
			//glRotatef(frame_no % 40 - 20, 0, 1, 0);
			glTranslatef(0, 0, 2);
			drawTriangles(wingsTriangles[1]);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.36, 1.5, -0.17);
			glRotatef(10, 1, 0, 0);
			glScalef(1, 0.15, 1);
			//glRotatef(-(frame_no % 40 - 20), 0, 1, 0);
			glTranslatef(0, 0, -2);
			drawTriangles(wingsTriangles[2]);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.36, 1.2, -0.17);
			glRotatef(-10, 1, 0, 0);
			glScalef(1, 0.15, 1);
			//glRotatef(-(frame_no % 40 - 20), 0, 1, 0);
			glTranslatef(0, 0, -2);
			drawTriangles(wingsTriangles[3]);
		glPopMatrix();
		// ogon
		glPushMatrix();
		glColor3f(0.02, 0.25, 0.42);
			glPushMatrix();
				glRotatef(-5, 0, 0, 1);
				glScalef(0.1, 0.4, 0.1);
				glScalef(1, 0.6, 1);
				glTranslatef(-0.45, 0.12, 0);
				drawTriangles(tailTriangles[0]);
			glPopMatrix();

			glPushMatrix();
				glRotatef(-6, 0, 0, 1);
				glScalef(0.1, 0.4, 0.1);
				glScalef(1, 0.8, 1);
				glTranslatef(-0.4, -2.2, 0);
				drawTriangles(tailTriangles[1]);
			glPopMatrix();
		
			glPushMatrix();
				glRotatef(-7, 0, 0, 1);
				glScalef(0.1, 0.4, 0.1);
				glTranslatef(-0.28, -4.04, 0);
				drawTriangles(tailTriangles[2]);
			glPopMatrix();
		
			glPushMatrix();
				glRotatef(-9, 0, 0, 1);
				glScalef(0.1, 0.4, 0.1);
				glScalef(1, 1.2, 1);
				glTranslatef(0.32, -5.75, 0);
				drawTriangles(tailTriangles[3]);
			glPopMatrix();

			glPushMatrix();
				glRotatef(-10, 0, 0, 1);
				glScalef(0.1, 0.4, 0.1);
				glScalef(1, 1.3, 1);
				glTranslatef(0.8, -7.7, 0);
				drawTriangles(tailTriangles[4]);
			glPopMatrix();
		glPopMatrix();
}

void Dragonfly::setCoordinates(const Coordinates& coordinates)
{
	delete this->coordinates;

	this->coordinates = &coordinates;
}

const Coordinates& Dragonfly::getCoordinates() const
{
	return *(this->coordinates);
}