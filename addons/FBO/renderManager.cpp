#include "renderManager.h"

#define keyShift 1

int lastX, lastY;
//---------------------------------------------------------------------------
renderManager::renderManager(){
    bInputSelected = false;

    activeScreenIn = -1;
    activePointIn  = -1;

    activeScreenOut = 0;
    activePointOut  = 0;
	lastY = lastX = 0;
    xmlFile = "";

    nScreens = divideH * divideV;
}

//---------------------------------------------------------------------------
void renderManager::allocateForNScreens(int renderWidth, int renderHeight){
    
	width   = renderWidth;
    height  = renderHeight;

    screenWidth  = (float)width / (float)divideV;
    screenHeight = (float)height / (float)divideH;

    myOffscreenTexture.allocate(renderWidth, renderHeight, false);

    //nScreens            = divide * divide;
    outputPositions     = new ofPoint*[nScreens];
    inputPositions      = new ofPoint*[nScreens];

    float w_d_3 = (float)width / (float)divideV;
    float h_d_3 = (float)height / (float)divideH;
    float inner_d_3 = 1.0f / (float)divideV;
    float height_d_3 = 1.0f / (float)divideH;
    for (int i = 0; i < divideV; i++){
	    for (int j = 0; j < divideH; j++){

			inputPositions[i*divideH+j]  = new ofPoint[pointNum];
			outputPositions[i*divideH+j] = new ofPoint[pointNum];

			//-----------------------------------------
			outputPositions[i*divideH+j][0].set(w_d_3*(i),h_d_3*(j),0);
			outputPositions[i*divideH+j][1].set(w_d_3*(i+1),h_d_3*(j),0);
			outputPositions[i*divideH+j][3].set(w_d_3*(i),h_d_3*(j+1),0);
			outputPositions[i*divideH+j][2].set(w_d_3*(i+1),h_d_3*(j+1),0);
			
			inputPositions[i*divideH+j][0].set(inner_d_3*(i),height_d_3*(j),0);
			inputPositions[i*divideH+j][1].set(inner_d_3*(i+1),height_d_3*(j),0);
			inputPositions[i*divideH+j][3].set(inner_d_3*(i),height_d_3*(j+1),0);
			inputPositions[i*divideH+j][2].set(inner_d_3*(i+1),height_d_3*(j+1),0);
			
//
//        printf("input is \n");
//        printf("(%f %f)          ", inputPositions[i][0].x, inputPositions[i][0].y);
//        printf("(%f %f)\n\n\n\n\n", inputPositions[i][1].x, inputPositions[i][1].y);
//        printf("(%f %f)          ", inputPositions[i][2].x, inputPositions[i][2].y);
//        printf("(%f %f)\n", inputPositions[i][3].x, inputPositions[i][3].y);
//
//        printf("\noutput is \n");
//        printf("(%f %f)          ", outputPositions[i][0].x, outputPositions[i][0].y);
//        printf("(%f %f)\n\n\n\n\n", inputPositions[i][1].x, outputPositions[i][1].y);
//        printf("(%f %f)          ", outputPositions[i][2].x, outputPositions[i][2].y);
//        printf("(%f %f)\n", outputPositions[i][3].x, outputPositions[i][3].y);
		}
    }
}

//---------------------------------------------------------------------------
void renderManager::startOffscreenDraw(){
    if( nScreens <= 0 ) return;

    myOffscreenTexture.swapIn();
    myOffscreenTexture.setupScreenForMe();
}

//---------------------------------------------------------------------------
void renderManager::endOffscreenDraw(){
    if( nScreens <= 0 ) return;

    myOffscreenTexture.swapOut();
    myOffscreenTexture.setupScreenForThem();
}

//---------------------------------------------------------------------------
void renderManager::drawInputDiagnostically(float x, float y, float w, float h){
   if( nScreens <= 0 ) return;

    myOffscreenTexture.draw(x, y, w,h);

	for (int i = 0; i < nScreens; i++){
        ofSetColor(0x33DD44);
        ofNoFill();
        ofBeginShape();
        for (int j = 0; j < 4; j++){
            ofVertex( inputPositions[i][j].x * w + x, inputPositions[i][j].y * h + y);
        }
        ofEndShape(true);

        ofFill();
        ofSetColor(0x3344FF);
        for (int j = 0; j < 4; j++){
            ofCircle(  inputPositions[i][j].x * w  + x, inputPositions[i][j].y * h + y, CLICKRADIUS);
        }
    }

}

