/*
 *  ofxSoundObject.h
 *  openFrameworks
 *
 *  Created by Arick on 8/17/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofxFMODIncludes.h"

class ofxSoundObject {
public:
	ofxSoundObject();
	//~ofxSoundObject();	
	mutable	string			fileName;
	mutable FMOD_SOUND		*fmodSound;
	mutable FMOD_CHANNEL	*channel;
	mutable FMOD_DSP		*soundDSP;
	FMOD_SYSTEM				*inst;
	
	FMOD_RESULT				result;

	bool bMultiPlay;
	bool bLoop;
	float pan; // 0 - 1
	float volume; // 0 - 1
	float internalFreq; // 44100 ?
	float speed; // -n to n, 1 = normal, -1 backwards
	unsigned int length; // in samples;
	
	void  loadSound				( string fileLocation);
	void  play					( bool loopSound );
	
	void Clear(void) { fileName = ""; }
private:
	void ERRCHECK( FMOD_RESULT result, std::string fileName );
	void ERRCHECK( FMOD_RESULT result );
};
