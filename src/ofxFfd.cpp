//
//  ofxFfd.cpp
//  
//
//  Created by satcy on 1/10/14.
//
//

#include "ofxFfd.h"

double ofxFfd::factorials (int n) {
    double fact = 1;
    
    for (int i = n; i > 0; i--)
        fact = fact * i;
    
    return fact;
}


double ofxFfd::bernstein (int u, int v, double STUpt)
{
    double binomial;
    double bern;
    
    /*********  BINOMIAL COEFFICIANT OF (v u)  *********/
    binomial = factorials(v) / (factorials(v - u) * factorials(u));
    bern = binomial * pow(STUpt, (double)u) * pow((1-STUpt), (double)(v-u));
    
    return bern;
    
}

ofVec3f ofxFfd::convertToSTU(const ofVec3f & xxx){
    ofVec3f STUxxx;
    
    ofVec3f cp_S = T_axis.crossed(U_axis);
    ofVec3f cp_T = S_axis.crossed(U_axis);
    ofVec3f cp_U = S_axis.crossed(T_axis);
    ofVec3f vs = xxx - origin;
    
    STUxxx.x = cp_S.dot(vs) / cp_S.dot(S_axis);
    STUxxx.y = cp_T.dot(vs) / cp_T.dot(T_axis);
    STUxxx.z = cp_U.dot(vs) / cp_U.dot(U_axis);
    
    return STUxxx;
}

ofVec3f ofxFfd::deform(const ofVec3f & p){
    ofVec3f STUp = convertToSTU(p);
    
    /*********  CALCUALTE THE TRIVARIATE BERNSTEIN FUNCTION **********/
    ofVec3f ffd3, ffd2, ffd1;
    double bpS = 0, bpT = 0, bpU = 0;
    
    ffd3.x = ffd3.y = ffd3.z = 0;
    ffd2.x = ffd2.y = ffd2.z = 0;
    ffd1.x = ffd1.y = ffd1.z = 0;
    
    for(int i = 0; i <= l; i++) {
        ffd2.x = 0; ffd2.y = 0; ffd2.z = 0;
        for(int j = 0; j <= m; j++) {
            ffd3.x = 0; ffd3.y = 0; ffd3.z = 0;
            for(int k = 0; k <= n; k++) {
                bpU = bernstein(k, n, STUp.z);
                
                ffd3.x = ffd3.x + (bpU * PCI[i][j][k].x);
                ffd3.y = ffd3.y + (bpU * PCI[i][j][k].y);
                ffd3.z = ffd3.z + (bpU * PCI[i][j][k].z);
            }
            bpT = bernstein(j, m, STUp.y);
            
            ffd2.x = ffd2.x + (bpT * ffd3.x);
            ffd2.y = ffd2.y + (bpT * ffd3.y);
            ffd2.z = ffd2.z + (bpT * ffd3.z);
        }
        bpS = bernstein(i, l, STUp.x);
        
        ffd1.x = ffd1.x + (bpS * ffd2.x);
        ffd1.y = ffd1.y + (bpS * ffd2.y);
        ffd1.z = ffd1.z + (bpS * ffd2.z);
    }
    
    
    return ffd1;
}

void ofxFfd::mouseDragged( ofMouseEventArgs & mouseEvent){
    //todo
}
void ofxFfd::mouseMoved( ofMouseEventArgs & mouseEvent){
    
}
void ofxFfd::mousePressed( ofMouseEventArgs & mouseEvent){
    
}
void ofxFfd::mouseReleased( ofMouseEventArgs & mouseEvent){
    
}

void ofxFfd::enableMouseEvent(){
    ofRegisterMouseEvents(this);
}

void ofxFfd::disableMouseEvent(){
    ofUnregisterMouseEvents(this);
}

void ofxFfd::setControlPointPosition(int ix, int iy, int iz, ofVec3f vec){
    if ( ix <= l && iy <= m && iz <= n )
        PCI[ix][iy][iz] = vec;
    else
        return;
}

void ofxFfd::setup(int numX, int numY, int numZ){
    setMinMax(ofVec3f(-100, -100, -100), ofVec3f(100, 100, 100));
    setControlPointSize(numX, numY, numZ);
}

void ofxFfd::setMinMax(ofVec3f minX, ofVec3f maxX)
{
    origin = minX;
    
    axis = maxX - minX;
    
    S_axis.x = axis.x;
    S_axis.y = S_axis.z = 0;
    
    T_axis.x = T_axis.z = 0;
    T_axis.y = axis.y;
    
    U_axis.x = U_axis.y = 0;
    U_axis.z = axis.z;
}

void ofxFfd::setControlPointSize(int numX, int numY, int numZ){
    l = ofClamp(numX, 0, 9);
    m = ofClamp(numY, 0, 9);
    n = ofClamp(numZ, 0, 9);
    for(int i = 0; i <= l; i++){
        for(int j = 0; j <= m; j++){
            for (int k = 0; k <= n; k++){
                PCI[i][j][k].x = origin.x + (((double)i/l) * axis.x);
                PCI[i][j][k].y = origin.y + (((double)j/m) * axis.y);
                PCI[i][j][k].z = origin.z + (((double)k/n) * axis.z);
                
            }
        }
    }
}

ofMesh ofxFfd::deformMesh(ofMesh mesh){
    ofMesh dst;
    dst.append(mesh);
    for ( int i=0; i<mesh.getNumVertices(); i++ ) {
        ofVec3f vec = mesh.getVertex(i);
        dst.setVertex(i, deform(vec));
    }
    return dst;
}

ofVec3f ofxFfd::deformPoint(const ofVec3f &vec){
    return deform(vec);
}

void ofxFfd::debugDraw(){
    for(int i = 0; i <= l; i++){
        for(int j = 0; j <= m; j++){
            for (int k = 0; k <= n; k++){
                ofSetColor(0, 255, 255);
                ofDrawBox(PCI[i][j][k], 4);
                ofSetColor(255, 0, 0);
                if ( j > 0) ofLine(PCI[i][j-1][k], PCI[i][j][k]);
                if ( k > 0) ofLine(PCI[i][j][k-1], PCI[i][j][k]);
                if ( i > 0) ofLine(PCI[i-1][j][k], PCI[i][j][k]);
            }
        }
    }
}