/*
 * GKOM Lab. 3: OpenGL
 *
 * Program stanowi zaadaptowana wersje przykladu accnot.c.
 */

#include <windows.h>
#include <GL/gl.h>
#include "glut.h"
#include "SOIL.h"
#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>

GLdouble eyex = 0;
GLdouble eyey = 0;
GLdouble eyez = 0;

GLdouble dx = 0;
GLdouble dy = 0;
GLdouble dz = 0;

GLdouble positionx = eyex;
GLdouble positiony = eyey;
GLdouble positionz = eyez;

// wspó³rzêdne punktu w którego kierunku jest zwrócony obserwator,

GLdouble centerx = eyex;
GLdouble centery = eyey;
GLdouble centerz = eyez-1;

int oldX = 0;
int oldY = 0;

int oldTime = 0;

float angleXZ = -90;
float angleYZ = 90;

float l = 0;

float sensitivity = -0.5f;
GLuint skybox[6];

GLfloat mat_ambient[]    = { 0.2, 0.2,  0.2, 1.0 };
    GLfloat mat_specular[]   = { 1.0, 1.0,  1.0, 1.0 };
    //GLfloat light_position[] = { 0.0, 10.0, 10.0, 1.0 };
    GLfloat lm_ambient[]     = { l, l, l, 1 };

void init()
{

    

    //glLightfv( GL_LIGHT0, GL_POSITION, light_position );
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, lm_ambient );
    
	//cianiowanie plaskie
    //glShadeModel( GL_FLAT );

    glEnable( GL_LIGHTING );
    glDisable( GL_LIGHT0 );

	glDisable( GL_LIGHT1 );
	glDisable( GL_LIGHT2 );
	glDisable( GL_LIGHT3 );
	glDisable( GL_LIGHT4 );
	glDisable( GL_LIGHT5 );
	glDisable( GL_LIGHT6 );
	glDisable( GL_LIGHT7 );

	// w³¹czenie obs³ugi w³aœciwoœci materia³ów
    glEnable( GL_COLOR_MATERIAL );
   
    // w³aœciwoœci materia³u okreœlone przez kolor wierzcho³ków
    glColorMaterial( GL_FRONT, GL_AMBIENT );

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDepthFunc( GL_LESS );
    glEnable( GL_DEPTH_TEST );

	// normalizacja wektorow zeby np swiatlo sie nie psulo przy skalowaniu
	glEnable(GL_NORMALIZE);

	glCullFace(GL_FRONT);

	//cieniowanie interpolowane
	glShadeModel(GL_SMOOTH);
	
	skybox[0] = SOIL_load_OGL_texture("img/right.jpg", 
                               SOIL_LOAD_AUTO, 
                               SOIL_CREATE_NEW_ID,
                               SOIL_FLAG_MIPMAPS |
							   SOIL_FLAG_INVERT_Y |
                               SOIL_FLAG_COMPRESS_TO_DXT);

	skybox[1] = SOIL_load_OGL_texture("img/left.jpg", 
                               SOIL_LOAD_AUTO, 
                               SOIL_CREATE_NEW_ID,
                               SOIL_FLAG_MIPMAPS |
							   SOIL_FLAG_INVERT_Y |
                               SOIL_FLAG_COMPRESS_TO_DXT);

	skybox[2] = SOIL_load_OGL_texture("img/top.jpg", 
                               SOIL_LOAD_AUTO, 
                               SOIL_CREATE_NEW_ID,
                               SOIL_FLAG_MIPMAPS |
							   SOIL_FLAG_INVERT_Y |
                               SOIL_FLAG_COMPRESS_TO_DXT);

	skybox[3] = SOIL_load_OGL_texture("img/bottom.jpg", 
                               SOIL_LOAD_AUTO, 
                               SOIL_CREATE_NEW_ID,
                               SOIL_FLAG_MIPMAPS |
							   SOIL_FLAG_INVERT_Y |
                               SOIL_FLAG_COMPRESS_TO_DXT);

	skybox[4] = SOIL_load_OGL_texture("img/back.jpg", 
                               SOIL_LOAD_AUTO, 
                               SOIL_CREATE_NEW_ID,
                               SOIL_FLAG_MIPMAPS |
							   SOIL_FLAG_INVERT_Y |
                               SOIL_FLAG_COMPRESS_TO_DXT);

	skybox[5] = SOIL_load_OGL_texture("img/front.jpg", 
                               SOIL_LOAD_AUTO, 
                               SOIL_CREATE_NEW_ID,
                               SOIL_FLAG_MIPMAPS |
							   SOIL_FLAG_INVERT_Y |
                               SOIL_FLAG_COMPRESS_TO_DXT);

	for(int i = 0; i < 6; i++)
	{
		if( 0 == skybox[i] )
		{
			printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
		}
	}

}

