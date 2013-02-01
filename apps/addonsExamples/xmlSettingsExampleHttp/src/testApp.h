#pragma once

#include "ofMain.h"
#include "ofxHttpUtils.h"
#include "ofxXmlSettings.h"

#define NUM_PTS 800

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void newResponse(ofxHttpResponse & response);
    
        ofxHttpUtils httpUtils;
        int counter;
        string responseStr;
        string requestStr;
        string action_url;

		ofxXmlSettings XML;
		ofTrueTypeFont TTF;

        //string xmlStructure;
		string message, photo, text;
        /*
		ofPoint dragPts[NUM_PTS];

		int pointCount;
		int lineCount;
		int lastTagNumber;

		float red;
		float green;
		float blue;*/
};

