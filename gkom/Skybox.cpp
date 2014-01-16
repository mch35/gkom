#include "Skybox.h"
#include "SOIL.h"

using namespace skybox;

const bool Skybox::loadTexture(const TexPosition texPosition, const std::string src)
{
	textures[texPosition] = SOIL_load_OGL_texture(src.c_str(), 
                               SOIL_LOAD_AUTO, 
                               SOIL_CREATE_NEW_ID,
                               SOIL_FLAG_MIPMAPS |
							   SOIL_FLAG_INVERT_Y |
                               SOIL_FLAG_COMPRESS_TO_DXT);

	if( textures[texPosition] == 0 )
	{
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
		return false;
	}

	return true;
}

void draw()
{

}