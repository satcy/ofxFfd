#include "testApp.h"
#include "ofxFfd.h"

ofEasyCam cam;
ofxFfd ffd;
ofMesh mesh;
vector<float> rotates;
//--------------------------------------------------------------
void testApp::setup(){
    ofIcoSpherePrimitive prmtv(200, 2);
    mesh = prmtv.getMesh();
    ffd.setup(1, 2, 1);
    for ( int i=0; i<3; i++ ) rotates.push_back(0);
}

//--------------------------------------------------------------
void testApp::update(){
    for ( int i=0; i<3; i++ ) {
        rotates[i] += (i+1)*0.01;
        for ( int j=0; j<4; j++ ) {
            int jj = j;
            if ( jj > 1 ) jj = 5 - jj;
            ffd.setControlPointPosition(j%2, i, int(j/2)%2, ofVec3f(cos(jj*(TWO_PI/4.0)+rotates[i])*141.42135623731, i*100 - 100, sin(jj*(TWO_PI/4.0)+rotates[i])*141.42135623731));
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    cam.begin();
    ofSetColor(255);
    ffd.deformMesh(mesh).drawWireframe();
    ffd.debugDraw();
    cam.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

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
