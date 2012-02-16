/**
 *  ofxProjectorBlend
 *  (version 2.0)
 *
 * based on Paul Bourke's paper http://local.wasp.uwa.edu.au/~pbourke/texture_colour/edgeblend/
 * Original openFrameworks addon by James George, http://www.jamesgeorge.org
 * in collaboration with FlightPhase http://www.flightphase.com
 * additions by Marek Bereza, http://www.mazbox.com/
 * further additions by Jeff Crouse http://www.jeffcrouse.info
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------
 *
 * This class is an openFrameworks wrapper for this technique:
 * Original Core Image filter converted by James George ( http://www.jamesgeorge.org/ )
 * for ofxProjectorBlend addon for openFrameworks ( http://www.openframeworks.cc )
 *
 * Originally used in the Infiniti MOI project for projector blending ( http://vimeo.com/14588336 )
 * Also used for Coke24 project with Hellicar and Lewis ( http://www.hellicarandlewis.com/2011/03/22/coke-24hr-music/ )
 * and Flightphase's University of Dayton interactive wall ( http://www.flightphase.com/main_wp/expanded-media/interactive-wall-at-ud )
 *
 *
 */

#include "ofxProjectorBlend.h"
#define keyShift 1
int lastX, lastY;

ofxProjectorBlend::ofxProjectorBlend()
{
	showBlend = true;
#ifdef USE_XML_GUI
	gui = NULL;
#endif
	gamma = gamma2 = 0.5;
	blendPower = blendPower2 = 1;
	luminance = luminance2 = 0;
	numProjectors = 0;
	threshold = 0;
	shaderLocation = "../../../../../addons/rayProjectorBlend/assets/SmoothEdgeBlend";
    grid = new ofPoint[GRID_X * GRID_Y];
	coor = new ofPoint[GRID_X * GRID_Y];
    bInputSelected = false;
    
    activeScreenIn = activePointIn  = -1;
    
    activeScreenOut = activePointOut = lastY = lastX = 0;
    xmlFile = "";
    
    nScreens = divideH * divideV;
    
}

//----------------------------------------------------------
ofxProjectorBlend::~ofxProjectorBlend()
{
	delete grid;
	delete coor;
}

string ofxProjectorBlend::setShaderLocation(string _shaderLocation){
	shaderLocation = _shaderLocation;
	blendShader.load(shaderLocation);
}

void ofxProjectorBlend::setup(int resolutionWidth, 
							  int resolutionHeight, 
							  int _numProjectors, 
							  int _pixelOverlap, 
							  ofxProjectorBlendLayout _layout, 
							  ofxProjectorBlendRotation _rotation)
{
    
    screenWidth  = (float)resolutionWidth / (float)divideV;
    screenHeight = (float)resolutionHeight / (float)divideH;

    //nScreens            = divide * divide;
    outputPositions     = new ofPoint*[nScreens];
    inputPositions      = new ofPoint*[nScreens];
    
    float w_d_3 = (float)resolutionWidth / (float)divideV;
    float h_d_3 = (float)resolutionHeight / (float)divideH;
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
    
	string l = "horizontal";
	if(layout==ofxProjectorBlend_Vertical) l = "vertical";
	
	string r = "normal";
	if(rotation==ofxProjectorBlend_RotatedLeft) r = "rotated left";
	else if(rotation==ofxProjectorBlend_RotatedRight) r = "rotated right";
	
	ofLog(OF_LOG_NOTICE, "ofxProjectorBlend: res: %d x %d * %d, overlap: %d pixels, layout: %s, rotation: %s\n", resolutionWidth, resolutionHeight, _numProjectors, _pixelOverlap, l.c_str(), r.c_str());
	numProjectors = _numProjectors;
	layout = _layout;
	rotation = _rotation;
	
	if(numProjectors <= 0){
		ofLog(OF_LOG_ERROR, "Cannot initialize with " + ofToString(this->numProjectors) + " projectors.");
		return;
	}
	
	//allow editing projector heights
	for(int i = 0; i < numProjectors; i++){
		projectorHeightOffset.push_back( 0 );
	}
	
	pixelOverlap = _pixelOverlap;
	
	if(rotation == ofxProjectorBlend_NoRotation) {
		singleChannelWidth = resolutionWidth;
		singleChannelHeight = resolutionHeight;
	}
	else {
		singleChannelWidth = resolutionHeight;
		singleChannelHeight = resolutionWidth;
	}
	
	if(layout == ofxProjectorBlend_Vertical) {
		fullTextureWidth = singleChannelWidth;
		fullTextureHeight = singleChannelHeight*numProjectors - pixelOverlap*(numProjectors-1);
	}
	else if(layout == ofxProjectorBlend_Horizontal) {
		fullTextureWidth = singleChannelWidth*numProjectors - pixelOverlap*(numProjectors-1);
		fullTextureHeight = singleChannelHeight;
	} else {
		ofLog(OF_LOG_ERROR, "ofxProjectorBlend: You have used an invalid ofxProjectorBlendLayout in ofxProjectorBlend::setup()");
		return;
	}
	
	
	displayWidth = resolutionWidth*numProjectors;
	displayHeight = resolutionHeight;
	
	fullTexture.allocate(fullTextureWidth, fullTextureHeight, GL_RGB, 4);
	blendShader.load(shaderLocation);
}