//---------------------------------------------------------------------------
void renderManager::drawOutputDiagnostically(float x, float y, float w, float h, bool toggleImage){
    if( nScreens <= 0 ) return;

    glPushMatrix();
    glTranslatef(x, y, 0);

    ofNoFill();
        ofSetColor(0x000000);
        ofRect(0, 0, w, h);
    ofFill();

    for (int i = 0; i < nScreens; i++){

        glPushMatrix();

            ofSetColor(255, 255, 255, 255);
            float wScale = ( w / screenWidth ) / divideV ;
            float hScale = (h / screenHeight ) / divideH ;
            glScalef(wScale, hScale, 1);
            drawScreen(i);
			if(true == toggleImage)
			{
				ofSetColor(0x4444CC);
				ofNoFill();
				ofBeginShape();
				for (int j = 0; j < 4; j++){
					ofVertex( outputPositions[i][j].x  , outputPositions[i][j].y );
				}
				ofEndShape(true);

				ofFill();
				for (int j = 0; j < 4; j++) {
					ofSetColor(0xFF3333);
					//if(activeScreenOut >= 0 && activePointOut >= 0)
					{
						if(outputPositions[activeScreenOut][activePointOut].x == outputPositions[i][j].x && outputPositions[activeScreenOut][activePointOut].y == outputPositions[i][j].y)
						{
							ofSetColor(0x3344FF);
						}
					}
					ofCircle(  outputPositions[i][j].x , outputPositions[i][j].y, CLICKRADIUS );
				}
			}
        glPopMatrix();
    }

    glPopMatrix();

}

//---------------------------------------------------------------------------
void renderManager::drawScreen(){
    //if( nScreens <= 0 ) return;
	for(int i=0; i< nScreens; i++)
	{
		drawScreen(i);
	}
}

void renderManager::drawScreen(int nScreen){
    if( nScreen < 0 ) return;
	
	myOffscreenTexture.setPoints(inputPositions[nScreen], outputPositions[nScreen]);
	myOffscreenTexture.draw();
}

//---------------------------------------------------------------------------
void renderManager::resetCoordinates(){
    if( nScreens <= 0)return;

	float w_d_3 = screenWidth;
    float h_d_3 = screenHeight;
    float inner_d_3 = 1.0f / (float)divideV;
    float height_d_3 = 1.0f / (float)divideH;

    for (int i = 0; i < divideV; i++){
		for (int j = 0; j < divideH; j++){
		
			//-----------------------------------------
			outputPositions[i*divideH+j][0].set(w_d_3*(i),h_d_3*(j),0);
			outputPositions[i*divideH+j][1].set(w_d_3*(i+1),h_d_3*(j),0);
			outputPositions[i*divideH+j][3].set(w_d_3*(i),h_d_3*(j+1),0);
			outputPositions[i*divideH+j][2].set(w_d_3*(i+1),h_d_3*(j+1),0);
			
			inputPositions[i*divideH+j][0].set(inner_d_3*(i),height_d_3*(j),0);
			inputPositions[i*divideH+j][1].set(inner_d_3*(i+1),height_d_3*(j),0);
			inputPositions[i*divideH+j][3].set(inner_d_3*(i),height_d_3*(j+1),0);
			inputPositions[i*divideH+j][2].set(inner_d_3*(i+1),height_d_3*(j+1),0);
			
		}
    }
}

//---------------------------------------------------------------------------
void renderManager::loadFromXml(string xmlFileName){
    xmlFile = xmlFileName;
    xml.loadFile(xmlFile);
    reloadFromXml();
}

