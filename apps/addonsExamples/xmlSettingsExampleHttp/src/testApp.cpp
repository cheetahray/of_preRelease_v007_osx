#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0,0,0);

	action_url = "http://ftp.daf.org.tw/wishing.php";
	ofAddListener(httpUtils.newResponseEvent,this,&testApp::newResponse);
	httpUtils.start();
    
	//load a monospaced font
	//which we will use to show part of the xml structure
	TTF.loadFont("mono.ttf", 7);
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
    //we draw our status message at the top
	//ofSetColor(210, 90, 100);
	ofSetColor(240, 240, 240);
	TTF.drawString("Save settings to XML hit 's' key     status: "+message, 170, 12);
	TTF.drawString("wav file: "+photo, 170, 60);
	ofDrawBitmapString(requestStr,20,120);
	ofDrawBitmapString(responseStr,20,160);
}

//--------------------------------------------------------------
void testApp::newResponse(ofxHttpResponse & response){
	responseStr = (string)response.responseBody;
    
    //-----------
	//the string is printed at the top of the app
	//to give the user some feedback
	message = "loading mySettings.xml";
    
    //we load our settings file
	//if it doesn't exist we can still make one
	//by hitting the 's' key
	if( XML.loadFromBuffer(responseStr) ){ // parse string 
		message = "mySettings.xml loaded!";
	}else{
		message = "unable to load mySettings.xml check data/ folder";
	}
    
	//read the colors from XML
	//if the settings file doesn't exist we assigns default values (170, 190, 240)
    text    = XML.getValue("infotree:text", "none");
	photo	= XML.getValue("infotree:url", "none");
	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    ofxHttpForm form;
	form.action = action_url;
	form.method = OFX_HTTP_GET;
	//form.addFormField("number", ofToString(counter));
	//form.addFile("file","ofw-logo.gif");
	httpUtils.addForm(form);
	requestStr = "message sent: " + ofToString(counter);
	counter++;
    ofSleepMillis(1000);

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