void ofxProjectorBlend::begin() {
	
	fullTexture.begin();
	
	ofPushStyle();
	ofClear(0,0,0,0);
}


float ofxProjectorBlend::getDisplayWidth() {
	return displayWidth;
}

float ofxProjectorBlend::getDisplayHeight() {
	return displayHeight;
}

void ofxProjectorBlend::moveDisplayVertical(unsigned int targetDisplay, int yOffset) {
	if(targetDisplay >= numProjectors){
		ofLog(OF_LOG_ERROR, "targetDisplay (" + ofToString(targetDisplay) + ") is invalid.");
		return;
	}
	
	projectorHeightOffset[targetDisplay] += yOffset;
}


/** This changes your app's window size to the correct output size */
void ofxProjectorBlend::setWindowToDisplaySize() {
	ofSetWindowShape(getDisplayWidth(), getDisplayHeight());
}

float ofxProjectorBlend::getCanvasWidth()
{
	return fullTextureWidth;
}

float ofxProjectorBlend::getCanvasHeight()
{
	return fullTextureHeight;
}


#ifdef USE_SIMPLE_GUI
void ofxProjectorBlend::addGuiPage()
{
	gui.addPage("Projector Blend");
	gui.addToggle("Show Blend", showBlend);
	gui.addSlider("Blend Power", blendPower, 0.0, 4.0);
	gui.addSlider("Gamma", gamma, 0.1, 4.0);
	gui.addSlider("Luminance", luminance, 0.0, 4.0);
	gui.addSlider("Blend Power 2", blendPower2, 0.0, 4.0);
	gui.addSlider("Gamma 2", gamma2, 0.1, 4.0);
	gui.addSlider("Luminance 2", luminance2, 0.0, 4.0);
	
	gui.page("Projector Blend").setXMLName("../../../ProjectorBlendSettings.xml");
}	

#endif

#ifdef USE_XML_GUI
ofxXmlGui *ofxProjectorBlend::getGui() {
	if(gui==NULL) {
		gui = new ofxXmlGui();
		gui->setup(0, 0, 200);
		gui->addToggle("Show Blend", showBlend);
		gui->addSlider("Blend Power", blendPower, 0, 4);
		gui->addSlider("Gamma", gamma, 0, 4);
		gui->addSlider("Luminance", luminance, 0, 4);
		gui->addSlider("Blend Power 2", blendPower2, 0, 4);
		gui->addSlider("Gamma 2", gamma2, 0, 4);
		gui->addSlider("Luminance 2", luminance2, 0, 4);
		gui->enableAutoSave("");
	}
	
	return gui;
}
#endif

