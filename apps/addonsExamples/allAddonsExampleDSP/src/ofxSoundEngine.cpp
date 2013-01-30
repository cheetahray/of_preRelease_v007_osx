/*
 *  ofxSoundEngine.cpp
 *  openFrameworks
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *  Created by Arick on 7/31/09.
 *  Copyright 2009 beatsmiths. All rights reserved.
 *
 *  Adapted from a great many sources on the internet, but mainly from a great example at:
 *  https://svn.assembla.com/svn/ICT207/ 
 */

#include "ofxSoundEngine.h"
#include "ofUtils.h"

float ofxSoundEnginefftValues[8192];			//
float ofxSoundEnginefftInterpValues[8192];	//
float ofxSoundEngineFFTSpectrum[8192];		// maximum # is 8192, in fmodex....

Sound::Sound()
{
	inst = ofxSystemSingleton::Instance().getSystem();
	INITIAL_VECTOR_SIZE = 10;
	currSoundInstanceIndex = -1;	// The first time this is incremented it will be first position of the vector, which should be 0 (unless something crazy happens)
	currDSPInstanceIndex = -1;
	m_errorNumber = -1;

//---------------- Setup soundInstance Vector
	soundInstanceVector = new std::vector<SoundInstance*>;
	soundInstanceVector->resize( INITIAL_VECTOR_SIZE );	

	for (int vectorIndex = 0; vectorIndex < INITIAL_VECTOR_SIZE; vectorIndex++)
	{
		soundInstanceVector->at(vectorIndex) = new SoundInstance;
	}

//---------------- Setup dspInstance Vector
	dspInstanceVector = new std::vector<DSPInstance*>;
	dspInstanceVector->resize(INITIAL_VECTOR_SIZE);
	
	for (int vectorIndex = 0; vectorIndex < INITIAL_VECTOR_SIZE; vectorIndex++)
	{
		dspInstanceVector->at(vectorIndex) = new DSPInstance;
	}
}

