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
    sp_setup();
    
    // landscape
    leftMesh = ofCylinderPrimitive(200, ofGetHeight()+200, 30, 10).getCylinderMesh();
    rightMesh = ofCylinderPrimitive(200, ofGetHeight()+200, 30, 10).getCylinderMesh();
    viewTimer = ofGetElapsedTimef();
}

moriokaView::~moriokaView(){
    
}

void moriokaView::draw(){
    
    //ofSetColor(255);
    if (viewTimer + time < ofGetElapsedTimef()) {
        viewToggle = !viewToggle;
        viewTimer = ofGetElapsedTimef();
    }
    
    if (viewToggle){
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofRotate(rotateY, 0.1, 1.0, 0.0);
        sp_draw();
        ofPopMatrix();
        rotateX += 0.3;
        rotateY += 1.0;
    }else {
        ofBackground(0);
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
    
}

void moriokaView::sp_setup(){
    sp_particleCount = 400;
    for (int i = 0; i < sp_particleCount ; i++) {
        SphereParticle newParticle;
        newParticle.color = ofColor(255, 255, 255);
        newParticle.set(500);
        newParticle.angleSpeed = ofVec2f(0.0, ofRandom(3.0));
        sp_particles.push_back(newParticle);
    }
}

void moriokaView::sp_draw(){
    bool hasBeat = myAudio->hasBeat();
    int switcher = ofRandom(5);
    
    float volume = myAudio->smoothedVol;
    
    if (hasBeat) {
        sp_timer = sp_timerLimit;
        
    }else{
        sp_timer = sp_timer - 1;
    }
    
    if (sp_timer > 0) {
        ofBackground(128);
    }else{
        ofBackground(0);
    }
    
    
    for (int i = 0; i < sp_particles.size(); i++) {
        if (hasBeat) {
            switch (switcher) {
                case 0:
                    sp_particles[i].angleSpeed = ofVec2f(0.0, ofRandom(3.0));
                    break;
                case 1:
                    sp_particles[i].angleSpeed = ofVec2f(ofRandom(3.0), 0.0);
                    break;
                case 2:
                    sp_particles[i].angleSpeed = ofVec2f(ofRandom(1.5), ofRandom(1.5));
                    break;
                case 3:
                    sp_particles[i].angleSpeed = ofVec2f(3.0, 0.0);
                    break;
                case 4:
                    sp_particles[i].angleSpeed = ofVec2f(0.0, 3.0);
                    break;
                case 5:
                    sp_particles[i].angleSpeed = ofVec2f(1.5, 1.5);
                    break;
                default:
                    break;
            }
            //sp_particles[i].angleSpeed = ofVec2f(0.0, ofRandom(3.0));
            
            
        }else{
            
        }
        
        sp_particles[i].update();
        
        if (sp_timer > 0) {
            sp_particles[i].color = ofColor(0,0,0);
            ofSetColor(sp_particles[i].color);
            sp_particles[i].set(ofRandom(500 - volume * 200,500 + volume * 200));
            ofDrawSphere(sp_particles[i].axis, 5);
        }else {
            sp_particles[i].color = ofColor(255,255,255);
            ofSetColor(sp_particles[i].color);
            ofDrawSphere(sp_particles[i].axis, 5);
        }
        
        
        //ofDrawCircle(sp_particles[i].axis, 5);
    }
    
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
        
        for (int i = 0; i < rw_particleNum; i++) {
            
            int movePattern = int(ofRandom(0, 2));
            bool makeRectangle = false;
            
            std::cout << myAudio->smoothedVol << endl;
            if (myAudio->smoothedVol > rw_rectangleThreshold) {
                makeRectangle = true;
            }
            
            float speed = 100.0;
            if (makeRectangle) {
                speed = 500.0;
            }
            
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
                if (makeRectangle){
                    ofSetColor(255, 255, 255, 255);
                    ofFill();
                    ofDrawRectangle(rw_particles[i].x, rw_particles[i].y, rw_particles[i].z,  next.x - rw_particles[i].x, next.y - rw_particles[i].y + ofRandom(100) * myAudio->smoothedVol * 3);
                    ofNoFill();
                }else{
                    ofSetColor(255);
                    ofDrawLine(rw_particles[i], next);
                }
            }
            
            rw_particles[i] = next;
        }
    }
}
