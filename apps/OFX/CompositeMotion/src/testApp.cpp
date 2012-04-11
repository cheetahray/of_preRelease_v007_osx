#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//ofSetVerticalSync(true);
	ofBackground(0, 0, 0);
	
	ofSetWindowTitle("ofxCompositeMotion");
	
	ball = Ball(0 , 0);
	
	motion = Position2DMotion(true, false, ofGetWindowWidth()/2, ofGetWindowHeight()/2);
	colorize = ColorizeMotion(true, false, 255.0, 255.0, 255.0);
	
	motion.properties(&ball.x, &ball.y);
	colorize.properties(&ball.r, &ball.b, &ball.g);
	
	colorize.endAt(200.0, 140.0, 90.0);
	
	composite.add(&motion);
	composite.add(&colorize);
	
	composite.update(ofGetElapsedTimeMillis());
}

//--------------------------------------------------------------
void testApp::update(){
	composite.update(ofGetElapsedTimeMillis());
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0, 0, 0);
	ofEnableSmoothing();
	
	//ofSetLineWidth(1.0);
	ofSetCircleResolution(100);
	
	ball.draw();
	
	ofSetColor(200, 140, 90);
	ofFill();
	ofEllipse(ofGetWindowWidth()/2, ofGetWindowHeight()/2 + 100, 50, 50);
	
	ofDisableSmoothing();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}