void ofxProjectorBlend::end() {
	fullTexture.end();
		
	ofPopStyle();
}

void ofxProjectorBlend::updateShaderUniforms() {
	
	blendShader.setUniform1f("OverlapTop", 0.0f);
	blendShader.setUniform1f("OverlapLeft", 0.0f);
	blendShader.setUniform1f("OverlapBottom", 0.0f);
	blendShader.setUniform1f("OverlapRight", 0.0f);
	
	blendShader.setUniform1f("BlendPower", blendPower);
	blendShader.setUniform1f("SomeLuminanceControl", luminance);
	blendShader.setUniform3f("GammaCorrection", gamma, gamma, gamma);
	
	blendShader.setUniform1f("BlendPower2", blendPower2);
	blendShader.setUniform1f("SomeLuminanceControl2", luminance2);
	blendShader.setUniform3f("GammaCorrection2", gamma2, gamma2, gamma2);
	
	blendShader.setUniform1f("projectors", this->numProjectors);
	blendShader.setUniform1f("threshold", threshold);
}

//----------------------------------------------------------
void ofxProjectorBlend::setPoint(int which, float x, float y){
    
	if( which>4 || which < 0){
		printf("ofTextureAdv:: please choose point 0-3\n");
		return;
	}
    
	quad[which].set(x, y, 0);
    
	updatePoints();
}

//----------------------------------------------------------
void ofxProjectorBlend::setPoints(ofPoint outputPts[pointNum]){
	for (int i = 0; i < pointNum; i++){
	    quad[i].set(outputPts[i].x, outputPts[i].y, 0);
	}
	updatePoints();
}

//----------------------------------------------------------
void ofxProjectorBlend::setPoints(ofPoint inputPts[pointNum], ofPoint outputPts[pointNum]){
	for (int i = 0; i < pointNum; i++){
	    quad[i].set(outputPts[i].x, outputPts[i].y, 0);
	    utQuad[i].set(inputPts[i].x, inputPts[i].y, 0);
	}
	updatePoints();
}

//----------------------------------------------------------
void ofxProjectorBlend::updatePoints(){
    
	int gridSizeX = GRID_X;
	int gridSizeY = GRID_Y;
    
	float xRes = 1.0/(gridSizeX-1);
	float yRes = 1.0/(gridSizeY-1);
    
	//GLfloat offsetw = 1.0f/(tex_w);
	//GLfloat offseth = 1.0f/(tex_h);
    
	//this is because we want pct to go from offsetw to texData.tex_t - offsetw
	GLfloat texTAdj = fullTexture.getTextureReference().texData.tex_t;
	GLfloat texUAdj = fullTexture.getTextureReference().texData.tex_u;
    GLboolean bFlipTexture = fullTexture.getTextureReference().texData.bFlipTexture;
	//printf("%f %f \n", texData.tex_t, texData.tex_u);
    
	for(int y = 0; y < gridSizeY; y++){
		for(int x = 0; x < gridSizeX; x++){
            
			float pctx = (float)x * xRes;
			float pcty = (float)y * yRes;
            
			float linePt0x = (1-pcty)*quad[0].x + pcty * quad[3].x;
			float linePt0y = (1-pcty)*quad[0].y + pcty * quad[3].y;
			float linePt1x = (1-pcty)*quad[1].x + pcty * quad[2].x;
			float linePt1y = (1-pcty)*quad[1].y + pcty * quad[2].y;
            float ptx 	   = (1-pctx) * linePt0x + pctx * linePt1x;
			float pty 	   = (1-pctx) * linePt0y + pctx * linePt1y;
            
            float utPt0x = (1-pcty)*utQuad[0].x + pcty * utQuad[3].x;
			float utPt0y = (1-pcty)*utQuad[0].y + pcty * utQuad[3].y;
			float utPt1x = (1-pcty)*utQuad[1].x + pcty * utQuad[2].x;
			float utPt1y = (1-pcty)*utQuad[1].y + pcty * utQuad[2].y;
            float tt 	   = (1-pctx) * utPt0x + pctx * utPt1x;
			float uu 	   = (1-pctx) * utPt0y + pctx * utPt1y;
            
			grid[y*gridSizeX + x].set(ptx, pty, 0);
			coor[y*gridSizeX + x].set( (tt * texTAdj), bFlipTexture ? texUAdj - (uu * texUAdj) : (uu * texUAdj), 0);
		}
	}
    
}

