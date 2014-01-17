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
#include "Camera.h"
#include "LightsManager.h"

GLdouble dx = 0;
GLdouble dy = 0;
GLdouble dz = 0;

int oldX = 0;
int oldY = 0;

int oldTime = 0;
/*
float l = 0;

float l0 = 0;

GLfloat mat_ambient[]    = { 0.2, 0.2,  0.2, 1.0 };
GLfloat mat_specular[]   = { 1.0, 1.0,  1.0, 1.0 };
GLfloat light_position[] = { -10.0, 10.0, 10.0, 1.0 };
GLfloat lm_ambient[]     = { l, l, l, 1 };

GLfloat lm_spec[]     = { l0, l0, l0, 10 };*/

Skybox* skybox;
Dragonfly* dragonfly;
Camera* camera;
LightsManager* lightsManager;

void init()
{
	skybox = new Skybox();
	dragonfly = new Dragonfly();
	camera = new Camera();
	lightsManager = new LightsManager(LightsManager::GLOBAL_AMBIENT_DEFAULT);

	float lightPosition[] = {-10, 10, 10};
	lightsManager->addLight(GL_LIGHT0, lightPosition);

	// w��czenie obs�ugi w�a�ciwo�ci materia��w
    glEnable( GL_COLOR_MATERIAL );
   
    // w�a�ciwo�ci materia�u okre�lone przez kolor wierzcho�k�w
    glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );

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
	lightsManager->enableLighting();

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

	glPushMatrix();
		glTranslatef(-10, 10, 10);
		glColor3f(1, 1, 1);
		glutSolidSphere(0.2, 6, 6);
	glPopMatrix();
	
	glColor3f(1, 1, 1);
	glPushMatrix();

	lightsManager->disableLighting();
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
	// czas od ostatniego renderowania	
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
	
	
	lightsManager->enableLighting();
	lightsManager->drawGlobalAmbient();

	glPushMatrix();
		gluLookAt(0, 0, 0, cos(camera->getAngleXZ() * M_PI / 180), cos(camera->getAngleYZ() * M_PI / 180), sin(camera->getAngleXZ() * M_PI / 180), 0, 1, 0 );
    	
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);    
		glDisable(GL_BLEND);
 		
		// rysowanie skyboxa
		skybox->draw();
 
		glPopAttrib();
    glPopMatrix();
	      
    glEnable( GL_COLOR_MATERIAL );
    glColorMaterial( GL_FRONT, GL_AMBIENT );
   
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	gluLookAt(camera->getPosition().getX(), camera->getPosition().getY(), camera->getPosition().getZ(),
			  camera->getPosition().getX()+camera->getDirection().getX(), camera->getPosition().getY()+camera->getDirection().getY(), camera->getPosition().getZ()+camera->getDirection().getZ(), 0, 1, 0 );
	
	lightsManager->enableAllLights();
	lightsManager->drawLights();

	glPushMatrix();		

	float speed = 0.001;
	//dx+=speed*elapsedTime;
	//dy+=speed*elapsedTime;
	//dz-=speed*elapsedTime;

		glTranslatef(dx, dy, dz);
		//glRotatef(-angleYZ+90, 0, 0, 1);
		//glRotatef(-angleXZ, 0, 1, 0);
		glRotatef(90, 0, 1, 0);
		dragonfly->draw(elapsedTime);
	glPopMatrix();
		

	glPushMatrix();
		displayObjects(0);
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
		camera->move(Camera::Direction::CAMERA_LEFT);
        break;
       
        // kursor w g�r�
    case GLUT_KEY_UP:
		camera->move(Camera::Direction::CAMERA_FORWARD);
        break;
       
        // kursor w prawo
    case GLUT_KEY_RIGHT:
		camera->move(Camera::Direction::CAMERA_RIGHT);
        break;
       
        // kursor w d�
    case GLUT_KEY_DOWN:
		camera->move(Camera::Direction::CAMERA_BACKWARD);
        break;
    }
	   
    // odrysowanie okna
    reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
}

void Keyboard( unsigned char key, int x, int y )
{
	float step = 0.1;
    switch(key)
	{
		case 'a':
			dz -= step*cos(camera->getAngleXZ() * M_PI / 180);
			dx += step*sin(camera->getAngleXZ() * M_PI / 180);
			break;
       
        // kursor w g�r�
		case 'w':
			dx += step*cos(camera->getAngleXZ() * M_PI / 180);
			dy += step*cos(camera->getAngleYZ() * M_PI / 180);
			dz += step*sin(camera->getAngleXZ() * M_PI / 180);
			break;
       
        // kursor w prawo
		case 'd':        
			dz += step*cos(camera->getAngleXZ() * M_PI / 180);
			dx -= step*sin(camera->getAngleXZ() * M_PI / 180);
			break;
       
        // kursor w d�
		case 's':
			dx -= step*cos(camera->getAngleXZ() * M_PI / 180);
			dy -= step*cos(camera->getAngleYZ() * M_PI / 180);
			dz -= step*sin(camera->getAngleXZ() * M_PI / 180);
			break;

		case '+':
			lightsManager->setGlobalAmbientBrighter();
			break;
       
        // kursor w d�
		case '-':
			lightsManager->setGlobalAmbientDarker();
			break;
    }
   
    // odrysowanie okna
    reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
}

void mouseMotion( int x, int y )
{
	float dX = oldX - x;
	float dY = oldY - y;

	camera->rotate(dX, dY);
	/*angleXZ += sensitivity*dX;
	angleYZ += sensitivity*dY;

	if(angleXZ < 0) angleXZ += 360;
	else if(angleXZ >= 360) angleXZ -= 360;

	if(angleYZ < 0) angleYZ = 0;
	else if(angleYZ >= 180) angleYZ = 180;
		
	centerx = cos(angleXZ * M_PI / 180);
	centery = cos(angleYZ * M_PI / 180);
	centerz = sin(angleXZ * M_PI / 180);*/
	
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
