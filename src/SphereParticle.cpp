//
//  SphereParticle.cpp
//  fotonLive
//
//  Created by MORIOKAToyoshi on 2017/05/27.
//
//

#include "SphereParticle.hpp"

SphereParticle::SphereParticle(){
    color = ofColor(255,255,255);
    angleSpeed = ofVec2f(0.0, 0.0);
    axis = ofVec3f(0.0, 0.0, 0.0);
}

void SphereParticle::set(float _rollRadius){
    rollRadius = _rollRadius;
    
    float unitZ = ofRandom(-1, 1);
    float radianT = ofDegToRad(ofRandom(360));
    
    float temp = sqrt(1 - unitZ * unitZ);
    
    axis.x = rollRadius * temp * cos(radianT);
    axis.y = rollRadius * temp * sin(radianT);
    axis.z = rollRadius * unitZ;
    
    if (axis.z < 0.0 ){
        angle.x = ofRadToDeg(acos(temp))+180;
    }else{
        angle.x = ofRadToDeg(acos(temp));
    }
    
    angle.y = ofRadToDeg(radianT);
}

void SphereParticle::update(){
    
    angle.x += angleSpeed.x;
    angle.y += angleSpeed.y;
    
    axis.x = rollRadius * cos(ofDegToRad(angle.x)) * cos(ofDegToRad(angle.y));
    axis.y = rollRadius * cos(ofDegToRad(angle.x)) * sin(ofDegToRad(angle.y));
    axis.z = rollRadius * sin(ofDegToRad(angle.x));
    
}
