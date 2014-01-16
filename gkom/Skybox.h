#ifndef _SKYBOX
#define _SKYBOX

#include <Windows.h>
#include <GL/gl.h>
#include <string>


namespace Skybox 
{
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
			const bool loadTexture(const TexPosition texPosition, const std::string src);

			/**
			 * Rysuje skybox
			 */
			void draw();
	};

	/**
	 * Pozycja tekstury w skyboxie
	 */
	enum TexPosition
	{
		TX_FRONT,
		TX_BACK,
		TX_LEFT,
		TX_RIGHT,
		TX_TOP,
		TX_BOTTOM
	};
}
#endif
