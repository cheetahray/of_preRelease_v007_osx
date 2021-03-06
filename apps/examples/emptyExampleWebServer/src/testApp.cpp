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
		httpResponse(colorString);
	}
}

//--------------------------------------------------------------
void testApp::update(){
	if(result.empty() == false)
	{
		if(serial.setup("\\\\.\\COM17",9600))
		{
			vector<string>::iterator it;
			string delayy = result.front();
			int raydelay = ofToInt(delayy);
			for ( it=result.begin() + 1 ; it < result.end(); it++ )
			{
				if( (*it).length() <= 1 )
				{
					break;
				}
				string rayray;
				rayray.append(delayy).append(*it);
				int raylen = strlen(rayray.data());
				serial.writeBytes( (unsigned char*)rayray.data(), raylen );
				ofSleepMillis(raydelay + (raylen * 5.5) );
			}
			result.clear();
			serial.close();
		}
	}
	else
	{
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		strftime (timebuffer[0],4,"%H",timeinfo);
		strftime (timebuffer[1],4,"%M",timeinfo);
		strftime (timebuffer[2],4,"%S",timeinfo);
		int opening = ofToInt(timebuffer[0]);
		int mm = ofToInt(timebuffer[1]);
		int ss = ofToInt(timebuffer[2]);
		if ( opening >= 11 && opening <= 18 && mm == 0 && ss < 30 )
		{
			colorString = "200 %000000000000001 %000000000001000 %000000000001000 %000000000000000 %000000000000100 %000000000010000 %000000000010000 %000000000000000 %000000000100000 %000000000010000 %000000000001000 %000000000000100 %000000000000001 %000001000001001 %000001000100001 %000000000000000 %000000000000001 %001000000001000 %001000000001000 %000000000000000 %000000000000100 %010000000010000 %010000000010000 %000000000000000 %100000000100000 %001000000001000 %000001000000001 %000001000000001 %001000000100000 %000000000100001 %000000000101000 %000000000000000 %000100000010000 %010000000010000 %000100000010000 %010000000010000 %000000000010000 %000000000001000 %000000000000100 %000000000010100 %000000000001000 %000000000101000 %000000000001001 %001000000101000 %000000000001000 %000000000000100 %000001000000001 %100001000100001 %000001000000001 %001000000001000 %001000000001000 %000000000000000 %000100000000100 %010000000010000 %010000000010000 %000000000000000 %100000000100000 %001000000001000 %000001000000001 %000001000000001 %100000000100000 %101000000101000 %100001000100001 %101001000101001 %000000000000001 %000000000000100 %000000000001000 %000000000010000 %000000000100000 %000000000001001 %000001000100000 %000000000000000 %000000000000001 %000000000001000 %000000000001000 %000000000101000 %000000000000100 %000000000010000 %000000000010000 %000000001010000 %000000000100000 %000000000010100 %000000000001001 %000000001000100 %000000000001001 %000000000001001 %000000000001001 %000000000000001 %000000000000001 %000000000001000 %000000000001000 %000000000001000 %000000000000100 %000000000010000 %000000000010000 %000000000010000 %000000000100000 %000000000001000 %000000000100001 %000000000001001 %000000000100000 %000000000000001 %000000000100001 %000000000100000 %000000000010000 %000000000010000 %000000000010000 %000000000010000 %000000000010000 %000000000001001 %000000001000100 %000000000000000 %000000000001000 %000000000001000 %000000000001000 %000000000001000 %000000000001000 %000000000010100 %000000000001001 %000000000000000 %000000000000001 %000000000101000 %000000000001001 %000000000000000 %000000000000100 %000000000010000 %000000001010000 %000000000000000 %000000000100000 %000000000001000 %000000000100001 %000000000001001 %000000000100000 %000000000000001 %000000000100000 %000000000000001 %000000000100000 %000000000000000 %000000000000001 %000000000000000 %000000000100000 %000000000101000 %000000000100001 %000000000101000 %100001000100000 %000000000000000 %000000000000000 %000000000000000 ";
			result = ofSplitString(colorString,"%");
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	verdana30.drawString("DoReMi", 20, 20);	
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