float edge(const float x)
{
	return -x*x*0.3;
}

float tail(const float x)
{
	return 2+x/(x+0.5)*(-0.2*x);
}

float body(const float x)
{
	return -(x*x*x*x*x*x)+1.5;
}

float beak(const float x)
{
	return 5+x/(x+0.5)*(-6*x);
}

float eye(const float x)
{
	return cos(x);
}

typedef float(*foo)(const float);

// TODO nie generuje jednego segmentu pionowo
void lathe(foo fun, float begin, float end, int verticalSegments = 10, int horizontalSegments = 36, float radius = 2)
{
	// krok pionowo
	float vertStep = abs(end - begin) / verticalSegments;
	// krok poziomo w radianach
	float horizStep = 360 / horizontalSegments * M_PI / 180;

	//std::cout << "vert: " << vertStep << " horiz: " << horizStep << std::endl;

	float upperY;
	float lowerY;
	
			//glBegin(GL_TRIANGLE_STRIP);
	glBegin(GL_TRIANGLES);
	for(float currentX = begin; currentX < end; currentX += vertStep)
	{
		upperY = fun(currentX);

		if((currentX + vertStep) > end)
		{
			lowerY = fun(end);
		}
		else
		{
			lowerY = fun(currentX + vertStep);
		}

		//std::cout << "upperY: " << upperY << " lowerY: " << lowerY << std::endl;

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

			glNormal3f(xn, yn, zn);
			glVertex3f(v1[0], v1[1], v1[2]);	// 1
			glVertex3f(v2[0], v2[1], v2[2]);	// 2
			glVertex3f(v3[0], v3[1], v3[2]);	// 3

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

			glNormal3f(xn, yn, zn);
			glVertex3f(v1[0], v1[1], v1[2]);	// 3
			glVertex3f(v2[0], v2[1], v2[2]);	// 4
			glVertex3f(v3[0], v3[1], v3[2]);	// 2
		}

		/** WERSJA NA CZWOROKATACH */
		/*for(int angleStep = 0; angleStep < horizontalSegments; ++angleStep)
		{
			// angle to kat obrotu danych wierzcholkow od poczatku obrotu
			float angle = angleStep * horizStep;
			// upperRadius to promien bryly na wyzszym poziomie
			float upperRadius = radius + upperY;
			// lowerRadius to promien bryly na nizszym poziomie
			float lowerRadius = radius + lowerY;

			glVertex3f(upperRadius * cos(angle), currentX, upperRadius * sin(angle));
			glVertex3f(lowerRadius * cos(angle), currentX+vertStep, lowerRadius * sin(angle));

			angle = (angleStep+1) * horizStep;
			glVertex3f(upperRadius * cos(angle), currentX, upperRadius * sin(angle));
			glVertex3f(lowerRadius * cos(angle), currentX+vertStep, lowerRadius * sin(angle));
		}*/
	}
				glEnd();
}

void circle(const float radius, const int segments)
{
	float step = 360 / segments;
	glBegin(GL_TRIANGLES);
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

		glNormal3f(xn, yn, zn);
		glVertex3f(v1[0], v1[1], v1[2]);
		glVertex3f(v2[0], v2[1], v2[2]);
		glVertex3f(v3[0], v3[1], v3[2]);
	}
	glEnd();
}

void mesh(int width, int height);