//---------------------------------------------------------------------------
void renderManager::reloadFromXml(){
    if(xmlFile == "")return;

	float w_d_3 = screenWidth;
    float h_d_3 = screenHeight;
    float inner_d_3 = 1.0f / (float)divideV;
    float height_d_3 = 1.0f / (float)divideH;

    for(int i = 0; i < divideV; i++){
		for(int j = 0; j < divideH; j++){

			//float inner_d_3 = 1.0 / nScreens;

			if( xml.pushTag("QUAD_IN_"+ofToString(i*divideH+j)) ){

					float inner_d_3 = 1.0 / divideV;
					float height_d_3 = 1.0 / divideH;

					inputPositions[i*divideH+j][0].x  = xml.getValue("pt0:x", inner_d_3*(i));
					inputPositions[i*divideH+j][0].y  = xml.getValue("pt0:y", height_d_3*(j));

					inputPositions[i*divideH+j][1].x  = xml.getValue("pt1:x", inner_d_3*(i+1));
					inputPositions[i*divideH+j][1].y  = xml.getValue("pt1:y", height_d_3*(j));

					inputPositions[i*divideH+j][2].x  = xml.getValue("pt2:x", inner_d_3*(i+1));
					inputPositions[i*divideH+j][2].y  = xml.getValue("pt2:y", height_d_3*(j+1));

					inputPositions[i*divideH+j][3].x  = xml.getValue("pt3:x", inner_d_3*(i));
					inputPositions[i*divideH+j][3].y  = xml.getValue("pt3:y", height_d_3*(j+1));

				xml.popTag();
			}
			if( xml.pushTag("QUAD_OUT_"+ofToString(i*divideH+j)) ){

					outputPositions[i*divideH+j][0].x  = xml.getValue("pt0:x", w_d_3*(i));
					outputPositions[i*divideH+j][0].y  = xml.getValue("pt0:y", h_d_3*(j));

					outputPositions[i*divideH+j][1].x  = xml.getValue("pt1:x", w_d_3*(i+1));
					outputPositions[i*divideH+j][1].y  = xml.getValue("pt1:y", h_d_3*(j));

					outputPositions[i*divideH+j][2].x  = xml.getValue("pt2:x", w_d_3*(i+1));
					outputPositions[i*divideH+j][2].y  = xml.getValue("pt2:y", h_d_3*(j+1));

					outputPositions[i*divideH+j][3].x  = xml.getValue("pt3:x", w_d_3*(i));
					outputPositions[i*divideH+j][3].y  = xml.getValue("pt3:y", h_d_3*(j+1));

				xml.popTag();
			}
		
		}
    }
}


//---------------------------------------------------------------------------
void renderManager::saveToXml(){
    if(xmlFile != ""){
        saveToXml(xmlFile);
    }
}


void renderManager::saveToXml(string xmlFileName){
    if(xmlFileName == "")return;

    //clear out the file
    xml.clear();

    for(int i = 0; i < nScreens; i++){

        //float inner_d_3 = 1.0 / divide;

        xml.addTag("QUAD_IN_"+ofToString(i) );
        if( xml.pushTag("QUAD_IN_"+ofToString(i)) ){

                //float inner_d_3 = 1.0 / nScreens;

                xml.setValue("pt0:x", inputPositions[i][0].x);
                xml.setValue("pt0:y", inputPositions[i][0].y);

                xml.setValue("pt1:x", inputPositions[i][1].x);
                xml.setValue("pt1:y", inputPositions[i][1].y);

                xml.setValue("pt2:x", inputPositions[i][2].x);
                xml.setValue("pt2:y", inputPositions[i][2].y);

                xml.setValue("pt3:x",  inputPositions[i][3].x);
                xml.setValue("pt3:y",  inputPositions[i][3].y);

            xml.popTag();
        }

        xml.addTag("QUAD_OUT_"+ofToString(i) );
        if( xml.pushTag("QUAD_OUT_"+ofToString(i)) ){

                xml.setValue("pt0:x", outputPositions[i][0].x);
                xml.setValue("pt0:y", outputPositions[i][0].y);

                xml.setValue("pt1:x", outputPositions[i][1].x);
                xml.setValue("pt1:y", outputPositions[i][1].y);

                xml.setValue("pt2:x", outputPositions[i][2].x);
                xml.setValue("pt2:y", outputPositions[i][2].y);

                xml.setValue("pt3:x", outputPositions[i][3].x);
                xml.setValue("pt3:y", outputPositions[i][3].y);

            xml.popTag();
        }
    }

    xml.saveFile(xmlFileName);
}

//---------------------------------------------------------------------------
bool renderManager::mouseSelectInputPoint(ofRectangle drawRect, ofPoint mousePoint, float clickRadius){
    if( nScreens <= 0 ) return false;

    //this is not the best way
    activeScreenIn      = -1;
    activePointIn       = -1;
    //activeScreenOut     = -1;
    //activePointOut      = -1;

    float smallestDist  = 999999;

    bool bFound = false;

	for (int i = 0; i < nScreens; i++){
        for (int j = 0; j < 4; j++){
            ofPoint inputPt;
            inputPt.x = inputPositions[i][j].x * drawRect.width + drawRect.x;
            inputPt.y = inputPositions[i][j].y * drawRect.height + drawRect.y;
            inputPt.z = 0;
            float len = sqrt( (inputPt.x - mousePoint.x) * (inputPt.x - mousePoint.x) +
                              (inputPt.y - mousePoint.y) * (inputPt.y - mousePoint.y));
            if (len < clickRadius && len < smallestDist){
                activeScreenIn = i;
                activePointIn  = j;
                smallestDist = len;

                bFound = true;
            }
        }
    }

    return bFound;
}

