/**
 * Easing.h
 * Created by BH on 12/11/11.
 */

#ifndef OFX_COMPOSITEMOTION_EASING
#define OFX_COMPOSITEMOTION_EASING

#define EASE_LINEAR 1

class Easing {
public:
	static float easeLinear(float time, float intialValue, float endValue, float duration) {
		return endValue * time/duration + intialValue;
	}
	
	static float chooseEase(int ease, float time, float intialValue, float endValue, float duration) {
		switch (ease) {
			case EASE_LINEAR:
			default:
				return Easing::easeLinear(time, intialValue, endValue, duration);
			break;
		}
	}
};

#endif /* OFX_COMPOSITEMOTION_EASING */