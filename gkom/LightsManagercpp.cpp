#include "LightsManager.h"

LightsManager::LightsManager(float* globalAmbient)
{
	for(int i = 0; i < 8; ++i)
	{
		lights[i] = 0;
	}
	
	if(globalAmbient != 0)
	{
		this->globalAmbient[0] = globalAmbient[0];
		this->globalAmbient[1] = globalAmbient[1];
		this->globalAmbient[2] = globalAmbient[2];
		this->globalAmbient[3] = globalAmbient[3];
	}
	else
	{
		this->globalAmbient[0] = 0.2f;
		this->globalAmbient[1] = 0.2f;
		this->globalAmbient[2] = 0.2f;
		this->globalAmbient[3] = 1.0f;
	}
}

LightsManager::~LightsManager()
{
	for(int i = 0; i < 8; ++i)
	{
		delete lights[i];
	}
}

void LightsManager::drawLights()
{
	for(int i = 0; i < 8; ++i)
	{
		if(lights[i] != 0)
		{
			lights[i]->draw();
		}
	}
}

void LightsManager::drawGlobalAmbient()
{
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, this->globalAmbient );
}

void LightsManager::addLight(int id, float* position)
{
	if((id - 0x4000 >= 0) && (id - 0x4000 < 8))
	{
		lights[id - 0x4000] = new LightSource(id, position);
	}
}

void LightsManager::setGlobalAmbient(float* globalAmbient)
{
	if(globalAmbient != 0)
	{
		this->globalAmbient[0] = globalAmbient[0];
		this->globalAmbient[1] = globalAmbient[1];
		this->globalAmbient[2] = globalAmbient[2];
		this->globalAmbient[3] = globalAmbient[3];
	}
}

void LightsManager::setGlobalAmbientBrighter()
{
	this->globalAmbient[0] += 0.1f;
	this->globalAmbient[1] += 0.1f;
	this->globalAmbient[2] += 0.1f;
}

void LightsManager::setGlobalAmbientDarker()
{
	this->globalAmbient[0] -= 0.1f;
	this->globalAmbient[1] -= 0.1f;
	this->globalAmbient[2] -= 0.1f;
}

const float* LightsManager::getGlobalAmbient()
{
	return globalAmbient;
}

void LightsManager::enableLighting()
{
	glEnable( GL_LIGHTING );
}

void LightsManager::disableLighting()
{
	glDisable( GL_LIGHTING );
}

void LightsManager::enableAllLights()
{
	for(int i = 0; i < 8; ++i)
	{
		if(lights[i] != 0)
		{
			lights[i]->enable();
		}
	}
}

void LightsManager::disableAllLights()
{
	for(int i = 0; i < 8; ++i)
	{
		if(lights[i] != 0)
		{
			lights[i]->disable();
		}
	}
}

LightSource* LightsManager::getLight(int id)
{
	if((id - 0x4000 >= 0) && (id - 0x4000 < 8))
	{
		if(lights[id - 0x4000] != 0)
		{
			return lights[id - 0x4000];
		}
	}

	return 0;
}