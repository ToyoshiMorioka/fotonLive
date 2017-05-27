//
//  SphereParticle.hpp
//  fotonLive
//
//  Created by MORIOKAToyoshi on 2017/05/27.
//
//

#ifndef SphereParticle_hpp
#define SphereParticle_hpp

#include "ofMain.h"

class SphereParticle{
public:
    ofColor color;
    ofVec2f angleSpeed;
    ofVec3f axis;
    
    SphereParticle();
    
    void set(float _rollRadius);
    
    void update();
    
private:
    
    ofVec2f angle;
    float rollRadius;
    
};

#endif /* SphereParticle_hpp */