void ofxProjectorBlend::draw(float x, float y) {
    
    int gridSizeX = GRID_X;
	int gridSizeY = GRID_Y;
    
	ofSetHexColor(0xFFFFFF);
	glPushMatrix();
	glTranslatef(x, y, 0);
	if(showBlend) {
		blendShader.begin();
		blendShader.setUniform1f("width", singleChannelWidth);
		blendShader.setUniform1f("height", singleChannelHeight);
		
		updateShaderUniforms();
		/*
		if(layout == ofxProjectorBlend_Horizontal) {
			blendShader.setUniform1f("OverlapRight", pixelOverlap);	
		}
		else {
			blendShader.setUniform1f("OverlapTop", pixelOverlap);
		}
		*/
        //blendShader.setUniform1f("OverlapRight", pixelOverlap);	
        //blendShader.setUniform1f("OverlapTop", pixelOverlap);
        blendShader.setUniform1f("OverlapLeft", pixelOverlap);
        //blendShader.setUniform1f("OverlapBottom", pixelOverlap);
		blendShader.setUniformTexture("Tex0", fullTexture.getTextureReference(), 0);
		
		
		ofVec2f offset(0,0);
		glPushMatrix();
		
		// loop through each projector and glTranslatef() to its position and draw.
		for(int i = 0; i < numProjectors; i++) {
			blendShader.setUniform2f("texCoordOffset", offset.x, offset.y);
			/*
			if(i==1) {
				// set the first edge
				if(layout == ofxProjectorBlend_Horizontal) {
					blendShader.setUniform1f("OverlapLeft", pixelOverlap);	
				}
				else {
					blendShader.setUniform1f("OverlapBottom", pixelOverlap);
				}
				
			}
			// if we're at the end of the list of projectors, turn off the second edge's blend
			
            if(i+1 == numProjectors) {
				if(layout == ofxProjectorBlend_Horizontal) {
					blendShader.setUniform1f("OverlapRight", 0);	
				}
				else {
					blendShader.setUniform1f("OverlapTop", 0);
				}
			}
			*/
			glPushMatrix(); {
				if(rotation == ofxProjectorBlend_RotatedRight) {
					glRotatef(90, 0, 0, 1);
					glTranslatef(0, -singleChannelHeight, 0);
				}
				else if(rotation == ofxProjectorBlend_RotatedLeft) {
					glRotatef(-90, 0, 0, 1);
					glTranslatef(-singleChannelWidth, 0, 0);
				}
				
				glTranslatef(0, (float)projectorHeightOffset[i], 0);
				
				glBegin(GL_QUADS);
				
				for(int y = 0; y < gridSizeY-1; y++)
                {
                    for(int x = 0; x < gridSizeX-1; x++)
                    {
                        
                        int pt0 = x + y*gridSizeX;
                        int pt1 = (x+1) + y*gridSizeX;
                        int pt2 = (x+1) + (y+1)*gridSizeX;
                        int pt3 = x + (y+1)*gridSizeX;
                        
                        glTexCoord2f(coor[pt0].x, coor[pt0].y );
                        glVertex2f(  grid[pt0].x, grid[pt0].y);
                        
                        glTexCoord2f(coor[pt1].x, coor[pt1].y );
                        glVertex2f(  grid[pt1].x, grid[pt1].y);
                        
                        glTexCoord2f(coor[pt2].x, coor[pt2].y );
                        glVertex2f(  grid[pt2].x, grid[pt2].y);
                        
                        glTexCoord2f(coor[pt3].x, coor[pt3].y );
                        glVertex2f(  grid[pt3].x, grid[pt3].y);
                        
                    }
                }
				
				glEnd();
			}
			glPopMatrix();
			
			// move the texture offset and where we're drawing to.
			if(layout == ofxProjectorBlend_Horizontal) {
				offset.x += singleChannelWidth - pixelOverlap;
			}
			else {
				offset.y += singleChannelHeight - pixelOverlap;
				
			}
			
			
			if(rotation == ofxProjectorBlend_RotatedLeft || rotation == ofxProjectorBlend_RotatedRight) {
				glTranslatef(singleChannelHeight, 0, 0);
			}
			else {
				glTranslatef(singleChannelWidth, 0, 0);
			}
			
		}
		glPopMatrix();
	
		blendShader.end();
	} else {
		fullTexture.draw(x, y);
	}
	glPopMatrix();
}

