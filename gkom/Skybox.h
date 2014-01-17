#ifndef _SKYBOX
#define _SKYBOX

#include <Windows.h>
#include <GL/gl.h>
#include "glut.h"
#include "SOIL.h"


/**
	* Pozycja tekstury w skyboxie
	*/
enum TexPosition
{
	TX_FRONT = 0,
	TX_BACK = 1,
	TX_LEFT = 2,
	TX_RIGHT = 3,
	TX_TOP = 4,
	TX_BOTTOM = 5
};

/**
	* Klasa reprezentujaca skybox
	*
	*/
class Skybox
{
	private:
		GLuint textures[6];
	public:
		Skybox();
		~Skybox();

		/**
			* Laduje teksture na dana pozycje
			*/
		const bool loadTexture(const TexPosition texPosition, const char* src);

		/**
			* Rysuje skybox
			*/
		void draw();
};
#endif
