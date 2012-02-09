#ifndef _RENDER_MANAGER_H
#define _RENDER_MANAGER_H

#define divideH 7
#define divideV 10
#define CLICKRADIUS 5

#include "ofxXmlSettings.h"
#include "ofTextureAdv.h"

class renderManager {


    public:

        renderManager();
        void allocateForNScreens(int renderWidth, int renderHeight);

        void drawInputDiagnostically(float x, float y, float w, float h); // can't specify abs dimensions, since might be diff apsect ratio...
        void drawOutputDiagnostically(float x, float y, float w, float h, bool toggleImage); // can't specify abs dimensions, since might be diff apsect ratio...

        void drawScreen();
		void drawScreen(int nScreen);

        void startOffscreenDraw();
        void endOffscreenDraw();

        bool mouseSelectInputPoint(ofRectangle drawRect, ofPoint mousePoint, float clickRadius = CLICKRADIUS * 2);
        bool mouseDragInputPoint(ofRectangle drawRect, ofPoint mousePoint);

        bool mouseSelectOutputPoint(ofRectangle drawRect, ofPoint mousePoint, float clickRadius = CLICKRADIUS * 2);
        bool mouseDragOutputPoint(ofRectangle drawRect, ofPoint mousePoint, bool xAdjust, bool yAdjust, bool maximum, bool isAlt, bool moveall);
		bool keyPressOutputPoint(int key, bool xAdjust, bool yAdjust, bool maximum, bool isAlt, bool moveall);
		void checkitout(int diffX, int diffY, bool xAdjust, bool yAdjust, bool maximum, bool isAlt, bool moveall);
		void minAndmax();
        void resetCoordinates();
		void singlePoint(int myPoint, int youScreen);
        void reloadFromXml();
        void loadFromXml(string xmlFileName);
        void saveToXml();
        void saveToXml(string xmlFileName);
		
        int    activeScreenIn;
        int    activePointIn;

        int    activeScreenOut;
        int    activePointOut;

        float width, height;
        float screenWidth, screenHeight;
        int nScreens;

        bool bInputSelected;

        ofPoint ** outputPositions;
        ofPoint ** inputPositions;

        string xmlFile;

        ofTextureAdv myOffscreenTexture;
        ofxXmlSettings xml;
		float setSize;

};

#endif

