#include "Skybox.h"
#include <cstdio>

Skybox::Skybox()
{
	loadTexture(TexPosition::TX_FRONT, "img/front.jpg");
	loadTexture(TexPosition::TX_BACK, "img/back.jpg");
	loadTexture(TexPosition::TX_LEFT, "img/left.jpg");
	loadTexture(TexPosition::TX_RIGHT, "img/right.jpg");
	loadTexture(TexPosition::TX_TOP, "img/top.jpg");
	loadTexture(TexPosition::TX_BOTTOM, "img/bottom.jpg");
}

Skybox::~Skybox()
{
	for(int i = 0; i < 6; ++i)
	{
		delete (int*)textures[i];
	}
}

const bool Skybox::loadTexture(const TexPosition texPosition, const char* src)
{
	textures[texPosition] = SOIL_load_OGL_texture(src, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	
	if( textures[texPosition] == 0 )
	{
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
		return false;
	}

	return true;
}

void Skybox::draw()
{
	// Render the front quad

	if(textures[TexPosition::TX_FRONT] != 0)
	{
		glBindTexture(GL_TEXTURE_2D, textures[TexPosition::TX_FRONT]);
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
	}
 
	if(textures[TexPosition::TX_LEFT] != 0)
	{
		// Render the left quad
		glBindTexture(GL_TEXTURE_2D, textures[TexPosition::TX_LEFT]);
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
	}
 
	if(textures[TexPosition::TX_BACK] != 0)
	{
		// Render the back quad
		glBindTexture(GL_TEXTURE_2D, textures[TexPosition::TX_BACK]);
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
	}
 
	if(textures[TexPosition::TX_RIGHT] != 0)
	{
		// Render the right quad
		glBindTexture(GL_TEXTURE_2D, textures[TexPosition::TX_RIGHT]);
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
	}
 
	if(textures[TexPosition::TX_TOP] != 0)
	{
		// Render the top quad
		glBindTexture(GL_TEXTURE_2D, textures[TexPosition::TX_TOP]);
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
	}
 
	if(textures[TexPosition::TX_BOTTOM] != 0)
	{
		// Render the bottom quad
		glBindTexture(GL_TEXTURE_2D, textures[TexPosition::TX_BOTTOM]);
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
	}
}