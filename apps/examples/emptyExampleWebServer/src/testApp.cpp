#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	server.start("httpdocs");
	server.addHandler(this, "/actions");
	verdana30.loadFont("verdana.ttf", 12, true, true);
	verdana30.setLineHeight(34.0f);
	verdana30.setLetterSpacing(1.035);
}


void testApp::httpGet(string url) {
	colorString = getRequestParameter("str");
}

void testApp::httpPost(string url, char *data, int dataLength) {
	if(result.empty() == true)
	{
		colorString.assign(data+strlen("str="));
		ofStringReplace(colorString, "%20", " %");
		result = ofSplitString(colorString,"%");
		result.erase (result.begin()+(result.size()-1));
		ofStringReplace(colorString, "%", "\n");
		size_t found = colorString.find_last_of(" ");
		if (found!=string::npos)
		{
			colorString = colorString.substr(0,found);
		}
		httpResponse(data);
		//httpResponse(colorString);
	}
}

//--------------------------------------------------------------
void testApp::update(){
	if(result.empty() == false)
	{
		if(serial.setup("\\\\.\\COM12",9600))
		{
			vector<string>::iterator it;
			string delayy = result.front();
			int raydelay = ofToInt(delayy);
			for ( it=result.begin() + 1 ; it < result.end(); it++ )
			{
				string rayray;
				rayray.append(delayy).append(*it);
				int raylen = strlen(rayray.data());
				serial.writeBytes( (unsigned char*)rayray.data(), raylen );
				ofSleepMillis(raydelay + (raylen << 1) );
			}
			result.clear();
			serial.close();
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	verdana30.drawString("??", 30, 30 );
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

