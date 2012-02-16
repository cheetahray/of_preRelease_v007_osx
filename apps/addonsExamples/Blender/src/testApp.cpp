#include "testApp.h"
#define shiftX 0
#define shiftY 0
#define PORT 11999 
/**
 *
 *	testApp.cpp
 * 
 *  ofxProjectorBlend sample project
 *  
 *  Created by James George ( http://www.jamesgeorge.org ) Sep. 20 2010
 *  In collaboration with FlightPhase ( http://www.flightphase.com )
 *
 *  Copyright (c) 2010 
 */

//--------------------------------------------------------------
void testApp::setup()
{
    ofBackground(0, 0, 0);
    if( XML.loadFile("mySettings.xml") )
    {
		miniSeconds = XML.getValue("CONFIG:miniSeconds", 50);
        Server = XML.getValue("CONFIG:IP", "127.0.0.1");
        movWidth = XML.getValue("CONFIG:x", 1024);
        movHeight = XML.getValue("CONFIG:y", 768);
        #ifdef _IMAGE_
        twoScreenImage.loadImage(XML.getValue("CONFIG:file", "adam.png"));
        #else
        fingerMovie.loadMovie(XML.getValue("CONFIG:file", "lastday_left.avi"));
        #endif
        iftcp = XML.getValue("CONFIG:iftcp", true);
	}
    else
    {
		miniSeconds = 50;
        Server = "127.0.0.1";
        movWidth = 1024;
        movHeight = 768;
        #ifdef _IMAGE_
        twoScreenImage.loadImage("adam.png");
        #else
        fingerMovie.loadMovie("lastday_left.avi");
        #endif
        iftcp = false;
	}
	blender.setup(movWidth, movHeight, PROJECTOR_COUNT, PIXEL_OVERLAP); 
	blender.gamma = .5;
	blender.blendPower = 1;
	blender.luminance = 0;
    blender.loadFromXml("fboSettings.xml");
    
    //guiIn   = ofRectangle(50 + movWidth, 40, 500, 178);
	guiIn   = ofRectangle(shiftX, shiftY, movWidth, movHeight);
    //guiOut  = ofRectangle(guiIn.x + guiIn.width + 30, 40 + guiIn.height + 50, 500, 178);
	guiOut  = ofRectangle(shiftX, shiftY, movWidth, movHeight);
	
	lastFrame = nowFrame = index[0] = index[1] = connectTime = deltaTime = 0;
    bSelect = bPause = showString = xAdjust = yAdjust = toggleImage = maximum = moveall = false;
    
	if(true == iftcp)
    {
        firstTime = 0;
        weConnected = tcpClient.setup(Server, PORT);
        tcpClient.setVerbose(true);
    }
    #ifndef _IMAGE_
    else
    {
        fingerMovie.play();
        fingerMovie.setLoopState(OF_LOOP_NORMAL);
    }
    #endif
}

//--------------------------------------------------------------
void testApp::update()
{
    if(true == iftcp)
	{
		if(weConnected)
		{
#ifndef _IMAGE_
            fingerMovie.idleMovie();
#endif
            
            nowFrame = fingerMovie.getCurrentFrame();
			if(0 == firstTime)
            {
                tcpClient.send("1");
            }
            else if(1 == firstTime)
            {
                if( true == (nowFrame < lastFrame) )
                {
                    fingerMovie.setPaused(true);
                    tcpClient.send("1");
                    firstTime = 2;
                }
            }
            
            //if data has been sent lets update our text
			string str = tcpClient.receive();
			
			if( str.length() > 0 )
            {
				
				if(5 == ofToInt(str))
                {
                    ofSleepMillis(miniSeconds);
                    
                    if(0 == firstTime)
                    {
                        
                        fingerMovie.play();
                        fingerMovie.setLoopState(OF_LOOP_NONE);
                    }
                    else
                        fingerMovie.setPaused(false);
                    
                    firstTime = 1;
                    
                }
                
			}
            lastFrame = nowFrame;
		}
		else
		{
            
			//if we are not connected lets try and reconnect every 5 seconds
            
            deltaTime = ofGetElapsedTimeMillis() - connectTime;
            
			if( deltaTime > 10000 ){
				weConnected = tcpClient.setup(Server, PORT);
				connectTime = ofGetElapsedTimeMillis();
			}
            
		}
        
	}
    #ifndef _IMAGE_
    else
        fingerMovie.idleMovie();
    #endif
}