void ofxProjectorBlend::drawOutputDiagnostically(float x, float y, float w, float h, bool toggleImage){
    if( nScreens <= 0 ) return;
    
    glPushMatrix();
    glTranslatef(x, y, 0);
    
    ofNoFill();
    ofSetColor(0,0,0);
    ofRect(0, 0, w, h);
    ofFill();
    
    for (int i = 0; i < nScreens; i++){
        
        glPushMatrix();
        
        float wScale = ( w / screenWidth ) / divideV ;
        float hScale = (h / screenHeight ) / divideH ;
        glScalef(wScale, hScale, 1);
        drawScreen(i);
        if(true == toggleImage)
        {
            ofSetColor(68,68,204);
            ofNoFill();
            ofBeginShape();
            for (int j = 0; j < 4; j++){
                ofVertex( outputPositions[i][j].x  , outputPositions[i][j].y );
            }
            ofEndShape(true);
            
            ofFill();
            for (int j = 0; j < 4; j++) {
                ofSetColor(255,51,51);
                //if(activeScreenOut >= 0 && activePointOut >= 0)
                {
                    if(outputPositions[activeScreenOut][activePointOut].x == outputPositions[i][j].x && outputPositions[activeScreenOut][activePointOut].y == outputPositions[i][j].y)
                    {
                        ofSetColor(51, 68, 255);
                    }
                }
                ofCircle(  outputPositions[i][j].x , outputPositions[i][j].y, CLICKRADIUS );
            }
            ofSetColor(255, 255, 255);
        }
        glPopMatrix();
    }
    
    glPopMatrix();
    
}

void ofxProjectorBlend::drawScreen(int nScreen){
    if( nScreen < 0 ) return;
	
	setPoints(inputPositions[nScreen], outputPositions[nScreen]);
	draw(0,0);
}

//---------------------------------------------------------------------------
void ofxProjectorBlend::resetCoordinates(){
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
void ofxProjectorBlend::loadFromXml(string xmlFileName){
    xmlFile = xmlFileName;
    xml.loadFile(xmlFile);
    reloadFromXml();
}

//---------------------------------------------------------------------------
void ofxProjectorBlend::reloadFromXml(){
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
void ofxProjectorBlend::saveToXml(){
    if(xmlFile != ""){
        saveToXml(xmlFile);
    }
}


void ofxProjectorBlend::saveToXml(string xmlFileName){
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
bool ofxProjectorBlend::mouseSelectOutputPoint(ofRectangle drawRect, ofPoint mousePoint, float clickRadius){
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
bool ofxProjectorBlend::mouseDragOutputPoint(ofRectangle drawRect, ofPoint mousePoint, bool xAdjust, bool yAdjust, bool maximum, bool isAlt, bool moveall)
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


bool ofxProjectorBlend::keyPressOutputPoint(int key, bool xAdjust, bool yAdjust, bool maximum, bool isAlt, bool moveall)
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

void ofxProjectorBlend::checkitout(int diffX, int diffY, bool xAdjust, bool yAdjust, bool maximum, bool isAlt, bool moveall)
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

void ofxProjectorBlend::singlePoint(int myPoint, int youScreen)
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