//---------------------------------------------------------------------------
bool renderManager::mouseDragInputPoint(ofRectangle drawRect, ofPoint mousePoint){
    if( nScreens <= 0 ) return false;

    if (activeScreenIn > -1 && activePointIn > -1){

        float xpct = (mousePoint.x - drawRect.x) / drawRect.width;
        float ypct = (mousePoint.y - drawRect.y) / drawRect.height;
        inputPositions[activeScreenIn][activePointIn].x = xpct;
        inputPositions[activeScreenIn][activePointIn].y = ypct;

        return true;
    }

    return false;
}

//---------------------------------------------------------------------------
bool renderManager::mouseSelectOutputPoint(ofRectangle drawRect, ofPoint mousePoint, float clickRadius){
    if( nScreens <= 0 ) return false;
	
	//this is not the best way
    activeScreenIn      = -1;
    activePointIn       = -1;
    //activeScreenOut     = -1;
    //activePointOut      = -1;

    float smallestDist  = 999999;
	ofPoint outputTry;
    bool bFound = false;

    for (int i = 0; i < nScreens; i++){
        for (int j = 0; j < 4; j++){
            ofPoint outputPt;

            //lets get the screen width and height at our scale
            outputPt.x = outputPositions[i][j].x;// * drawRect.width) + drawRect.x;
            outputPt.y = outputPositions[i][j].y;// * drawRect.height) + drawRect.y;
            outputPt.z = 0;

            outputPt.x *= ( (float)drawRect.width /  screenWidth ) / divideV ;
            outputPt.y *= ( (float)drawRect.height / screenHeight ) / divideH ;
			outputTry = outputPt;
            outputPt.x += drawRect.x;
            outputPt.y += drawRect.y;

            //printf("outputPt  is %f %f \n", outputPt.x, outputPt.y);

            float len = sqrt( (outputPt.x - mousePoint.x) * (outputPt.x - mousePoint.x) +
                              (outputPt.y - mousePoint.y) * (outputPt.y - mousePoint.y));
            if (len < clickRadius && len < smallestDist){
                activeScreenOut = i;
                activePointOut = j;
                smallestDist = len;
				
				lastX = outputPositions[activeScreenOut][activePointOut].x;
				lastY = outputPositions[activeScreenOut][activePointOut].y;
				//checkitout(outputTry.x-lastX, outputTry.y-lastY, xAdjust, yAdjust);

				//printf("setting point %i of screen %i \n", activePointOut, activeScreenOut);
				return true;
                //bFound = true;
            }
        }
    }
    return bFound;

}

//---------------------------------------------------------------------------
bool renderManager::mouseDragOutputPoint(ofRectangle drawRect, ofPoint mousePoint, bool xAdjust, bool yAdjust, bool maximum, bool isAlt, bool moveall)
{
    if( nScreens <= 0 ) return false;

    //if (activeScreenOut > -1 && activePointOut > -1)
	{

        float xpct = (mousePoint.x - drawRect.x) / (drawRect.width / divideV);
        float ypct = (mousePoint.y - drawRect.y) / (drawRect.height / divideH);

        float outPtX = xpct * screenWidth;
        float outPtY = ypct * screenHeight;

        outputPositions[activeScreenOut][activePointOut].x = outPtX;
        outputPositions[activeScreenOut][activePointOut].y = outPtY;

		checkitout(outPtX-lastX, outPtY-lastY, xAdjust, yAdjust, maximum, isAlt, moveall);
		//printf("point - %i  of screen %i setting output position to %f %f \n",activePointOut,  activeScreenOut, outputPositions[activeScreenOut][activePointOut].x, outputPositions[activeScreenOut][activePointOut].y);

        return true;
    }
    return false;
}


bool renderManager::keyPressOutputPoint(int key, bool xAdjust, bool yAdjust, bool maximum, bool isAlt, bool moveall)
{
    if( nScreens <= 0 ) return false;
	
	switch(key)
	{
		case 357://Up 
			outputPositions[activeScreenOut][activePointOut].y -= keyShift;
			checkitout(0,-1,xAdjust,yAdjust, maximum, isAlt, moveall);
		break;
		case 359: //down
			outputPositions[activeScreenOut][activePointOut].y += keyShift;
			checkitout(0,1,xAdjust,yAdjust, maximum, isAlt, moveall);
			break;
		case 356://left
	        outputPositions[activeScreenOut][activePointOut].x -= keyShift;
			checkitout(-1,0,xAdjust,yAdjust, maximum, isAlt, moveall);
			break;
		case 358://right
	        outputPositions[activeScreenOut][activePointOut].x += keyShift;
			checkitout(1,0,xAdjust,yAdjust, maximum, isAlt, moveall);
			break;
	}
        
		//printf("point - %i  of screen %i setting output position to %f %f \n",activePointOut,  activeScreenOut, outputPositions[activeScreenOut][activePointOut].x, outputPositions[activeScreenOut][activePointOut].y);

        return true;
    
}

