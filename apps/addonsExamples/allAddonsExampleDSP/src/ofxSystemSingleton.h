/*
 *  ofxSystemSingleton.h
 *  openFrameworks
 *
 *  Created by Arick on 8/17/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "ofxFMODIncludes.h"

class ofxSystemSingleton
{
	public:
		FMOD_SYSTEM *getSystem() {return system;}
		FMOD_RESULT				result;
		unsigned int			version;
	
	static ofxSystemSingleton &Instance()
	{
		// This line only runs once, thus creating the only instance in existence
		static ofxSystemSingleton *instance = new ofxSystemSingleton;
		// dereferencing the variable here, saves the caller from having to use 
		// the arrow operator, and removes tempation to try and delete the 
		// returned instance.
		return *instance;
	}
	private:
		ofxSystemSingleton()
		{
		
		result = FMOD_System_Release(system);
			ERRCHECK(result);
			printf("\n(Sound Manager) Initialising FMOD Sound System..." );
					
			// **** Initialise the FMOD system ****
			result = FMOD_System_Create(&system);		// Create the main system object.
			if (result != FMOD_OK)
			{
				printf("\n(Sound Manager) FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
				printf("\nSound Has Been Disabled");
			}
					
			// Use ALSA if we are running on Linux
			
			result = FMOD_System_SetSoftwareChannels(system, 100);		// Allow 100 software mixed voices to be audible at once.
			ERRCHECK(result);
			if (result != FMOD_OK)
			{
				printf("\n(Sound Manager) Software Channels could not be set correctly. Error");
			}
					
			result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
			if (result != FMOD_OK)
			{
				printf("\n(Sound Manager) FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
				printf("\nSound Has Been Disabled");
			}
				
			result = FMOD_System_GetVersion(system, &version);
			ERRCHECK(result);
			if (version < FMOD_VERSION)
			{
				printf("\n(Sound Manager) Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
			}				
			printf("Okay");
			ERRCHECK(result);
			
		} // default constructor available only to members or friends of this class
		
		// Note that the next two functions are not given bodies, thus any attempt 
		// to call them implicitly will return as compiler errors. This prevents 
		// accidental copying of the only instance of the class.
		ofxSystemSingleton(const ofxSystemSingleton &old); // disallow copy constructor
		
		void ERRCHECK( FMOD_RESULT result )
		{
			if (result != FMOD_OK)
			{
				printf("\n\nFMOD Sound Error! (%d) %s", result, FMOD_ErrorString(result));
			}
		}
	
		// Note that although this should be allowed, 
		// some compilers may not implement private destructors
		// This prevents others from deleting our one single instance, which was otherwise created on the heap
		~ofxSystemSingleton(){} 
	private:
	FMOD_SYSTEM *system;
};
		