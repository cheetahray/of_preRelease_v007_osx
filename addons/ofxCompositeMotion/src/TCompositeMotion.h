/**
 * TCompositeMotion.h
 * Created by BH on 12/11/11.
 */

#ifndef OFX_COMPOSITEMOTION_TEMPLATE
#define OFX_COMPOSITEMOTION_TEMPLATE

#include <vector>
#include <algorithm>

using namespace std;

/**
 * Template for all motions.
 */
template<class M>
class Composite: public M {
	
protected:
	
	vector<M*> motions; //Child T Motion list.
	
public:
	
	typedef	typename vector<M*>::iterator MotionIterator;
	
	/*
	 * Add a Component to the child list.
	 */
	void add(M* motion) {
		MotionIterator itr = find(motions.begin(),motions.end(),motion);
		if( itr == motions.end()) {
			motions.push_back(motion);
		}
	}
	/*
	 * Remove a component from the child list if it exist there.
	 */
	void removeMotion(M* motion) {
		MotionIterator itr = find(motions.begin(),motions.end(),motion);
		if( itr != motions.end()) {
			motions.erase(itr);
		}
	}

	/*
	 * Remove all child from the list.
	 */
	void Clear() {
		motions.clear();
	}
	virtual ~Composite()
	{
		
	}
};

#endif /* OFX_COMPOSITEMOTION_TEMPLATE */