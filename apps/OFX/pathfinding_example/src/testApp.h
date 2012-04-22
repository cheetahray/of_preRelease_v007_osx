#pragma once

#include "ofMain.h"
#include "ofxPathfinding.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        vector<MapTile*> grid ;
        int nRows ;
        int nCols ;
        int gridTileSize ; 
    
        int startIndex ; 
        int endIndex ; 
    
        void startPathFinding() ; 
        vector<int> shortestPath ; 
    
        int checkTile ( int tileIndex ) ;  
		ofxPathfinding pathFinder ; 
        
		
};
