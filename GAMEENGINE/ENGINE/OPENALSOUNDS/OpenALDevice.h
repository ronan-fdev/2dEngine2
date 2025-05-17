#pragma once

#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>

#include "../LOGGER/log.h"

class OpenALDevice
{
public:
	OpenALDevice();
	~OpenALDevice();

	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;
};