//--------------------------------------------------------------
void testApp::draw(){

	if(true == weConnected || false == iftcp)
    {

        //call blender.begin() to draw onto the blendable canvas
        blender.begin();
	
        #ifdef _IMAGE_
        twoScreenImage.draw(shiftX, shiftY, movWidth, movHeight);
        #else
        fingerMovie.draw( shiftX, shiftY, movWidth, movHeight);
        #endif
        
        //call when you are finished drawing
        blender.end();
	
        //this draws to the main window
        blender.drawOutputDiagnostically(guiOut.x, guiOut.y, guiOut.width, guiOut.height, toggleImage);
        
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

}

void testApp::xxyy(int key)
{
	if(0 != index[0])
	{
		if(0 != index[1])
		{
			blender.activePointOut = 2;
			blender.activeScreenOut = (index[1] - 1) * divideH + index[0] - 1;
		}
		else 
		{
			blender.activePointOut = 3;
			blender.activeScreenOut = index[0] - 1;
		}
	}
	else
	{
		if(0 != index[1])
		{
			blender.activePointOut = 1;
			blender.activeScreenOut = (index[1] - 1) * divideH;
		}
		else
		{
			blender.activePointOut = 0;
			blender.activeScreenOut = 0;
		}
	}
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
    switch(key)
	{
        case 'a':
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
            blender.saveToXml();
            break;
        case 'r':
            blender.reloadFromXml();
            break;
        case 'c':
            blender.resetCoordinates();
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
                blender.keyPressOutputPoint(key, xAdjust, yAdjust, maximum, false, moveall); 
            }
            break;
        case ' ':
            blender.showBlend = !blender.showBlend;
            break;
        case 'g':
            blender.gamma  -= .05;
            blender.gamma2 -= .05;
            break;
        case 'G':
            blender.gamma  += .05;
            blender.gamma2 += .05;
            break;
        case 'l':
            blender.luminance  -= .05;
            blender.luminance2 -= .05;
            break;
        case 'L':
            blender.luminance  += .05;
            blender.luminance2 += .05;
            break;
        case 'p':
            blender.blendPower  -= .05;
            blender.blendPower2 -= .05;
            break;
        case 'P':
            blender.blendPower  += .05;
            blender.blendPower2 += .05;
            break;
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    if(true == toggleImage && true == bSelect)
	{
		//rm.mouseDragInputPoint(guiIn, ofPoint(x, y));
		blender.mouseDragOutputPoint(guiOut, ofPoint(x, y), xAdjust, yAdjust, maximum, false, moveall);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(true == toggleImage)
    {
        //if( !rm.mouseSelectInputPoint(guiIn, ofPoint(x, y)) )
        bSelect = blender.mouseSelectOutputPoint(guiOut, ofPoint( x, y ));
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if(true == toggleImage && true == bSelect)
	{
		switch(blender.activePointOut)
		{
            case 3:
                index[0] = blender.activeScreenOut + 1;
                index[1] = 0;
                break;
            case 2:
                index[0] = blender.activeScreenOut % divideH + 1;
                index[1] = blender.activeScreenOut / divideH + 1;
                break;
            case 1:
                index[0] = 0;
                index[1] = blender.activeScreenOut / divideH + 1;
                break;
            case 0:
                index[0] = 0;
                index[1] = 0;
                break;
		}
		bSelect = false;
	}
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

