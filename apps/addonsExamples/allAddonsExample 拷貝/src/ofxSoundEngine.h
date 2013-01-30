/*
 *  ofxSoundEngine.h
 *  openFrameworks
 *
 *  Created by Arick on 7/31/09.
 *  Copyright 2009 beatsmiths. All rights reserved.
 *
 */

#pragma once

#include "ofxFMODIncludes.h"
#include "ofxSoundObject.h"


class SoundInstance {
	public:
	~SoundInstance();	
		mutable	string fileName;
		mutable FMOD_SOUND	*fmodSound;
		mutable FMOD_CHANNEL *channel;
		mutable FMOD_DSP *soundDSP;
		
		bool bMultiPlay;
		bool bLoop;
		float pan; // 0 - 1
		float volume; // 0 - 1
		float internalFreq; // 44100 ?
		float speed; // -n to n, 1 = normal, -1 backwards
		unsigned int length; // in samples;
		
		void Clear(void) { fileName = ""; }
};

class DSPInstance {
public:
	~DSPInstance();
	mutable string dspName;
	mutable FMOD_DSP *fmodDSP;
	
	void Clear(void) { dspName = ""; }

};

class Sound
	{
	private:
		signed int			INITIAL_VECTOR_SIZE;
		unsigned int		currSoundInstanceIndex;		// Holds the current position in the soundInstaceVector <FMOD::Sound vector>
		unsigned int		currDSPInstanceIndex;
				
		vector<SoundInstance*>	*soundInstanceVector;
		vector<DSPInstance*>	*dspInstanceVector;
		
		FMOD_SYSTEM *inst;
		
		FMOD_RESULT				result;
		FMOD_RESULT				fmod_on;
		int						key;
		
		void				IncrementSoundInstanceVector		();	
		void				IncrementDSPInstanceVector			();

		bool				CheckIfAlreadyLoaded				( std::string fileName, int &vectorPos );
		void				ERRCHECK							( FMOD_RESULT result );
		void				ERRCHECK							( FMOD_RESULT result, std::string fileName );
		
		bool				fmodHasBeenInitialised;
		int					m_errorNumber;

	public:
		Sound();
		~Sound();
		void PrintAllLoadedSounds( void );
		
		void UpdateSoundEngine();	//Run UpdateSoundEngine once per frame
		
		int  loadSound				( string fileLocation, string fileName );
		
		int	 GetSoundID				( std::string fileLocation );
		
		int  play					( std::string fileName, bool loopSound );
		void play					( const int soundFileID, bool loopSound );
		
		void setVolume				( const int vectorPos, float volumeLevel );
		/**/void getVolume				( const int vectorPos );
		
		void setPaused				( const int soundFileID, bool isPaused );
		bool getPaused				( const int soundFileID );
		
		void togglePauseSound		( std::string fileLocation );
		void togglePauseSound		( const int soundFileID );
		
		/*todo*/int setMultiPlay			( string fileName, bool bMp );
		/*todo*/void setMultiPlay			( const int soundFileID, bool bMp );
		
		/*todo*/bool getMultiPlay			( string fileName );
		/*todo*/bool getMultiPlay			( const int soundFileID );
		
		/*todo*/void toggleMultiPlay		( string fileLocation );
		/*todo*/void toggleMultiPlay		( const int soundFileID );
			
		void getPosition				( const int soundFileID, unsigned int &ms);
		
		///*todo*/void setParameter			
		int createParentStream(int numSubSounds);
		void setSubSound( const int soundFileID, const int subSoundID, int index);
		void setSubSentence( const int soundFileID, int	*soundSentence, int numSubSounds );

		
		void stopSound				( std::string fileLocation );
		void stopSound				( const int soundFileID );
		
		int  createDSP				( FMOD_DSP_TYPE type, std::string dspName );
		
		int  addDSP					( std::string fileName, std::string dspName );
		int  addDSP					( const int soundFileID, std::string dspName );
		void addDSP					( std::string fileName, const int dspID );
		void addDSP					( const int soundFileID, const int dspID );
		
		void removeDSP				( std::string dspName );
		
		bool getDSPActive			( std::string dspName );
		bool getDSPActive			( const int dspID );
		
		float * getSoundSpectrum	(int nBands, const int soundFileID);
	};