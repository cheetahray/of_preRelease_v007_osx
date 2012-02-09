#ifndef _TEST_APP
#define _TEST_APP


#define OF_ADDON_USING_OFXOPENCV
//#define OF_ADDON_USING_OFXOBJLOADER
//#define OF_ADDON_USING_OFXDIRLIST
#define OF_ADDON_USING_OFXVECTORMATH
//#define OF_ADDON_USING_OFXNETWORK
//#define OF_ADDON_USING_OFXVECTORGRAPHICS
//#define OF_ADDON_USING_OFXOSC
//#define OF_ADDON_USING_OFXTHREAD
#define OF_ADDON_USING_OFXXMLSETTINGS

//#define _IMAGE_ 
#define _TCP_

#ifdef _IMAGE_
#define movWidth 1024
#define movHeight 768
#else
#define movWidth 1280
#define movHeight 960
#endif

#include "ofMain.h"
//#include "ofAddons.h"
#include "renderManager.h"
#include "ofxNetwork.h"
#include "ofxKeyMap.h"

class testApp : public ofSimpleApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased (int key);

		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void xxyy(int key);
        
        renderManager rm;

        ofRectangle guiIn, guiOut;
		
		int index[2], firstTime, connectTime, deltaTime, synFrame;
		
        bool toggleImage, showString, bPause, xAdjust, yAdjust, bSelect, maximum, moveall, weConnected;
        
		#ifdef _IMAGE_
		ofImage twoScreenImage;
		#else
		ofVideoPlayer 		fingerMovie;
		#endif
		
		ofxKeyMap ofkey;
};

#endif

