/**
 * CompositeMotion.h
 * Created by BH on 12/11/11.
 */

#ifndef OFX_COMPOSITEMOTION_
#define OFX_COMPOSITEMOTION_

class CompositeMotion {
public:
	
	//virtual void start() {};
	virtual void update(int time) {};
	
protected:
	
	int _ease;
	
};

#endif /* OFX_COMPOSITEMOTION_ */