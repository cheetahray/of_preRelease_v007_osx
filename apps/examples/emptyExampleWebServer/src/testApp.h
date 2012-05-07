#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxWebServer.h"

class testApp : public ofBaseApp, public ofxWSRequestHandler {

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	ofxWebServer server;
	void httpGet(string url);
	void httpPost(string url, char *data, int dataLength);
	string colorString;
	vector<string> result;
	ofSerial	serial;
	ofTrueTypeFont	verdana30;
};

#endif
