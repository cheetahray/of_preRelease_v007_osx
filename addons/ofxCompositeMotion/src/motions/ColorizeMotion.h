/**
 * ColorizeMotion.h
 * Created by BH on 12/11/11.
 */

#ifndef OFX_COMPOSITEMOTION_COLORIZE
#define OFX_COMPOSITEMOTION_COLORIZE

#include "CompositeMotion.h"

class ColorizeMotion : public CompositeMotion {
	
public:
	
	ColorizeMotion(bool autoStart = true, bool isInfinity = true, float endR = 0.0, float endB = 0.0, float endG = 0.0, int ease = 1);
	
	//--------------------------------------------------------
	void start();
	void update(int time);
	
	//--------------------------------------------------------
	void properties(float* r, float* b, float* g);
	void endAt(float endR, float endB, float endG);
	
protected:
	
	float* _r;
	float* _b;
	float* _g;
	
	float _endR;
	float _endB;
	float _endG;
	
	bool _autoStart;
	bool _isInfinity;
	
};


#endif /* OFX_COMPOSITEMOTION_COLORIZE */