void dragonfly(int frame_no)
{
	// wazka

		//glowa
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
			lathe(body, -1.06, 1.06, 20, 36, 0.5);
		glPopMatrix();

		//skrzydla
		glColor4f(0, 0, 0, 0.5);
		glPushMatrix();
			glTranslatef(-0.36, 1.5, 0.17);
			glRotatef(-10, 1, 0, 0);
			glScalef(1, 0.15, 1);
			glRotatef(frame_no % 40 - 20, 0, 1, 0);
			glTranslatef(0, 0, 2);
			circle(2, 24);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.36, 1.2, 0.17);
			glRotatef(10, 1, 0, 0);
			glScalef(1, 0.15, 1);
			glRotatef(frame_no % 40 - 20, 0, 1, 0);
			glTranslatef(0, 0, 2);
			circle(2, 24);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.36, 1.5, -0.17);
			glRotatef(10, 1, 0, 0);
			glScalef(1, 0.15, 1);
			glRotatef(-(frame_no % 40 - 20), 0, 1, 0);
			glTranslatef(0, 0, -2);
			circle(2, 24);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.36, 1.2, -0.17);
			glRotatef(-10, 1, 0, 0);
			glScalef(1, 0.15, 1);
			glRotatef(-(frame_no % 40 - 20), 0, 1, 0);
			glTranslatef(0, 0, -2);
			circle(2, 24);
		glPopMatrix();
		// ogon
		glPushMatrix();
		glColor3f(0.02, 0.25, 0.42);
			glPushMatrix();
				glRotatef(-5, 0, 0, 1);
				glScalef(0.1, 0.4, 0.1);
				glScalef(1, 0.6, 1);
				glTranslatef(-0.45, 0.12, 0);
				lathe(tail, -0.4, 2.5, 50, 36, 0);
			glPopMatrix();

			glPushMatrix();
				glRotatef(-6, 0, 0, 1);
				glScalef(0.1, 0.4, 0.1);
				glScalef(1, 0.8, 1);
				glTranslatef(-0.4, -2.2, 0);
				lathe(tail, -0.4, 2.1, 50, 36, 0);
			glPopMatrix();
		
			glPushMatrix();
				glRotatef(-7, 0, 0, 1);
				glScalef(0.1, 0.4, 0.1);
				glTranslatef(-0.28, -4.04, 0);
				lathe(tail, -0.4, 2.1, 50, 36, 0);
			glPopMatrix();
		
			glPushMatrix();
				glRotatef(-9, 0, 0, 1);
				glScalef(0.1, 0.4, 0.1);
				glScalef(1, 1.2, 1);
				glTranslatef(0.32, -5.75, 0);
				lathe(tail, -0.4, 2.1, 50, 36, 0);
			glPopMatrix();

			glPushMatrix();
				glRotatef(-10, 0, 0, 1);
				glScalef(0.1, 0.4, 0.1);
				glScalef(1, 1.3, 1);
				glTranslatef(0.8, -7.7, 0);
				lathe(tail, -0.4772, 2, 50, 36, 0);
			glPopMatrix();
		glPopMatrix();
}


void displayObjects(int frame_no)
{		
    glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*glBegin(GL_QUADS);

	glColor3f(255, 255, 255);
	glVertex3f(-2, 2, -2);
	glVertex3f(-2, -2, -2);
	glVertex3f(2, -2, -2);
	glVertex3f(2, 2, -2);
	
	glColor3f(255, 0, 0);
	glVertex3f(-2, 2, 2);
	glVertex3f(-2, -2, 2);
	glVertex3f(-2, -2, -2);
	glVertex3f(-2, 2, -2);
	
	glColor3f(0, 255, 0);
	glVertex3f(2, 2, 2);
	glVertex3f(2, -2, 2);
	glVertex3f(2, -2, -2);
	glVertex3f(2, 2, -2);
	
	glColor3f(0, 0, 255);
	glVertex3f(-2, 2, 2);
	glVertex3f(-2, -2, 2);
	glVertex3f(2, -2, 2);
	glVertex3f(2, 2, 2);
	glEnd();	

	glPopMatrix();*/
	
	glPushMatrix();
		glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(-10, 0, 10);
		glutSolidSphere(1, 24, 24);
		glPopMatrix();
		
		glPushMatrix();
		glColor3f(0, 1, 0);
		glTranslatef(-10, 0, -10);
		glutSolidSphere(1, 24, 24);
		glPopMatrix();
		
		glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(10, 0, 10);
		glutSolidSphere(1, 24, 24);
		glPopMatrix();
		
		glPushMatrix();
		glColor3f(0, 0, 0);
		glTranslatef(10, 0, -10);
		glutSolidSphere(1, 24, 24);
		glPopMatrix();

		
	glPopMatrix();
	
	glColor3f(1, 1, 1);
	glPushMatrix();
    glDisable(GL_LIGHTING);
	mesh(10, 10);
	glPopMatrix();
}

void mesh(int width, int height)
{
	glLineWidth(2);

	glBegin(GL_LINES);
	
	for(int i = 0; i <= width; ++i)
	{
		for(int j = 0; j < height; ++j)
		{
			glVertex3f(i, 0, j);
			glVertex3f(i, 0, j+1);
		}
	}

	for(int i = 0; i <= height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			glVertex3f(j, 0, i);
			glVertex3f(j+1, 0, i);
		}
	}

	glEnd();
}

