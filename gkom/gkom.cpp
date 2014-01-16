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

#include "Skybox.h"
#include "Dragonfly.h"

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
//GLuint skybox[6];

GLfloat mat_ambient[]    = { 0.2, 0.2,  0.2, 1.0 };
GLfloat mat_specular[]   = { 1.0, 1.0,  1.0, 1.0 };
//GLfloat light_position[] = { 0.0, 10.0, 10.0, 1.0 };
GLfloat lm_ambient[]     = { l, l, l, 1 };

_skybox::Skybox* skybox;
Dragonfly* dragonfly;

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

}

void mesh(int width, int height);

void displayObjects(int frame_no)
{		
    glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
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

	// rysowanie skyboxa
	skybox->draw();
 
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
		dragonfly->draw();
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

   skybox = new _skybox::Skybox();
   dragonfly = new Dragonfly();
   init();
   
   oldTime = glutGet(GLUT_ELAPSED_TIME);
   glutMainLoop();

   return 0;
}
