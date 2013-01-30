/*
 *  ofxSoundObject.cpp
 *  openFrameworks
 *
 *  Created by Arick on 8/17/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxSoundObject.h"

ofxSoundObject::ofxSoundObject()
{	
	inst = ofxSystemSingleton::Instance().getSystem();

}

void ofxSoundObject::loadSound(string fileLocation)
{
	result = FMOD_System_CreateStream(inst, fileLocation.c_str(), (FMOD_SOFTWARE | FMOD_CREATESTREAM), NULL, &fmodSound);

	ERRCHECK(result, fileLocation);
	
	result = FMOD_Sound_SetMode(fmodSound, FMOD_LOOP_OFF);
	
	ERRCHECK(result);
	
}

void ofxSoundObject::play(bool loopSound )
{
	if ( loopSound )
	{
		result = FMOD_Sound_SetMode(fmodSound, FMOD_LOOP_NORMAL);
	}
	else
	{
		result = FMOD_Sound_SetMode(fmodSound, FMOD_LOOP_OFF);
	}
	
	ERRCHECK(result);
	
	
	printf( "\n(Sound Manager) Playing sound: %s", fileName.c_str() );
	
	result = FMOD_System_PlaySound(inst, FMOD_CHANNEL_FREE, fmodSound, false, &channel);
	
	ERRCHECK(result);
	
	result = FMOD_Channel_SetVolume(channel, 0.8f); // Set the volume to 5% while it is paused.
	
	ERRCHECK(result);
	
	result = FMOD_Channel_SetPaused(channel, false); // This is where the sound really starts playing.
	
	ERRCHECK(result);
	
}

// -----------------------------------------------------------------------------

/*!FMOD Error Checking For Incorrect  FileNames, ect.*/
void ofxSoundObject::ERRCHECK( FMOD_RESULT result, std::string fileName )
{
	if ( result == FMOD_ERR_FILE_NOTFOUND )
	{
		printf("\n(Sound Manager) You just tried to load a sound file that doesn't exist (%s)\n", fileName.c_str() );
	}
	else if (result != FMOD_OK)
	{
		printf("\n\nFMOD Sound Error! (%d) %s", result, FMOD_ErrorString(result));
	}
}

// -----------------------------------------------------------------------------

/*!Prints Out Erros Recieved By FMOD*/
void ofxSoundObject::ERRCHECK( FMOD_RESULT result )
{
	if (result != FMOD_OK)
	{
		printf("\n\nFMOD Sound Error! (%d) %s", result, FMOD_ErrorString(result));
	}
}

