#ifndef _TEST_APP
#define _TEST_APP

/**
 *
 *	testApp.h
 *
 *  ofxProjectorBlend sample project
 *  
 *  Created by James George ( http://www.jamesgeorge.org ) Sep. 20 2010
 *  In collaboration with FlightPhase ( http://www.flightphase.com )
 *
 *  Copyright (c) 2010 
 *
 */

#include "ofMain.h"
#include "ofxProjectorBlend.h"
#include "ofxNetwork.h"

//let's pretend we are blending 2 projectors 
//side by side, set at a display resolution of 640x480
//with a 40 pixel overlap
#define PIXEL_OVERLAP 256
#define PROJECTOR_COUNT 1
#define PROJECTOR_WIDTH 1280
#define PROJECTOR_HEIGHT 960

class testApp : public ofBaseApp{

  public:
	void setup();
	void update();
	void draw();
    void xxyy(int key);
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	ofRectangle guiIn, guiOut;
	ofxProjectorBlend blender;
    
    int index[12], firstTime, connectTime, deltaTime, synFrame, movWidth, movHeight, miniSeconds,nowFrame, lastFrame, count, howmuch;
    
    bool toggleImage, showString, bPause, xAdjust, yAdjust, bSelect, maximum, moveall, iftcp;
    
    #ifdef _IMAGE_
    ofImage twoScreenImage;
    #else
    ofVideoPlayer 		fingerMovie;
    #endif
    
    ofxUDPManager udpConnection;
    ofxXmlSettings XML;
    
    string Server;
};

#endif
