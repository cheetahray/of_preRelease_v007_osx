#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//we run at 60 fps!
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

    //create the socket and bind to port 11999
	udpConnection.Create();
	udpConnection.Bind(12345);
    udpConnection.Connect("192.168.2.67",12344);
	udpConnection.SetNonBlocking(true);
    
    ofSetBackgroundAuto(false);
	ofBackground(255, 255, 255);
}

//--------------------------------------------------------------
void testApp::update(){

	char udpMessage[32];
	udpConnection.Receive(udpMessage,32);
    if(udpMessage[0] != 0)
    {
        /*string*/message=udpMessage;
    }
    /*
	if(message!=""){
		stroke.clear();
		float x,y;
		vector<string> strPoints = ofSplitString(message,"[/p]");
		for(int i=0;i<strPoints.size();i++){
			vector<string> point = ofSplitString(strPoints[i],"|");
			if( point.size() == 2 ){
				x=atof(point[0].c_str());
				y=atof(point[1].c_str());
				stroke.push_back(ofPoint(x,y));
			}
		}
	}
    */
}

//--------------------------------------------------------------
void testApp::draw(){
    ofFill();
    ofSetHexColor(0xFFFFFF);
    ofRect(0,0,200,30);
	ofSetHexColor(0x101010);
	ofDrawBitmapString(/*"UDP Receiver Example "*/message, 10, 20);
    /*
	for(int i=1;i<stroke.size();i++){
		ofLine(stroke[i-1].x,stroke[i-1].y,stroke[i].x,stroke[i].y);
	}*/
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    udpConnection.Send("Hello",strlen("Hello"));
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    char *ray = "101001011001100101011001100101";
    udpConnection.Send(ray,strlen(ray));
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
