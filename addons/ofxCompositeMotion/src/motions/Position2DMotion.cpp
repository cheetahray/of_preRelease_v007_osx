/**
 * Position2DMotion.cpp
 * Created by BH on 12/11/11.
 */

#include "Position2DMotion.h"

//------------------------------------------------
//
Position2DMotion::Position2DMotion(bool autoStart, bool isInfinity, float endX, float endY, int ease) {
	_autoStart = autoStart;
	_isInfinity = isInfinity;
	
	_endX = endX;
	_endY = endY;
	
	_ease = ease;
}

//------------------------------------------------
//
void Position2DMotion::update(int time) {
	//printf("Position2DMotion::update\n");
	
	float __x = *_x;
	float __y = *_y;
	
	if (!_isInfinity && (__x >= _endX || __y >= _endY)) {
		
		
		
		__x = _endX;
		__y = _endY;
		
	} else {
		
		__x += 1;
		__y += 1;
	}
	
	*_x = __x;
	*_y = __y;
}

void Position2DMotion::properties(float* x, float* y) {
	_x = x;
	_y = y;
}

void Position2DMotion::endAt(float endX, float endY) {
	_endX = endX;
	_endY = endY;
	
	_isInfinity = false;
}