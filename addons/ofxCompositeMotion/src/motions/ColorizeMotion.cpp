/**
 * ColorizeMotion.cpp
 * Created by BH on 12/11/11.
 */

#include "ColorizeMotion.h"

ColorizeMotion::ColorizeMotion(bool autoStart, bool isInfinity, float endR, float endB, float endG, int ease) {
	_autoStart = autoStart;
	_isInfinity = isInfinity;
	
	_endR = endR;
	_endB = endB;
	_endG = endG;
	
	_ease = ease;
}

void ColorizeMotion::update(int time) {	
	float __r = *_r;
	float __b = *_b;
	float __g = *_g;
	
	if (!_isInfinity && (__r >= _endR || __b >= _endB)) {
		__r = _endR;
		__b = _endB;
		__g = _endG;
	} else {
		__r += 1.0;
		__b += 1.0;
		__g += 1.0;
	}
	
	*_r = __r;
	*_b = __b;
	*_g = __g;
}

void ColorizeMotion::properties(float* r, float* b, float* g) {
	_r = r;
	_b = b;
	_g = g;
}

void ColorizeMotion::endAt(float endR, float endB, float endG) {
	_endR = endR;
	_endB = endB;
	_endG = endG;
	
	_isInfinity = false;
}