void renderManager::checkitout(int diffX, int diffY, bool xAdjust, bool yAdjust, bool maximum, bool isAlt, bool moveall)
{
	int scrPlus1 = activeScreenOut+1;
	int plusDivide = activeScreenOut+divideH;
	
	int iii = activeScreenOut % divideH;
	int jjj = activeScreenOut / divideH;	
	int kkk = activeScreenOut % divideV;
	if(true == maximum && activeScreenOut == (nScreens - 1) && 2 == activePointOut)
	{
		float w_d_3 = (float) (outputPositions[nScreens-1][2].x - outputPositions[0][0].x) / (float)divideV;
		float h_d_3 = (float) (outputPositions[nScreens-1][2].y - outputPositions[0][0].y)  / (float)divideH;
    
		for (int i = 0; i < divideV; i++)
		{
			for (int j = 0; j < divideH; j++)
			{

				outputPositions[i*divideH+j][0].set(w_d_3*(i)+outputPositions[0][0].x,h_d_3*(j)+outputPositions[0][0].y,0);
				outputPositions[i*divideH+j][1].set(w_d_3*(i+1)+outputPositions[0][0].x,h_d_3*(j)+outputPositions[0][0].y,0);
				outputPositions[i*divideH+j][3].set(w_d_3*(i)+outputPositions[0][0].x,h_d_3*(j+1)+outputPositions[0][0].y,0);
				outputPositions[i*divideH+j][2].set(w_d_3*(i+1)+outputPositions[0][0].x,h_d_3*(j+1)+outputPositions[0][0].y,0);
			}
		}

	}
    else if(true == moveall)
    {
        for (int i = 0; i < divideV; i++)
		{
			for (int j = 0; j < divideH; j++)
			{
                for(int k = 0; k < 4; k++)
                {
                    if(activeScreenOut != i*divideH+j || k != activePointOut)
                    {
                        outputPositions[i*divideH+j][k].x += diffX;
                        outputPositions[i*divideH+j][k].y += diffY;
                    }
                }
                
			}
		}
        lastX = outputPositions[activeScreenOut][activePointOut].x;
		lastY = outputPositions[activeScreenOut][activePointOut].y;
    }
	else
	{
		// 'y'
		if(true == yAdjust)
		{
			bool is3 = false;
			
			for(int ii = 0; ii < divideV; ii++)
			{
				if(ii == jjj) 
				{

					switch(activePointOut)
					{
					case 0:
						if(true == isAlt)
						{
							is3 = true;
							outputPositions[activeScreenOut][1].y += (float)diffY * ( 1.0 - (float)( ii - jjj + 1) / (float)(divideV - jjj) );
						}
						else
						{
							outputPositions[activeScreenOut][1].y += diffY;
						}
						outputPositions[activeScreenOut][1].x += diffX;
						break;
					case 1:
						if(true == isAlt)
						{
							outputPositions[activeScreenOut][0].y += (float)diffY * ( (float)( ii ) / (float)(jjj+1) );
						}
						else
						{	
							outputPositions[activeScreenOut][0].y += diffY;
						}
						outputPositions[activeScreenOut][0].x += diffX;
						break;
					case 3:
						if(true == isAlt)
						{
							is3 = true;
							outputPositions[activeScreenOut][2].y += (float)diffY * ( 1.0 - (float)( ii - jjj + 1) / (float)(divideV - jjj) );
						}
						else
						{
							outputPositions[activeScreenOut][2].y += diffY;
						}
						outputPositions[activeScreenOut][2].x += diffX;
						if(iii != divideH - 1)
						{
							outputPositions[scrPlus1][1].y = outputPositions[activeScreenOut][2].y;
							outputPositions[scrPlus1][1].x = outputPositions[activeScreenOut][2].x;
						}
						break;
					case 2:
						if(true == isAlt)
						{
							outputPositions[activeScreenOut][3].y += (float)diffY * ( (float)( ii ) / (float)(jjj+1) );
						}
						else
						{
							outputPositions[activeScreenOut][3].y += diffY;
						}
						outputPositions[activeScreenOut][3].x += diffX;
						if(iii != divideH - 1)
						{
							outputPositions[scrPlus1][0].y = outputPositions[activeScreenOut][3].y;
							outputPositions[scrPlus1][0].x = outputPositions[activeScreenOut][3].x;
						}
						break;
					}

				}
				else
				{
					int iiii = ii * divideH + iii;
					
					if(activePointOut <= 1)
					{
						if(1 != activePointOut || iiii != plusDivide)
						{
							if(true == isAlt)
							{
								if(jjj > ii)
								{
									outputPositions[iiii][0].y += (float)diffY * ( (float)( ii ) / (float)(jjj+1) );
								}
								else
								{
									outputPositions[iiii][0].y += (float)diffY * ( 1.0 - (float)( ii - jjj - 1 + (is3?1:0) ) / (float)(divideV - jjj - 1 + (is3?1:0) ) );
								}
							}
							else
							{
								outputPositions[iiii][0].y += diffY;
							}
							outputPositions[iiii][0].x += diffX;
						}
						if(true == isAlt)
						{
							if(jjj > ii)
							{
								outputPositions[iiii][1].y += (float)diffY * ( (float)( ii + 1 ) / (float)(jjj+1) );
							}
							else if(ii != divideV - 1)
							{
								outputPositions[iiii][1].y += (float)diffY * ( 1.0 - (float)( ii - jjj + (is3?1:0) ) / (float)(divideV - jjj - 1 + (is3?1:0) ) );
							}
						}
						else
						{
							outputPositions[iiii][1].y += diffY;
						}
						outputPositions[iiii][1].x += diffX;
					}
					else 
					{	
						if(true == isAlt)
						{
							if(jjj > ii)
							{
								outputPositions[iiii][2].y += (float)diffY * ( (float)( ii + 1 ) / (float)(jjj+1) );
							}
							else if(ii != divideV - 1)
							{
								outputPositions[iiii][2].y += (float)diffY * ( 1.0 - (float)( ii - jjj + (is3?1:0) ) / (float)(divideV - jjj - 1 + (is3?1:0) ) );
							}
						}
						else
						{
							outputPositions[iiii][2].y += diffY;
						}
						outputPositions[iiii][2].x += diffX;
						
						if(2 != activePointOut || iiii != plusDivide)
						{
							if(true == isAlt)
							{
								if(jjj > ii)
								{
									outputPositions[iiii][3].y += (float)diffY * ( (float)( ii ) / (float)(jjj+1) );
								}
								else
								{
									outputPositions[iiii][3].y += (float)diffY * ( 1.0 - (float)( ii - jjj - 1 + (is3?1:0) ) / (float)(divideV - jjj - 1 + (is3?1:0) ) );
								}
							}
							else
							{
								outputPositions[iiii][3].y += diffY;
							}
							outputPositions[iiii][3].x += diffX;
						}

						if(iii != divideH - 1)
						{
							
							if(2 != activePointOut || iiii != plusDivide + 1)
							{
								outputPositions[iiii+1][0].y = outputPositions[iiii][3].y;
								outputPositions[iiii+1][0].x = outputPositions[iiii][3].x;
							}

							outputPositions[iiii+1][1].y = outputPositions[iiii][2].y;
							outputPositions[iiii+1][1].x = outputPositions[iiii][2].x;
						}
					}
				}
			}
		}

		// 'x'
		if(true == xAdjust)
		{
			int lll = jjj * divideH;
			int mmm = lll + divideH;
			bool is1 = false;
			
			for(int ii = lll; ii < mmm; ii++)
			{
				int iindex = ii - lll;
				int llll = ii + divideH;
				switch(activePointOut)
				{
				case 2:
					if(llll < nScreens)
					{
						if(llll != plusDivide + 1)
						{
							if(true == isAlt)
							{
								if(kkk >= iindex)
								{
									outputPositions[llll][0].x += (float)diffX * ( (float)( iindex ) / (float)( kkk + 1 ) );
								}
								else
								{
									outputPositions[llll][0].x += (float)diffX * ( 1.0 - (float)( iindex - kkk - 1 + (is1?1:0) ) / (float)(divideH - kkk - 1 + (is1?1:0) ) );
								}
							}
							else
							{
								outputPositions[llll][0].x += diffX;
							}
							outputPositions[llll][0].y += diffY;
						}
						if(llll != plusDivide)
						{
							if(true == isAlt)
							{
								if(kkk >= iindex)
								{
									outputPositions[llll][3].x += (float)diffX * ( (float)( iindex + 1 ) / (float)( kkk + 1 ) );
								}
								else if(iindex != divideH - 1)
								{
									outputPositions[llll][3].x += (float)diffX * ( 1.0 - (float)( iindex - kkk + (is1?1:0) ) / (float)(divideH - kkk - 1 + (is1?1:0) ) );
								}
							}
							else
							{
								outputPositions[llll][3].x += diffX;
							}	
							outputPositions[llll][3].y += diffY;
						}
					}
					if(ii != scrPlus1)
					{
						if(llll < nScreens)
						{
							outputPositions[ii][1].x = outputPositions[llll][0].x;
							outputPositions[ii][1].y = outputPositions[llll][0].y;
						}
						else
						{
							if(true == isAlt)
							{
								if(kkk >= iindex)
								{
									outputPositions[ii][1].x += (float)diffX * ( (float)( iindex ) / (float)( kkk + 1 ) );
								}
								else
								{
									//outputPositions[ii][1].x += (float)diffX * ( 1.0 - (float)( iindex - kkk - 1 + (is1?1:0) ) / (float)(divideH - kkk - 1 + (is1?1:0) ) );
									outputPositions[ii][1].x = outputPositions[ii-1][2].x;
								}
							}
							else
							{
								outputPositions[ii][1].x += diffX;
							}
							outputPositions[ii][1].y += diffY;
						}
					}
					if(ii != activeScreenOut)  
					{	
						if(llll < nScreens)
						{
							outputPositions[ii][2].x = outputPositions[llll][3].x;
							outputPositions[ii][2].y = outputPositions[llll][3].y;
						}
						else
						{
							if(true == isAlt)
							{
								if(kkk >= iindex)
								{
									outputPositions[ii][2].x += (float)diffX * ( (float)( iindex + 1 ) / (float)( kkk + 1 ) );
								}
								else if(iindex != divideH - 1)
								{
									outputPositions[ii][2].x += (float)diffX * ( 1.0 - (float)( iindex - kkk + (is1?1:0) ) / (float)(divideH - kkk - 1 + (is1?1:0) ) );
								}
							}
							else
							{
								outputPositions[ii][2].x += diffX;
							}	
							outputPositions[ii][2].y += diffY;
						}
					}
					break;
				case 3:
					if(ii != scrPlus1)
					{
						if(true == isAlt)
						{
							if(kkk >= iindex)
							{
								outputPositions[ii][0].x += (float)diffX * ( (float)( iindex ) / (float)( kkk + 1 ) );
							}
							else
							{
								outputPositions[ii][0].x = outputPositions[ii-1][3].x;
							}
						}
						else
						{
							outputPositions[ii][0].x += diffX;
						}	
						outputPositions[ii][0].y += diffY;
					}
					if(ii != activeScreenOut)  
					{	
						if(true == isAlt)
						{
							if(kkk >= iindex)
							{
								outputPositions[ii][3].x += (float)diffX * ( (float)( iindex + 1 ) / (float)( kkk + 1 ) );
							}
							else if(iindex != divideH - 1)
							{
								outputPositions[ii][3].x += (float)diffX * ( 1.0 - (float)( iindex - kkk + (is1?1:0) ) / (float)(divideH - kkk - 1 + (is1?1:0) ) );
							}
						}
						else
						{
							outputPositions[ii][3].x += diffX;
						}
						outputPositions[ii][3].y += diffY;
					}
					break;
				case 1:
					is1 = true;
					if(llll < nScreens)
					{
						if(llll != plusDivide)
						{
							if(true == isAlt)
							{
								if(kkk > iindex)
								{
									outputPositions[llll][0].x += (float)diffX * ( (float)( iindex ) / (float)( kkk + 1 ) );
								}
								else
								{
									outputPositions[llll][0].x += (float)diffX * ( 1.0 - (float)( iindex - kkk - 1 + (is1?1:0) ) / (float)(divideH - kkk - 1 + (is1?1:0) ) );
								}
							}
							else
							{
								outputPositions[llll][0].x += diffX;
							}
							outputPositions[llll][0].y += diffY;
						}
						if(true == isAlt)
						{
							if(kkk > iindex)
							{
								outputPositions[llll][3].x += (float)diffX * ( (float)( iindex + 1 ) / (float)( kkk + 1 ) );
							}
							else if(iindex != divideH - 1)
							{
								outputPositions[llll][3].x += (float)diffX * ( 1.0 - (float)( iindex - kkk + (is1?1:0) ) / (float)(divideH - kkk - 1 + (is1?1:0) ) );
							}
						}
						else
						{
							outputPositions[llll][3].x += diffX;
						}
						outputPositions[llll][3].y += diffY;
					}
					if(llll < nScreens)
					{
						outputPositions[ii][2].x = outputPositions[llll][3].x;
						outputPositions[ii][2].y = outputPositions[llll][3].y;
					}
					else
					{
						if(true == isAlt)
						{
							if(kkk > iindex)
							{
								outputPositions[ii][2].x += (float)diffX * ( (float)( iindex + 1 ) / (float)( kkk + 1 ) );
							}
							else if(iindex != divideH - 1)
							{
								outputPositions[ii][2].x += (float)diffX * ( 1.0 - (float)( iindex - kkk + (is1?1:0) ) / (float)(divideH - kkk - 1 + (is1?1:0) ) );
							}
						}
						else
						{
							outputPositions[ii][2].x += diffX;
						}
						outputPositions[ii][2].y += diffY;
					}
					if(ii != activeScreenOut)  
					{	
						if(llll < nScreens)
						{
							outputPositions[ii][1].x = outputPositions[llll][0].x;
							outputPositions[ii][1].y = outputPositions[llll][0].y;
						}
						else
						{
							if(true == isAlt)
							{
								if(kkk >= iindex)
								{
									outputPositions[ii][1].x += (float)diffX * ( (float)( iindex ) / (float)( kkk + 1 ) );
								}
								else
								{
									//outputPositions[ii][1].x += (float)diffX * ( 1.0 - (float)( iindex - kkk - 1 + (is1?1:0) ) / (float)(divideH - kkk - 1 + (is1?1:0) ) );
									outputPositions[ii][1].x = outputPositions[ii-1][2].x;
								}
							}
							else
							{
								outputPositions[ii][1].x += diffX;
							}
							outputPositions[ii][1].y += diffY;
						}
					}
					break;
				case 0:
					is1 = true;
					if(ii != activeScreenOut)  
					{	
						if(true == isAlt)
						{
							if(kkk > iindex)
							{
								outputPositions[ii][0].x += (float)diffX * ( (float)( iindex ) / (float)( kkk + 1 ) );
							}
							else
							{
								outputPositions[ii][0].x = outputPositions[ii-1][3].x;
							}
						}
						else
						{
							outputPositions[ii][0].x += diffX;
						}
						outputPositions[ii][0].y += diffY;
					}
					if(true == isAlt)
					{
						if(kkk > iindex)
						{
							outputPositions[ii][3].x += (float)diffX * ( (float)( iindex + 1 ) / (float)( kkk + 1 ) );
						}
						else if(iindex != divideH - 1)
						{
							outputPositions[ii][3].x += (float)diffX * ( 1.0 - (float)( iindex - kkk + (is1?1:0) ) / (float)(divideH - kkk - 1 + (is1?1:0) ) );
						}
					}
					else
					{
						outputPositions[ii][3].x += diffX;
					}
					outputPositions[ii][3].y += diffY;
					break;
				}

			}
		}
		
		singlePoint(activePointOut, activeScreenOut);

		lastX = outputPositions[activeScreenOut][activePointOut].x;
		lastY = outputPositions[activeScreenOut][activePointOut].y;
	}
}

