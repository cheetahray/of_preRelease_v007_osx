#ifndef _IMAGE_TEXTURE_ADV_H_
#define _IMAGE_TEXTURE_ADV_H_

#include "ofMain.h"
#include "ofFBOTexture.h"


#define GRID_X 12
#define GRID_Y 8

#define pointNum 4

class ofTextureAdv : public ofxFBOTexture {

	public :
		ofTextureAdv();
		~ofTextureAdv();
		//void allocate(int w, int h, int internalGlDataType);
		void clear();
		void loadData(unsigned char * data, int w, int h, int glDataType) { ofTexture::loadData(data,w,h,glDataType); };
		void setPoint(int which, float x, float y);
		void setPoints(ofPoint pts[pointNum]);
		void setPoints(ofPoint inputPts[pointNum], ofPoint outputPts[pointNum]);
		void draw();
        void draw(float x, float y) { ofTexture::draw(x,y); };
        void draw(float x, float y, float w, float h) { ofTexture::draw(x,y,w,h); };
        void allocate(int w, int h, bool clear){ ofxFBOTexture::allocate(w,h, clear);   };

		int width, height;

	protected:

		void updatePoints();

		ofPoint quad[pointNum];
		ofPoint utQuad[pointNum];
		ofPoint * grid;
		ofPoint * coor;


};

#endif
