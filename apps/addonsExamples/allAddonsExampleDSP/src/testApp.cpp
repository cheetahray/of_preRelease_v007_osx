#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//Allways run Init. 
	//testSystem.Initialise();
	
	
	//loadSound returns an int for optional indexing, but you can also reference by file name
	drumID = testSystem.loadSound("/Users/amywah/Documents/of_preRelease_v007_osx/apps/addonsExamples/allAddonsExampleDSP/bin/data/drums.aif","drums");
	stringID = testSystem.loadSound("/Users/amywah/Documents/of_preRelease_v007_osx/apps/addonsExamples/allAddonsExampleDSP/bin/data/09 strings 2.aif","strings");
	loopTest.loadSound("/Users/amywah/Documents/of_preRelease_v007_osx/apps/addonsExamples/allAddonsExampleDSP/bin/data/testbeat2.aif");
	loopTest.play(true);
	/*example of using subSounds to create seemless playback of multiple samples*/
	subSoundID[0] = testSystem.loadSound("/Users/amywah/Documents/of_preRelease_v007_osx/apps/addonsExamples/allAddonsExampleDSP/bin/data/testbeat1.aif","beat1");
	subSoundID[1] = testSystem.loadSound("/Users/amywah/Documents/of_preRelease_v007_osx/apps/addonsExamples/allAddonsExampleDSP/bin/data/testbeat2.aif","beat2");
	
	//create Parent stream to load subsounds into.
	soundSentenceID = testSystem.createParentStream(2);
	
	//load subsounds
	testSystem.setSubSound(soundSentenceID, subSoundID[0],0);
	testSystem.setSubSound(soundSentenceID, subSoundID[1],1);
	
	//create soundlist, basically order of which to play the sounds
	int soundList[2] = {1,0};
	
	//set up the "sentence"
	testSystem.setSubSentence(soundSentenceID, soundList, 2);	
	
	//play the "sentence"
	testSystem.play(soundSentenceID, TRUE);
	
	
	//Example of reference by file name
	testSystem.play("drums", TRUE);
	
	//Example of reference by file ID
	testSystem.play(stringID, TRUE);

	
	testSystem.togglePauseSound("strings");
	testSystem.togglePauseSound("drums");

	//Setting a Volume. Note: Should be done after play.
	testSystem.setVolume(stringID, 1.0);
	
	//You create a dsp by calling createDSP(type, name). The name is, once again, for reference
	testSystem.createDSP(DSP_DISTORTION, "drumDistort");
	testSystem.createDSP(DSP_ECHO, "stringEcho");

	//Example of getting dspID 
	dspID[0] = testSystem.createDSP(DSP_CHORUS, "Chorus");
	dspID[1] = testSystem.createDSP(DSP_FLANGE, "Flange");
	
	//There are several ways to add DSPs into the chain of things
	
	//Example of reference by name for both file and dsp
	testSystem.addDSP("drums","drumDistort");
	
	//Example of reference by file ID and dsp name
	testSystem.addDSP(stringID,"stringEcho");
	
	//Example of reference by file ID and dsp ID
	testSystem.addDSP(drumID,dspID[1]);
	
	//Example of reference by file name and dsp ID
	testSystem.addDSP("/Users/amywah/Documents/of_preRelease_v007_osx/apps/addonsExamples/allAddonsExampleDSP/bin/data/09 strings 2.aif", dspID[0]);
	
	fftSmoothed1 = new float[8192];
	for (int i = 0; i < 8192; i++){
		fftSmoothed1[i] = 0;
	}
	fftSmoothed2 = new float[8192];
	for (int i = 0; i < 8192; i++){
		fftSmoothed2[i] = 0;
	}
	
	font.loadFont("Sudbury_Basin_3D.ttf", 32);

	nBandsToGet = 128;
	
	

}

//--------------------------------------------------------------
void testApp::update(){
	testSystem.UpdateSoundEngine();
	
	float * val = testSystem.getSoundSpectrum(nBandsToGet, stringID);		// request 128 values for fft
	for (int i = 0;i < nBandsToGet; i++){
		
		// let the smoothed calue sink to zero:
		fftSmoothed1[i] *= 0.96f;
		
		// take the max, either the smoothed or the incoming:
		if (fftSmoothed1[i] < val[i]) fftSmoothed1[i] = val[i];
		
	}
	
	val = testSystem.getSoundSpectrum(nBandsToGet, drumID);
	for (int i = 0;i < nBandsToGet; i++){
		
		// let the smoothed calue sink to zero:
		fftSmoothed2[i] *= 0.96f;
		
		// take the max, either the smoothed or the incoming:
		if (fftSmoothed2[i] < val[i]) fftSmoothed2[i] = val[i];
		
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	
	char tempStr[255];
	
	float width = (float)(5*128) / nBandsToGet;
	for (int i = 0;i < nBandsToGet; i++){
		// (we use negative height here, because we want to flip them
		// because the top corner is 0,0)
		ofSetColor(255,255,255);
		ofRect(100+i*width,ofGetHeight()-100,width,-(fftSmoothed1[i] * 100));
		ofEnableAlphaBlending();
		ofSetColor(255,0,0,60);
		ofRect(100+i*width,ofGetHeight()-100,width,-(fftSmoothed2[i] * 100));
		ofDisableAlphaBlending();
		ofSetColor(255,255,255);
	}
	testSystem.getPosition(stringID, ms);
	ofSetColor(0x000000);
	sprintf(tempStr, "String Sample\nTime %02d:%02d:%02d",  ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100);
	ofDrawBitmapString(tempStr, 50,ofGetHeight()-50);
	
	testSystem.getPosition(drumID, ms);
	bool bPaused = testSystem.getPaused(drumID);
	
	sprintf(tempStr, "Drum Sample\nTime %02d:%02d:%02d\nPaused:%s",  ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100,(bPaused)?"true":"false");
	ofDrawBitmapString(tempStr, 250,ofGetHeight()-50);
	
	//testSystem.getPosition(soundSentenceID, ms);
	//sprintf(tempStr, "Sound Sentence Sample\nTime %02d:%02d:%02d",  ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100);
	//ofDrawBitmapString(tempStr, 450,ofGetHeight()-50);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	switch(key) {
		case 'd':
			if(testSystem.getDSPActive("drumDistort")){
				testSystem.removeDSP("drumDistort");
			}else {
				testSystem.addDSP("drums","drumDistort");
			}
			
			break;
		case 'e':
			if(testSystem.getDSPActive("stringEcho")){
				testSystem.removeDSP("stringEcho");
			}else {
				testSystem.addDSP("strings","stringEcho");
			}			
			break;
		case 'f':
			if(testSystem.getDSPActive("Flange")){
				testSystem.removeDSP("Flange");
			}else {
				testSystem.addDSP("drums","Flange");
			}			
			break;
		case 'c':
			if(testSystem.getDSPActive("Chorus")){
				testSystem.removeDSP("Chorus");
			}else {
				testSystem.addDSP("drums","Chorus");
			}			
			break;
		case 's':
			testSystem.togglePauseSound("strings");
			testSystem.togglePauseSound("drums");
			break;
			
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

