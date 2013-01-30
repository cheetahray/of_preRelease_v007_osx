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


#ifndef OFX_PROJECTOR_BLEND
#define OFX_PROJECTOR_BLEND

//Uncomment this to access the addGuiPage() method
//that will add a simple page to your SimpleGUI allowing
//control of the projector blend through the interface
//#define USE_SIMPLE_GUI

#include "ofMain.h"

#ifdef USE_SIMPLE_GUI
#include "ofxSimpleGuiToo.h"
#endif

#define pointNum 4
#define GRID_X 12
#define GRID_Y 8
#define divideH 1
#define divideV 1
#define CLICKRADIUS 5
#ifndef WIN32
#define USE_XML_GUI
#endif
#ifdef USE_XML_GUI
#include "ofxXmlGui.h"
#else
#include "ofxXmlSettings.h"
#endif

enum ofxProjectorBlendLayout {
	ofxProjectorBlend_Horizontal,
	ofxProjectorBlend_Vertical,
};

enum ofxProjectorBlendRotation {
	ofxProjectorBlend_NoRotation,
	ofxProjectorBlend_RotatedLeft,
	ofxProjectorBlend_RotatedRight
};

class ofxProjectorBlend
{
  public:
	ofxProjectorBlend();
	~ofxProjectorBlend();
	
	/**
	 * Set resolutionWidth and height to the resolutions of each projector output - if you're portrait, 
	 * still give it a landscape resolution.
	 *
	 * layout   - this is how the projectors are stacked, so if they are next to eachother, it's horizontal
	 *            and if they're on-top of eachother, it's vertical. (this is regardless of rotation)
	 * 
	 * rotation - this is the rotation of the projector. If the projectors are on their sides then it's 
	 *            rotatedRight, if it's the right way round, it's noRotation.
	 *
	 */
	void setup(int resolutionWidth, int resolutionHeight, int numProjectors, int pixelOverlap, 
			   ofxProjectorBlendLayout layout = ofxProjectorBlend_Horizontal, 
			   ofxProjectorBlendRotation rotation = ofxProjectorBlend_NoRotation);
	
	//call begin and end like you would if if you were using a normal fbo
	void begin();
	void end();
	
	void draw(float x = 0, float y = 0);
	
	
	// this is how big the area to draw to is.
	float getCanvasWidth();
	float getCanvasHeight();

	ofFbo & getFullTexture() { return fullTexture; }

	void setShaderLocation(string shaderLocation);
	void setPoint(int which, float x, float y);
    void setPoints(ofPoint pts[pointNum]);
    void setPoints(ofPoint inputPts[pointNum], ofPoint outputPts[pointNum]);
	
	/** 
	 * This is how big all the projector resolutions would be
	 * if laid out horizontally, next to eachother. This is
	 * essentially the output resolution (regardless of rotation
	 * and orientation.) - the output to set your project to.
	 *
	 * On a mac, if you want multiple screens doing fullscreen, 
	 * you need to put your screens next to eachother with the
	 * main one (the one with the menu) on the left hand side.
	 * If you don't, you'll only get the first screen.
	 */
	float getDisplayWidth();
	float getDisplayHeight();
	
	void moveDisplayVertical(unsigned int targetDisplay, int yOffset);
	void drawOutputDiagnostically(float x, float y, float w, float h, bool toggleImage); // can't specify abs dimensions, since might be diff apsect ratio...
	
	/** This changes your app's window size to the correct output size */
	void setWindowToDisplaySize();
	
	bool showBlend;
	
	// variables that control the blend
	// more info here on what these variables do
	// http://local.wasp.uwa.edu.au/~pbourke/texture_colour/edgeblend/
	float blendPower;
	float gamma;
	float luminance;
	float blendPower2;
	float gamma2;
	float luminance2;
	float threshold;
	float screenWidth, screenHeight;
#ifdef USE_SIMPLE_GUI
	void addGuiPage();
#endif
	void drawScreen(int nScreen);
#ifdef USE_XML_GUI
	ofxXmlGui *getGui();
	ofxXmlGui *gui;
#endif
    bool mouseSelectOutputPoint(ofRectangle drawRect, ofPoint mousePoint, float clickRadius = CLICKRADIUS * 2);
    bool mouseDragOutputPoint(ofRectangle drawRect, ofPoint mousePoint, bool xAdjust, bool yAdjust, bool maximum, bool isAlt, bool moveall);
    bool keyPressOutputPoint(int key, bool xAdjust, bool yAdjust, bool maximum, bool isAlt, bool moveall);
    void checkitout(int diffX, int diffY, bool xAdjust, bool yAdjust, bool maximum, bool isAlt, bool moveall);
    int    activeScreenIn, activePointIn, activeScreenOut, activePointOut, nScreens;
    bool bInputSelected;
    string xmlFile;
    void resetCoordinates();
    void reloadFromXml();
    void loadFromXml(string xmlFileName);
    void saveToXml();
    void saveToXml(string xmlFileName);
    void singlePoint(int myPoint, int youScreen);
protected:
	
	vector<int> projectorHeightOffset;
	string shaderLocation;
	
	float fullTextureWidth;
	float fullTextureHeight;
	float singleChannelWidth;
	float singleChannelHeight;

	float displayWidth;
	float displayHeight;
	
	float pixelOverlap;
	int numProjectors;
	ofxProjectorBlendLayout layout;
	ofxProjectorBlendRotation rotation;
	ofShader blendShader;
	ofFbo fullTexture;
	void updateShaderUniforms();
    
    void updatePoints();
    
    ofPoint quad[pointNum];
    ofPoint utQuad[pointNum];
    ofPoint * grid;
    ofPoint * coor;
    
    ofPoint ** outputPositions;
    ofPoint ** inputPositions;
    ofxXmlSettings xml;
};

#endif