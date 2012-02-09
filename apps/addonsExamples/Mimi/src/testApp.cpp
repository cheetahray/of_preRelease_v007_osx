#include "testApp.h"

#define shiftX 0
#define shiftY 0
#define SERVER "127.0.0.1"
#define PORT 11999 

//--------------------------------------------------------------
void testApp::setup(){
	
    rm.allocateForNScreens(movWidth, movHeight);
    rm.loadFromXml("fboSettings.xml");

    //guiIn   = ofRectangle(50 + movWidth, 40, 500, 178);
	guiIn   = ofRectangle(shiftX, shiftY, movWidth, movHeight);
    //guiOut  = ofRectangle(guiIn.x + guiIn.width + 30, 40 + guiIn.height + 50, 500, 178);
	guiOut  = ofRectangle(shiftX, shiftY, movWidth, movHeight);
	
	index[0] = index[1] = connectTime = deltaTime = 0;
    bSelect = bPause = showString = xAdjust = yAdjust = toggleImage = maximum = moveall = false;
	
    #ifdef _IMAGE_
    twoScreenImage.loadImage("adam.png");
	#else
    fingerMovie.loadMovie("lastday_left.avi");
    fingerMovie.play();
    fingerMovie.setLoopState(OF_LOOP_NORMAL);
    #endif
    
}

//--------------------------------------------------------------
void testApp::update(){

	#ifndef _IMAGE_
	fingerMovie.idleMovie();
	#endif

}

//--------------------------------------------------------------
void testApp::draw(){

    rm.startOffscreenDraw();

    #ifdef _IMAGE_
    twoScreenImage.draw(shiftX, shiftY, movWidth, movHeight);
	#else
    ofSetColor(255, 255, 255);
    fingerMovie.draw( shiftX, shiftY, movWidth, movHeight);
    #endif

	rm.endOffscreenDraw();

    //rm.drawInputDiagnostically(guiIn.x, guiIn.y, guiIn.width, guiIn.height);
	rm.drawOutputDiagnostically(guiOut.x, guiOut.y, guiOut.width, guiOut.height, toggleImage);
	
	/*
     glPushMatrix();
        
		//glTranslatef(10, 300, 0);
		glTranslatef(0, 0, 0);
        
		ofSetColor(255, 255, 255, 255);
		//rm.drawScreen();
		
     glPopMatrix();
	*/

	if(true == showString)
	{
		char tmp[16];
		sprintf(tmp,"%d", synFrame); 
		ofDrawBitmapString(tmp, 710, 290);
		
		//ofDrawBitmapString("s - to save to xml   r - to reload from xml    c - reset coordinates    g -  draw open gl shapes\n", 10, 16);
	}
	
	if(true == toggleImage)
	{	

		ofSleepMillis(100);

		if(true == xAdjust)
		{
			ofSleepMillis(200);
		}
		
		if(true == yAdjust)
		{
			ofSleepMillis(200);
		}

		if(true == maximum || true == moveall)
		{
			ofSleepMillis(200);
		}
	}

}

void testApp::xxyy(int key)
{
	if(0 != index[0])
	{
		if(0 != index[1])
		{
			rm.activePointOut = 2;
			rm.activeScreenOut = (index[1] - 1) * divideH + index[0] - 1;
		}
		else 
		{
			rm.activePointOut = 3;
			rm.activeScreenOut = index[0] - 1;
		}
	}
	else
	{
		if(0 != index[1])
		{
			rm.activePointOut = 1;
			rm.activeScreenOut = (index[1] - 1) * divideH;
		}
		else
		{
			rm.activePointOut = 0;
			rm.activeScreenOut = 0;
		}
	}
		
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    switch(key)
	{
    case 'a':
        ofkey.isAlt = true;
    break;
	case 'g':
		bPause = !bPause;
        fingerMovie.setPaused(bPause);
        toggleImage = !toggleImage;
		break;
	case 'x':
		xAdjust = !xAdjust;
		break;
	case 'y':
		yAdjust = !yAdjust;
		break;
	case 'z':
		showString = xAdjust = yAdjust = maximum = moveall = false;
		break;
	case 'b':
        showString = !showString;
    	break;
	case 'm':
        maximum = !maximum;
    	break;
    case 'n':
        moveall = !moveall;
        break;
	case 's':
        rm.saveToXml();
    	break;
	case 'r':
        rm.reloadFromXml();
    	break;
	case 'c':
        rm.resetCoordinates();
    	break;
	case 360://Up        
		if( true == toggleImage )
		{
			index[0] = index[0]>0?--index[0]:index[0];
			xxyy(key);
		}
		break;
    case 361: //down
		if( true == toggleImage )
		{
			index[0] = index[0]<divideH?++index[0]:index[0]; 
			xxyy(key);
		}
		break;
    case 362://left
		if( true == toggleImage )
		{
			index[1] = index[1]>0?--index[1]:index[1]; 
			xxyy(key);
		}
		break;
    case 363://right
		if( true == toggleImage )
		{
			index[1] = index[1]<divideV?++index[1]:index[1]; 
			xxyy(key);
		}
		break;
    case 357://Up 
    case 359: //down
    case 356://left
    case 358://right
		if( true == toggleImage )
		{
			rm.keyPressOutputPoint(key, xAdjust, yAdjust, maximum, ofkey.isAltDown(), moveall); 
		}
		break;
    }

	
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
    switch(key)
	{
        case 'a':
            ofkey.isAlt = false;
            break;
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if(true == toggleImage && true == bSelect)
	{
		//rm.mouseDragInputPoint(guiIn, ofPoint(x, y));
		rm.mouseDragOutputPoint(guiOut, ofPoint(x, y), xAdjust, yAdjust, maximum, ofkey.isAltDown(), moveall);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
		if(true == toggleImage)
		{
			//if( !rm.mouseSelectInputPoint(guiIn, ofPoint(x, y)) )
			bSelect = rm.mouseSelectOutputPoint(guiOut, ofPoint( x, y ));
		}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
	if(true == toggleImage && true == bSelect)
	{
		switch(rm.activePointOut)
		{
		case 3:
			index[0] = rm.activeScreenOut + 1;
			index[1] = 0;
			break;
		case 2:
			index[0] = rm.activeScreenOut % divideH + 1;
			index[1] = rm.activeScreenOut / divideH + 1;
			break;
		case 1:
			index[0] = 0;
			index[1] = rm.activeScreenOut / divideH + 1;
			break;
		case 0:
			index[0] = 0;
			index[1] = 0;
			break;
		}
		bSelect = false;
	}

}
