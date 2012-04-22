/**
 * MParam.h
 * Created by BH on 12/11/11.
 */

#ifndef OFX_COMPOSITEMOTION_MPARAM
#define OFX_COMPOSITEMOTION_MPARAM

template <class Mod>
class MParam {
	
public:
	
    MParam(Mod* modifier, Mod final, int ease = 1) {
		_manipulate = modifier;
		_initial = &_manipulate; 
		_final = final;
		
		_ease = ease;
	}
	
	virtual void update(int time){};
	
protected:
	
	Mod *_manipulate;
	Mod _initial;
	Mod _final;
	
	int _ease;
	
};

#endif /* OFX_COMPOSITEMOTION_MPARAM */