#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){
	pathFinder.setupGrid( 25 , true ) ; 
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw()
{
   pathFinder.draw() ; 
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch ( key ) 
    {
        case 'f' : 
        case 'F' : 
            pathFinder.startPathFinding() ; 
            break ;

		case 'c':
		case 'C':
			pathFinder.reset() ; 
			break; 


    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	pathFinder.setObstacles ( x , y ) ;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
   pathFinder.input ( x , y ) ;
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}