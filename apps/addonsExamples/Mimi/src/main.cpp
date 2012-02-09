#include "ofMain.h"
#include "testApp.h"


//========================================================================
int main( ){

	ofSetupOpenGL(movWidth, movHeight, OF_FULLSCREEN);			// <-------- setup the GL context
    ofSetWindowPosition(movWidth, 0);
    // this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:

	ofRunApp(new testApp());

}