int frames = 0;
int timeElapsed = 0;
int fps = 0;

void display()
{
	// czyscimy bufory koloru i glebokosci
	
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int elapsedTime = currentTime - oldTime;

	timeElapsed += elapsedTime;
	if(timeElapsed >= 1000)
	{
		timeElapsed -= 1000;
				
		printf("%d\n", frames);
		frames = 0;
	}


    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glClearColor(0, 0, 0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	static int frame = 0;

	frame == 359 ? frame = 0 : ++frame;

	//gluLookAt( positionx, positiony, positionz, positionx+centerx, positiony+centery, positionz+centerz, 0, 1, 0 );
			
	glPushMatrix();
 gluLookAt(
        0, 0, 0,
        cos(angleXZ * M_PI / 180), cos(angleYZ * M_PI / 180), sin(angleXZ * M_PI / 180), 0, 1, 0 );
    // Enable/Disable features


	
    glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

	float lm_ambient[] = { l, l, l, 1 };

	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, lm_ambient );
    glDisable(GL_BLEND);
 
    // Just in case we set all vertices to white.
    glColor3f(0.5,0.5,0.5);
 
    // Render the front quad
    glBindTexture(GL_TEXTURE_2D, skybox[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
        glTexCoord2f(1, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
        glTexCoord2f(1, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
        glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
    glEnd();
 
    // Render the left quad
    glBindTexture(GL_TEXTURE_2D, skybox[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  0.5f, -0.5f,  0.5f );
        glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
        glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
        glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f,  0.5f );
    glEnd();
 
    // Render the back quad
    glBindTexture(GL_TEXTURE_2D, skybox[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f,  0.5f );
        glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f,  0.5f );
        glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f,  0.5f );
        glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f,  0.5f );
 
    glEnd();
 
    // Render the right quad
    glBindTexture(GL_TEXTURE_2D, skybox[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
        glTexCoord2f(1, 0); glVertex3f( -0.5f, -0.5f,  0.5f );
        glTexCoord2f(1, 1); glVertex3f( -0.5f,  0.5f,  0.5f );
        glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
    glEnd();
 
    // Render the top quad
    glBindTexture(GL_TEXTURE_2D, skybox[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glBegin(GL_QUADS);
        glTexCoord2f(1, 0); glVertex3f( -0.5f,  0.5f, -0.5f );
        glTexCoord2f(1, 1); glVertex3f( -0.5f,  0.5f,  0.5f );
        glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f,  0.5f );
        glTexCoord2f(0, 0); glVertex3f(  0.5f,  0.5f, -0.5f );
    glEnd();
 
    // Render the bottom quad
    glBindTexture(GL_TEXTURE_2D, skybox[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
        glTexCoord2f(0, 1); glVertex3f( -0.5f, -0.5f,  0.5f );
        glTexCoord2f(1, 1); glVertex3f(  0.5f, -0.5f,  0.5f );
        glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
    glEnd();
 
    // Restore enable bits and matrix
    glPopAttrib();
    glPopMatrix();
	
	glEnable( GL_LIGHTING );
   
    // parametry globalnego œwiat³a otaczaj¹cego
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, lm_ambient );
   
    glEnable( GL_COLOR_MATERIAL );
    glColorMaterial( GL_FRONT, GL_AMBIENT );
   
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	float speed = 0.001;
	//dx+=speed*elapsedTime;
	//dy+=speed*elapsedTime;
	//dz-=speed*elapsedTime;

		glTranslatef(dx, dy, dz);
		//glRotatef(-angleYZ+90, 0, 0, 1);
		//glRotatef(-angleXZ, 0, 1, 0);
		glRotatef(90, 0, 1, 0);
		dragonfly(frame);
		glPopMatrix();
	gluLookAt( positionx, positiony, positionz, positionx+centerx, positiony+centery, positionz+centerz, 0, 1, 0 );
	
	//glLightfv( GL_LIGHT0, GL_POSITION, light_position );
	glPushMatrix();
    displayObjects(frame);
	glPopMatrix();

	//wyczyszczenie buforow
	glFlush();
	glutSwapBuffers();

	oldTime = currentTime;
	frames++;
}

void reshape(GLsizei w, GLsizei h)
{
    if( h > 0 && w > 0 ) {
		// to bedzie dostepne dla openGLa
      glViewport( 0, 0, w, h );
      glMatrixMode( GL_PROJECTION );
      glLoadIdentity();
      
	  GLdouble aspect = 1;
    if( h > 0 )
         aspect = w /( GLdouble ) h;
   
    // rzutowanie perspektywiczne
    gluPerspective( 90, aspect, 0.1, 50.0 );

	  //cofamy obserwatora do tylu
	  // z-bufor ma 32 bity - ograniczona rozdzielczosc. odleglosc miedzy scianami bedzie za duza, to potem moga dwa obiekty na tym msamym miejscu 
    }
}

void SpecialKeys( int key, int x, int y )
{
	float step = 0.1;
    switch( key )
    {
        // kursor w lewo
    case GLUT_KEY_LEFT:
        eyez -= step*cos(angleXZ * M_PI / 180);
		eyex += step*sin(angleXZ * M_PI / 180);
        break;
       
        // kursor w górê
    case GLUT_KEY_UP:
        eyex += step*cos(angleXZ * M_PI / 180);
        eyey += step*cos(angleYZ * M_PI / 180);
		eyez += step*sin(angleXZ * M_PI / 180);
        break;
       
        // kursor w prawo
    case GLUT_KEY_RIGHT:        
        eyez += step*cos(angleXZ * M_PI / 180);
		eyex -= step*sin(angleXZ * M_PI / 180);
        break;
       
        // kursor w dó³
    case GLUT_KEY_DOWN:
        eyex -= step*cos(angleXZ * M_PI / 180);
        eyey -= step*cos(angleYZ * M_PI / 180);
		eyez -= step*sin(angleXZ * M_PI / 180);
        break;
    }

	positionx = eyex;
	positiony = eyey;
	positionz = eyez;
	   
    // odrysowanie okna
    reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
}

void Keyboard( unsigned char key, int x, int y )
{
	float step = 0.1;
    switch(key)
	{
		case 'a':
			dz -= step*cos(angleXZ * M_PI / 180);
			dx += step*sin(angleXZ * M_PI / 180);
			break;
       
        // kursor w górê
		case 'w':
			dx += step*cos(angleXZ * M_PI / 180);
			dy += step*cos(angleYZ * M_PI / 180);
			dz += step*sin(angleXZ * M_PI / 180);
			break;
       
        // kursor w prawo
		case 'd':        
			dz += step*cos(angleXZ * M_PI / 180);
			dx -= step*sin(angleXZ * M_PI / 180);
			break;
       
        // kursor w dó³
		case 's':
			dx -= step*cos(angleXZ * M_PI / 180);
			dy -= step*cos(angleYZ * M_PI / 180);
			dz -= step*sin(angleXZ * M_PI / 180);
			break;

		case '+':        
			l += 0.1;
			break;
       
        // kursor w dó³
		case '-':
			l -= 0.1;
			break;
    }
   
    // odrysowanie okna
    reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
}

void mouseMotion( int x, int y )
{
	float dX = oldX - x;
	float dY = oldY - y;

	angleXZ += sensitivity*dX;
	angleYZ += sensitivity*dY;

	if(angleXZ < 0) angleXZ += 360;
	else if(angleXZ >= 360) angleXZ -= 360;

	if(angleYZ < 0) angleYZ = 0;
	else if(angleYZ >= 180) angleYZ = 180;
		
	centerx = cos(angleXZ * M_PI / 180);
	centery = cos(angleYZ * M_PI / 180);
	centerz = sin(angleXZ * M_PI / 180);
	
	oldX = x;
	oldY = y;
}

void firstMotion( int x, int y )
{
    oldX = x;
	oldY = y;

	glutPassiveMotionFunc(mouseMotion);
}


int main(int argc, char** argv)
{
	// inicjuje glut i jej wspolprace z sys okienkowym
   glutInit( &argc, argv );
   //bufor glebokosci z-bufor
   // poj buforowanie
   // podwojne buforowanie 
   glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

   glutInitWindowPosition( 100, 100 );
   glutInitWindowSize( 1200, 1024 );

   glutCreateWindow( "GPOB: OpenGL" );

   glutDisplayFunc( display );
   glutReshapeFunc( reshape );

   glutSpecialFunc( SpecialKeys );

   glutKeyboardFunc( Keyboard );

   glutPassiveMotionFunc(firstMotion);

   glutIdleFunc(display);

   init();
   
   oldTime = glutGet(GLUT_ELAPSED_TIME);
   glutMainLoop();

   return 0;
}
