//
//  moriokaView.cpp
//  fotonLive
//
//  Created by MORIOKAToyoshi on 2017/05/18.
//
//

#include "moriokaView.h"

moriokaView::moriokaView(){
    rw_particleSetup();
    ts_setup();
    
    // landscape
    leftMesh = ofCylinderPrimitive(200, ofGetHeight()+200, 30, 10).getCylinderMesh();
    rightMesh = ofCylinderPrimitive(200, ofGetHeight()+200, 30, 10).getCylinderMesh();
}

moriokaView::~moriokaView(){
    
}

void moriokaView::draw(){
    ofBackground(0);
    //ofSetColor(255);
    
    // landscape
    
    // calculate
    
    // draw
    ofSetColor(255);
    
    // left cylinder
    ofPushMatrix();
    ofTranslate(-100, ofGetHeight()/2);
    ofRotateY(leftLandAngle);
    leftMesh.drawWireframe();
    leftLandAngle += AngleSpeed;
    ofPopMatrix();
    
    // right cylinder
    ofPushMatrix();
    ofTranslate(ofGetWidth()+100, ofGetHeight()/2);
    ofRotateY(rightLandAngle);
    rightMesh.drawWireframe();
    rightLandAngle -= AngleSpeed;
    ofPopMatrix();
    
    // random walk particles
    rw_particleDraw();
    
    // trangle
    ts_draw();
}

void moriokaView::ts_setup(){
    
}
void moriokaView::ts_draw(){
    ofSetColor(255);
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ofVec3f first, second, third;
    
    int triangleCount = (int)myAudio->getNormalSpectrum().size();
    float triangleWidth = ts_circleRadius * 2. * PI / (float)triangleCount;
    
    for (int i = 0; i < triangleCount; i++){
        ofPushMatrix();
        
        ofTranslate(0.0, ts_circleYPos);
        float angle = (float)i * 360.0 / (float)triangleCount;
        ofRotate(angle + ts_offsetAngle, 0.0, 1.0, 0.0);
        
        float zPos = myAudio->getNormalSpectrum()[i] * ts_spectrumParam;
        if (myAudio->smoothedVol >= ts_spectrumThreshold){
            zPos = zPos * ts_spectrumByParam;
            
        }
        
        first.set(0.0, 0.0, ts_circleRadius + zPos);
        second.set(triangleWidth / 2, 0.0, ts_circleRadius);
        third.set(-triangleWidth / 2, 0.0, ts_circleRadius);
        ofDrawTriangle(first, second, third);
        ofPopMatrix();
        
    }
    
    std::cout << myAudio->smoothedVol << endl;
    if (myAudio->smoothedVol >= ts_hightSpeedThreshold && ts_highSpeedFrame <= 0){
        ts_highSpeedFrame = ts_highSpeedFrameKeep;
    }
    
    if (ts_highSpeedFrame > 0){
        ts_offsetAngle += ts_offsetMoveHighSpeed;
        ts_highSpeedFrame--;
    }else{
        ts_offsetAngle += ts_offsetMoveSpeed;
    }
    
    
    
    ofPopMatrix();
    
}

void moriokaView::rw_particleSetup(){
    rw_recentParticleLife = rw_particleLife;
    for (int i = 0; i < rw_particleNum; i++) {
        rw_particles.push_back(ofVec3f(ofGetWidth()/2,ofGetHeight()/2,0));
    }
}

void moriokaView::rw_particleReset(){
    for (int i = 0; i < rw_particleNum; i++) {
        rw_particles[i] = ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0);
    }
}

void moriokaView::rw_particleDraw(){
    rw_recentParticleLife--;
    if (myAudio->hasBeat()) {
        rw_recentParticleLife = rw_particleLife;
    }
    
    if (rw_recentParticleLife <= 0) {
        rw_particleReset();
    }else{
        ofSetColor(255);
        
        for (int i = 0; i < rw_particleNum; i++) {
            
            int movePattern = int(ofRandom(0, 2));
            float speed = 100.0;
            ofVec3f moveDistance;
            switch (movePattern) {
                case 0:
                    moveDistance.set(ofRandom(-speed, speed),0,0);
                    break;
                case 1:
                    moveDistance.set(0,ofRandom(-speed, speed),0);
                    break;
                default:
                    break;
            }
            ofVec3f next = rw_particles[i] + moveDistance;
            
            if (i < int((float)rw_particleNum * myAudio->smoothedVol * rw_drawLineParam
                        )) {
                ofDrawLine(rw_particles[i], next);
            }
            
            rw_particles[i] = next;
        }
    }
}