/*!Deconstructor For The Sound Class*/
Sound::~Sound()
{		
	if (result != FMOD_OK)
	{
		printf("\n(Sound Manager) FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		printf("\nSound could not be released cleanly when calling SoundManager() destructor");
	}
	else
	{
		printf("\n(Sound Manager) FMOD Sound Released Cleanly, SoundManager object released");
	}
	delete soundInstanceVector;
	delete dspInstanceVector;
}


// -----------------------------------------------------------------------------

/*!As All The Sounds Are Stored In A Vector You Can Pass In A Sound Name And Get Its Location Back*/
int Sound::GetSoundID( std::string fileName )
{
	int foundPosition = 0;
	
	for ( unsigned int count = 0; count < soundInstanceVector->capacity(); count++ )
	{
		if ( soundInstanceVector->at(count)->fileName == fileName )
		{
			foundPosition = count;
		}
	}
	
	if ( foundPosition != 0 )
	{
		return foundPosition;
	}
	else
	{
		return m_errorNumber;
	}
}

// -----------------------------------------------------------------------------

/* Load a file, ie: LoadMusic( "../media/drumloop.wav" );*/
int Sound::createDSP(FMOD_DSP_TYPE type, std::string dspName)
{
	this->IncrementDSPInstanceVector();
	dspInstanceVector->at(currDSPInstanceIndex)->dspName = dspName;
	result = FMOD_System_CreateDSPByType(inst, type, &dspInstanceVector->at(currDSPInstanceIndex)->fmodDSP);
	ERRCHECK(result);
	return currDSPInstanceIndex;
}

int Sound::loadSound( std::string fileLocation, string fileName )
{
	int vectorPos;
	bool isLoaded = this->CheckIfAlreadyLoaded( fileLocation, vectorPos );
	
	if ( !isLoaded )
	{
		this->IncrementSoundInstanceVector();
		
		soundInstanceVector->at( currSoundInstanceIndex )->channel = 0;
		soundInstanceVector->at( currSoundInstanceIndex )->fileName = fileName;
		printf("\n(Sound Manager) Loading sound: %s", fileLocation.c_str() );
		result = FMOD_System_CreateStream(inst, fileLocation.c_str(), (FMOD_SOFTWARE | FMOD_CREATESTREAM), NULL, &soundInstanceVector->at( currSoundInstanceIndex )->fmodSound);
		//result = FMOD_System_CreateSound(&inst, fileLocation.c_str(), (FMOD_SOFTWARE | FMOD_CREATESTREAM), NULL, &soundInstanceVector->at( currSoundInstanceIndex )->fmodSound );	// FMOD_HARDWARE
		ERRCHECK(result, fileLocation);
		
		result = FMOD_Sound_SetMode(soundInstanceVector->at( currSoundInstanceIndex )->fmodSound, FMOD_LOOP_OFF);
		
		ERRCHECK(result);
		
		return currSoundInstanceIndex;
	}
	else
	{
		printf("\n(Sound Manager) Warning!: %s could not be loaded, because it has already been loaded! LoadMusic() returning it's soundID: ( %d )", fileLocation.c_str(), vectorPos );
		return vectorPos;
	}
}

// -----------------------------------------------------------------------------

/*!Accepts A Sound Name And A Loop Flag And Plays That Sound*/
int Sound::play( std::string fileName, bool loopSound )
{
	int vectorPos = m_errorNumber;
	
	for ( unsigned int count = 0; count < soundInstanceVector->capacity(); count++ )
	{
		if ( soundInstanceVector->at(count)->fileName == fileName )
		{
			vectorPos = count;
			count = soundInstanceVector->capacity();
		}
	}
	
	if ( vectorPos != m_errorNumber )
	{
		if ( loopSound )
		{
			result = FMOD_Sound_SetMode(soundInstanceVector->at( vectorPos )->fmodSound, FMOD_LOOP_NORMAL);
		}
		else
		{
			result = FMOD_Sound_SetMode(soundInstanceVector->at( vectorPos )->fmodSound, FMOD_LOOP_OFF);
		}
		
		ERRCHECK(result);

		
		printf( "\n(Sound Manager) Playing sound: %s", soundInstanceVector->at(vectorPos)->fileName.c_str() );
		
		result = FMOD_System_PlaySound(inst, FMOD_CHANNEL_FREE, soundInstanceVector->at(vectorPos)->fmodSound, false, &soundInstanceVector->at(vectorPos)->channel);

		ERRCHECK(result);
				
		result = FMOD_Channel_SetVolume(soundInstanceVector->at(vectorPos)->channel, 0.8f); // Set the volume to 5% while it is paused.

		ERRCHECK(result);
		
		result = FMOD_Channel_SetPaused(soundInstanceVector->at(vectorPos)->channel, false); // This is where the sound really starts playing.
		
		ERRCHECK(result);
		
	}
	else
	{
		printf( "\n(Sound Manager) Could not play: %s as it was not found, you probably haven't called LoadMusic() yet", fileName.c_str() );
	}
	
	return vectorPos;
}

// -----------------------------------------------------------------------------

/*!Overloaded PlayMusic With SoundID Instead Of Name*/
void Sound::play( const int soundFileID, bool loopSound )
{
	// If the soundInstanceVector has not been initialised, OR the id you are trying to load is greater than its size THEN
	if ( (unsigned int)soundFileID > soundInstanceVector->size() && soundFileID > m_errorNumber )
	{
		printf("\n(Sound Manager) Calling PlayMusic() with soundID: %d. \nHowever this id is greater than the size of the internal array.\nYou are either passing in the wrong soundID variable, or you\nhave not loaded the sound you are referring to", soundFileID );
	}
	else if ( soundFileID == m_errorNumber )
	{
		printf("\n(Sound Manager) Calling PlayMusic() with soundID: %d. \nError: This soundID ( %d ) either previously failed loading OR it was never loaded to begin with.", soundFileID, m_errorNumber );
	}
	else	// Play ze Musics!
	{
		if ( loopSound )
		{
			result = FMOD_Sound_SetMode(soundInstanceVector->at( soundFileID )->fmodSound, FMOD_LOOP_NORMAL);
		}
		else
		{
			result = FMOD_Sound_SetMode(soundInstanceVector->at( soundFileID )->fmodSound, FMOD_LOOP_OFF);
		}
		ERRCHECK(result);
		
		result = FMOD_System_PlaySound(inst, FMOD_CHANNEL_FREE, soundInstanceVector->at(soundFileID)->fmodSound, false, &soundInstanceVector->at(soundFileID)->channel );
		
		ERRCHECK(result);
		
		//result = FMOD_Channel_SetVolume(soundInstanceVector->at(soundFileID)->channel, 0.2f);  // Set the volume to 5% while it is paused.
		
		ERRCHECK(result);
		
		result = FMOD_Channel_SetPaused(soundInstanceVector->at(soundFileID)->channel,false);  // This is where the sound really starts playing.

		ERRCHECK(result);
		
		printf( "\n(Sound Manager) Playing sound: %s", soundInstanceVector->at(soundFileID)->fileName.c_str() );
	}
}

// -----------------------------------------------------------------------------

/*!Toggles The Music*/
void Sound::togglePauseSound( std::string fileLocation )
{
	bool found = false;
	int isPaused = 0;
	int foundIndex = 0;
	
	// -----------------------------------------------------------------------------
	// Find the sound in the soundInstanceVector
	// -----------------------------------------------------------------------------
	// Check if the last allocated soundInstance equals our passed in file, to potentially save search time as this was most recently used
	if ( soundInstanceVector->at(currSoundInstanceIndex)->fileName == fileLocation )
	{
		found = true;								// We have found the position
		foundIndex = currSoundInstanceIndex;		// Save the found location
	}
	else	// If we did not find the file, then search the entire vector for it
	{
		found = false;
		// Look for the fileName in our soundInstanceVector
		for ( unsigned int index = 0; index < soundInstanceVector->capacity(); index++ )
		{
			// Check if we found the file from our soundInstanceVector
			if ( soundInstanceVector->at(index)->fileName == fileLocation )
			{
				found = true;								// We have found the position
				foundIndex = index;							// Save the found location
				index =	soundInstanceVector->capacity();	// exit the for loop
				
				// Check if the sound is currently paused or not. TRUE = paused, FALSE = not paused.
				FMOD_Channel_GetPaused(soundInstanceVector->at(foundIndex)->channel, &isPaused);
			}
		}
	}
	
	// -----------------------------------------------------------------------------
	// Toggle the sound playing
	// -----------------------------------------------------------------------------
	if ( found == true )	// If we found the sound in the vector, then pause it
	{
		// Check if the sound is currently paused or not. TRUE = paused, FALSE = not paused.
		FMOD_Channel_GetPaused(soundInstanceVector->at(foundIndex)->channel, &isPaused);	
		
		// If sound is paused, then play it
		if ( isPaused != 0 )
		{
			result = FMOD_Channel_SetPaused(soundInstanceVector->at(foundIndex)->channel, false);
			ERRCHECK(result);
			printf("\n(Sound Manager) File Unpaused: %s", soundInstanceVector->at(foundIndex)->fileName.c_str() );
		}
		else	// Pause the sound
		{
			result = FMOD_Channel_SetPaused(soundInstanceVector->at(foundIndex)->channel, true);
			ERRCHECK(result);
			printf("\n(Sound Manager) File Paused: %s", soundInstanceVector->at(foundIndex)->fileName.c_str() );
		}
	}	
	else	// Else, do nothing and print an error message.
	{
		found = false;
		printf("\n(Sound Manager) Could not Pause %s, as file not found", fileLocation.c_str() );
	}
	// -----------------------------------------------------------------------------
}

// -----------------------------------------------------------------------------



/*!OverLoaded Music Toggle Using Sound ID*/
void Sound::togglePauseSound( const int soundFileID )
{
	int isPaused = 0;
	
	// -----------------------------------------------------------------------------
	// Toggle the sound playing
	// -----------------------------------------------------------------------------
	// Check if the sound is currently paused or not. TRUE = paused, FALSE = not paused.
	FMOD_Channel_GetPaused(soundInstanceVector->at(soundFileID)->channel, &isPaused);	
	printf("%l", isPaused);
	// If sound is paused, then play it
	if ( isPaused != 0 )
	{
		result = FMOD_Channel_SetPaused(soundInstanceVector->at(soundFileID)->channel, false);
		ERRCHECK(result);
		printf("\n(Sound Manager) File Unpaused: %s", soundInstanceVector->at(soundFileID)->fileName.c_str() );
	}
	else if ( isPaused = 0 )	// Pause the sound
	{
		result = FMOD_Channel_SetPaused(soundInstanceVector->at(soundFileID)->channel, true);
		ERRCHECK(result);
		printf("\n(Sound Manager) File Paused: %s", soundInstanceVector->at(soundFileID)->fileName.c_str() );
	}
	else	// Else, do nothing and print an error message.
	{
		printf("\n(Sound Manager) Could not Pause %s, as file not found", soundInstanceVector->at(soundFileID)->fileName.c_str() );
	}
	
}

// -----------------------------------------------------------------------------

/*!Manually Set If A Sound Is Paused*/
void Sound::setPaused( const int soundFileID, bool isPaused )
{
	if ( soundFileID != m_errorNumber )
	{
		if ( isPaused )
		{
			printf("\n(Sound Manager) File Paused: %s", soundInstanceVector->at(soundFileID)->fileName.c_str() );
		}
		else
		{
			printf("\n(Sound Manager) File UnPaused: %s", soundInstanceVector->at(soundFileID)->fileName.c_str() );
		}
		result = FMOD_Channel_SetPaused(soundInstanceVector->at(soundFileID)->channel, isPaused);
		ERRCHECK(result);
	}
	else
	{
		printf("\n(Sound Manager) ERROR! When calling SetPaused(%d), this is an invalid soundID, volume was not set.", soundFileID );
	}
	
}

bool Sound::getPaused( const int soundFileID )
{
	int isPaused = 0;
	FMOD_Channel_GetPaused(soundInstanceVector->at(soundFileID)->channel, &isPaused);	
	return (isPaused != 0 ? true : false);
}

// -----------------------------------------------------------------------------

/*!Sets The Built In FMOD API's Sound Level For A Specified Track*/
void Sound::setVolume( const int vectorPos, float volumeLevel )
{
	if ( vectorPos != m_errorNumber )
	{
		result = FMOD_Channel_SetVolume(soundInstanceVector->at( vectorPos )->channel, volumeLevel);
		ERRCHECK(result);
		if (result != FMOD_OK)
		{
			printf("\n(Sound Manager) Warning: When calling SetVolume() for soundID: %d in internal vector", vectorPos );
			printf("\n(Sound Manager) Remeber that SetVolume() should only be called AFTER PlaySound() has been called. You may be calling it before, which won't work.");
		}
		else
		{
			printf("\n(Sound Manager) Volume set to %f", volumeLevel);
		}
	}
	else
	{
		printf("\n(Sound Manager) Warning: When calling SetVolume(%d), this is an invalid soundID, volume was not set.", m_errorNumber );
	}
	
}

// -----------------------------------------------------------------------------

//! This is important! This must be called once per frame. Do not call any more than that as it is just inefficient and makes our code more like Shay's code.
void Sound::UpdateSoundEngine( void )
{
	result = FMOD_System_Update(inst);
	if (result != FMOD_OK)
	{
		printf("\n(Sound Manager) UpdateSoundEngine: ERROR! FMOD Sound could not be updated! Sound.cpp on line %d", __LINE__ );
	}
}

// -----------------------------------------------------------------------------

/*!Checks To See If A Specified Sound Has Already Been Loaded, In Order To Prevent Duplication*/
bool Sound::CheckIfAlreadyLoaded( std::string fileName, int &vectorPos )
{
	bool isLoaded = false;
	// Check if the fileName already exists in the array
	for ( unsigned int index = 0; index < soundInstanceVector->capacity(); index++ )
	{
		if ( soundInstanceVector->at( index )->fileName.c_str() == fileName )
		{
			vectorPos = index;
			isLoaded = true;		// It is not okay to increase the size of the vector, the fileName already exists int the vector
			printf("\n(Sound Manager) Warning! %s could not be loaded as it has already been loaded into soundInstanceVector.", fileName.c_str() );
			index = soundInstanceVector->capacity();	// Exit out of the for loop
		}
	}
	
	return isLoaded;
}

// -----------------------------------------------------------------------------

/*!Sets Up A New Position In The Vector And Initialises It*/
void Sound::IncrementDSPInstanceVector()
{
	signed int oldCapacity = dspInstanceVector->capacity();
	signed int newCapacity = oldCapacity * 2;
	
	if ( dspInstanceVector->capacity() == currSoundInstanceIndex )
	{
		printf( "\n(Sound Manager) dspInstanceVector is :%d", (int)dspInstanceVector->capacity() );
		dspInstanceVector->resize( newCapacity );
		printf( "\n(Sound Manager) dspInstanceVector resized to :%d", (int)dspInstanceVector->capacity() );
		
		// Initialise the new vector space
		for ( int vectorIndex = oldCapacity; vectorIndex < newCapacity; vectorIndex++ )
		{
			dspInstanceVector->at(vectorIndex) = new DSPInstance;
		}
	}
	
	// INCREMENT the currSoundInstanceIndex if there is space in the vector	
	if ( currDSPInstanceIndex + 1 < (unsigned int)dspInstanceVector->capacity() )
	{
		currDSPInstanceIndex++;
	}
	else
	{
		printf( "\n(Sound Manager) ERROR: Could not increment currDSPInstanceIndex" );
	}
}

// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------

/*!Sets Up A New Position In The Vector And Initialises It*/
void Sound::IncrementSoundInstanceVector()
{
	signed int oldCapacity = soundInstanceVector->capacity();
	signed int newCapacity = oldCapacity * 2;
	
	if ( soundInstanceVector->capacity() == currSoundInstanceIndex )
	{
		printf( "\n(Sound Manager) soundInstanceVector is :%d", (int)soundInstanceVector->capacity() );
		soundInstanceVector->resize( newCapacity );
		printf( "\n(Sound Manager) soundInstanceVector resized to :%d", (int)soundInstanceVector->capacity() );
		
		// Initialise the new vector space
		for ( int vectorIndex = oldCapacity; vectorIndex < newCapacity; vectorIndex++ )
		{
			soundInstanceVector->at(vectorIndex) = new SoundInstance;
			soundInstanceVector->at(vectorIndex)->Clear();
			soundInstanceVector->at(vectorIndex)->channel = 0;			
		}
	}
	
	// INCREMENT the currSoundInstanceIndex if there is space in the vector	
	if ( currSoundInstanceIndex + 1 < (unsigned int)soundInstanceVector->capacity() )
	{
		currSoundInstanceIndex++;
	}
	else
	{
		printf( "\n(Sound Manager) ERROR: Could not increment currSoundInstanceIndex" );
	}
}

// -----------------------------------------------------------------------------

/*!FMOD Error Checking For Incorrect  FileNames, ect.*/
void Sound::ERRCHECK( FMOD_RESULT result, std::string fileName )
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
void Sound::ERRCHECK( FMOD_RESULT result )
{
	if (result != FMOD_OK)
	{
		printf("\n\nFMOD Sound Error! (%d) %s", result, FMOD_ErrorString(result));
	}
}

// -----------------------------------------------------------------------------

int Sound::createParentStream(int numSubSounds)
{
	FMOD_CREATESOUNDEXINFO exinfo;
	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    
    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.defaultfrequency = 44100;
    exinfo.numsubsounds = numSubSounds;
    exinfo.numchannels = 2;
	exinfo.format = FMOD_SOUND_FORMAT_PCM16;
	int vectorPos;
	this->IncrementSoundInstanceVector();
		
	soundInstanceVector->at( currSoundInstanceIndex )->channel = 0;
	result = FMOD_System_CreateStream(inst, 0, (FMOD_LOOP_OFF | FMOD_OPENUSER), &exinfo, &soundInstanceVector->at( currSoundInstanceIndex )->fmodSound);
	ERRCHECK(result);
	printf("\n The Parent is %d", currSoundInstanceIndex);	
	return currSoundInstanceIndex;
}

void Sound::setSubSound(const int soundFileID, const int subSoundID, int index)
{
	result = FMOD_Sound_SetSubSound(soundInstanceVector->at(soundFileID)->fmodSound, index, soundInstanceVector->at(subSoundID)->fmodSound);
	ERRCHECK(result);
}

void Sound::setSubSentence(const int soundFileID, int *soundSentence, int numSubSounds )
{
	result = FMOD_Sound_SetSubSoundSentence(soundInstanceVector->at(soundFileID)->fmodSound, soundSentence, numSubSounds);
	ERRCHECK(result);
}


// -----------------------------------------------------------------------------

/*!Prints Out All The Sounds Which Are In The Sound Vector To Screen*/
void Sound::PrintAllLoadedSounds( void )
{
	printf( "\n\n(Sound Manager) Capacity of Sound Vector: %d/%d", (int)soundInstanceVector->size(), (int)soundInstanceVector->capacity() );
	
	for ( unsigned int index = 0; index < soundInstanceVector->size(); index++ )
	{
		FMOD_BOOL isPaused = FMOD_Channel_GetPaused(soundInstanceVector->at(index)->channel, &isPaused);
		if ( isPaused )
		{
			printf("\n(Sound Manager) SoundID: %d, Paused: Yes, Filename: %s ", index, soundInstanceVector->at(index)->fileName.c_str() );
		}
		else
		{
			printf("\n(Sound Manager) SoundID: %d, Paused: No,  Filename: %s ", index, soundInstanceVector->at(index)->fileName.c_str() );
		}
		
	}
}


// -----------------------------------------------------------------------------

int Sound::addDSP( std::string fileName, std::string dspName ){
	
	int vectorSoundPos = m_errorNumber;
	int vectorDSPPos = m_errorNumber;
	
	
	for ( unsigned int count = 0; count < soundInstanceVector->capacity(); count++ )
	{
		if ( soundInstanceVector->at(count)->fileName == fileName )
		{
			vectorSoundPos = count;
			count = soundInstanceVector->capacity();
		}
	}
	
	for ( unsigned int count = 0; count < dspInstanceVector->capacity(); count++ )
	{
		if ( dspInstanceVector->at(count)->dspName == dspName )
		{
			vectorDSPPos = count;
			count = dspInstanceVector->capacity();
		}
	}
	
	
	if ( vectorSoundPos != m_errorNumber &&  vectorDSPPos != m_errorNumber )
	{
		result = FMOD_Channel_AddDSP(soundInstanceVector->at(vectorSoundPos)->channel, dspInstanceVector->at(vectorDSPPos)->fmodDSP, 0);
		ERRCHECK(result);
		
	}
	
	return vectorDSPPos;
}

// -----------------------------------------------------------------------------
/*!Overloaded addDSP With SoundID Instead Of Name returns dspID*/

int Sound::addDSP( const int soundFileID, std::string dspName ){
	
	int vectorDSPPos = m_errorNumber;
	
	for ( unsigned int count = 0; count < dspInstanceVector->capacity(); count++ )
	{
		if ( dspInstanceVector->at(count)->dspName == dspName )
		{
			vectorDSPPos = count;
			count = dspInstanceVector->capacity();
		}
	}
	
	
	if ( vectorDSPPos != m_errorNumber &&  (unsigned int)soundFileID <= soundInstanceVector->size() )
	{
		result = FMOD_Channel_AddDSP(soundInstanceVector->at(soundFileID)->channel, dspInstanceVector->at(vectorDSPPos)->fmodDSP, 0);
		ERRCHECK(result);
		
	}
	
	return vectorDSPPos;
}


// -----------------------------------------------------------------------------
/*!Overloaded addDSP With dspID Instead Of Name*/

void Sound::addDSP(std::string fileName , const int dspID ){
	
	int vectorSoundPos = m_errorNumber;
	
	for ( unsigned int count = 0; count < soundInstanceVector->capacity(); count++ )
	{
		if ( soundInstanceVector->at(count)->fileName == fileName )
		{
			vectorSoundPos = count;
			count = soundInstanceVector->capacity();
		}
	}
	
	if ( vectorSoundPos != m_errorNumber &&  (unsigned int)dspID <= dspInstanceVector->size())
	{
		result = FMOD_Channel_AddDSP(soundInstanceVector->at(vectorSoundPos)->channel, dspInstanceVector->at(dspID)->fmodDSP, 0);
		ERRCHECK(result);
		
	}
}

// -----------------------------------------------------------------------------
/*!Overloaded addDSP With SoundID and dspID instead Of Names*/

void Sound::addDSP( const int soundFileID, const int dspID ){
	
	if ( (unsigned int)soundFileID <= soundInstanceVector->size() &&  (unsigned int)dspID <= dspInstanceVector->size())
	{
		result = FMOD_Channel_AddDSP(soundInstanceVector->at(soundFileID)->channel, dspInstanceVector->at(dspID)->fmodDSP, 0);
		ERRCHECK(result);
		
	}
}

// -----------------------------------------------------------------------------
	
void Sound::removeDSP( std::string dspName )
{
	int vectorDSPPos = m_errorNumber;

	for ( unsigned int count = 0; count < dspInstanceVector->capacity(); count++ )
	{
		if ( dspInstanceVector->at(count)->dspName == dspName )
		{
			vectorDSPPos = count;
			count = dspInstanceVector->capacity();
		}
	}
		
	if ( vectorDSPPos != m_errorNumber )
	{
		result = FMOD_DSP_Remove(dspInstanceVector->at(vectorDSPPos)->fmodDSP);
		ERRCHECK(result);
	}
		
}
		
// -----------------------------------------------------------------------------

bool Sound::getDSPActive( std::string dspName )
{
	int vectorDSPPos = m_errorNumber;
	
	for ( unsigned int count = 0; count < dspInstanceVector->capacity(); count++ )
	{
		if ( dspInstanceVector->at(count)->dspName == dspName )
		{
			vectorDSPPos = count;
			count = dspInstanceVector->capacity();
		}
	}

		int active = 0;	
		FMOD_DSP_GetActive(dspInstanceVector->at(vectorDSPPos)->fmodDSP, &active);
		return (active != 0 ? true : false);	
}

bool Sound::getDSPActive( const int dspID )
{
	int active = 0;	
	FMOD_DSP_GetActive(dspInstanceVector->at(dspID)->fmodDSP, &active);
	return (active != 0 ? true : false);	
}

void Sound::getPosition( const int soundFileID, unsigned int &ms)
{
	int subSoundCheck;
	FMOD_Sound_GetNumSubSounds(soundInstanceVector->at(soundFileID)->fmodSound, &subSoundCheck);
	if(subSoundCheck > 0){
		result = FMOD_Channel_GetPosition(soundInstanceVector->at(soundFileID)->channel, &ms, (FMOD_TIMEUNIT)(FMOD_TIMEUNIT_SENTENCE_SUBSOUND | FMOD_TIMEUNIT_MS));
		ERRCHECK(result);
	} else {
		result = FMOD_Channel_GetPosition(soundInstanceVector->at(soundFileID)->channel, &ms, FMOD_TIMEUNIT_MS);
		ERRCHECK(result);
	}

}
//---------------------------------------------------------------------------------

float * Sound::getSoundSpectrum(int nBands, const int soundFileID){
	
	// 	set to 0
	for (int i = 0; i < 8192; i++){
		ofxSoundEnginefftInterpValues[i] = 0;
	}
	
	// 	check what the user wants vs. what we can do:
	if (nBands > 8192){
		ofLog(OF_LOG_ERROR, "error in ofSoundGetSpectrum, the maximum number of bands is 8192 - you asked for %i we will return 8192", nBands);
		nBands = 8192;
	} else if (nBands <= 0){
		ofLog(OF_LOG_ERROR, "error in ofSoundSpectrum, you've asked for %i bands, minimum is 1", nBands);
		return ofxSoundEnginefftInterpValues;
	}
	
	// 	FMOD needs pow2
	int nBandsToGet = ofNextPow2(nBands);
	if (nBandsToGet < 64) nBandsToGet = 64;  // can't seem to get fft of 32, etc from fmodex
	
	// 	get the fft
	FMOD_Channel_GetSpectrum(soundInstanceVector->at(soundFileID)->channel, ofxSoundEngineFFTSpectrum, nBandsToGet, 0, FMOD_DSP_FFT_WINDOW_HANNING);
	
	// 	convert to db scale
	for(int i = 0; i < nBandsToGet; i++){
        ofxSoundEnginefftValues[i] = 10.0f * (float)log10(1 + ofxSoundEngineFFTSpectrum[i]) * 2.0f;
	}
	
	// 	try to put all of the values (nBandsToGet) into (nBands)
	//  in a way which is accurate and preserves the data:
	//
	
	if (nBandsToGet == nBands){
		
		for(int i = 0; i < nBandsToGet; i++){
			ofxSoundEnginefftInterpValues[i] = ofxSoundEnginefftValues[i];
		}
		
	} else {
		
		float step 		= (float)nBandsToGet / (float)nBands;
		//float pos 		= 0;
		// so for example, if nBands = 33, nBandsToGet = 64, step = 1.93f;
		int currentBand = 0;
		
		for(int i = 0; i < nBandsToGet; i++){
			
			// if I am current band = 0, I care about (0+1) * step, my end pos
			// if i > endPos, then split i with me and my neighbor
			
			if (i >= ((currentBand+1)*step)){
				
				// do some fractional thing here...
				float fraction = ((currentBand+1)*step) - (i-1);
				float one_m_fraction = 1 - fraction;
				ofxSoundEnginefftInterpValues[currentBand] += fraction * ofxSoundEnginefftValues[i];
				currentBand++;
				// safety check:
				if (currentBand >= nBands){
					ofLog(OF_LOG_ERROR, "ofSoundGetSpectrum - currentBand >= nBands");
				}
				
				ofxSoundEnginefftInterpValues[currentBand] += one_m_fraction * ofxSoundEnginefftValues[i];
				
			} else {
				// do normal things
				ofxSoundEnginefftInterpValues[currentBand] += ofxSoundEnginefftValues[i];
			}
		}
		
		// because we added "step" amount per band, divide to get the mean:
		for (int i = 0; i < nBands; i++){
			ofxSoundEnginefftInterpValues[i] /= step;
			if (ofxSoundEnginefftInterpValues[i] > 1)ofxSoundEnginefftInterpValues[i] = 1; 	// this seems "wrong"
		}
		
	}
	
	return ofxSoundEnginefftInterpValues;
}