void renderManager::singlePoint(int myPoint, int youScreen)
{

	int scrPlus1 = youScreen+1;
	int plusDivide = youScreen+divideH;
	int iii = activeScreenOut % divideH;
	
	switch(myPoint)
	{
		case 3:
					
			if( youScreen < divideH - 1)
			{
				outputPositions[scrPlus1][0].x = outputPositions[youScreen][myPoint].x;
				outputPositions[scrPlus1][0].y = outputPositions[youScreen][myPoint].y;
			}

		break;

		case 1:

			// Not 'x' 

			if( youScreen < nScreens - divideH )
			{
				outputPositions[plusDivide][0].x = outputPositions[youScreen][myPoint].x;
				outputPositions[plusDivide][0].y = outputPositions[youScreen][myPoint].y;
			}
					
		break;

		case 2:

			// Not 'x' 

			if( iii != (divideH-1) )
			{
				outputPositions[scrPlus1][1].x = outputPositions[youScreen][myPoint].x;
				outputPositions[scrPlus1][1].y = outputPositions[youScreen][myPoint].y;
			}
			if( youScreen < nScreens - divideH )
			{
				outputPositions[plusDivide][3].x = outputPositions[youScreen][myPoint].x;
				outputPositions[plusDivide][3].y = outputPositions[youScreen][myPoint].y;
				if( iii < divideH - 1)
				{
					outputPositions[plusDivide+1][0].x = outputPositions[youScreen][myPoint].x;
					outputPositions[plusDivide+1][0].y = outputPositions[youScreen][myPoint].y;
				}
			}

		break;
					
	}
}
