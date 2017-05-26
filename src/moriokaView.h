//
//  moriokaView.h
//  fotonLive
//
//  Created by MORIOKAToyoshi on 2017/05/18.
//
//

#ifndef moriokaView_h
#define moriokaView_h

#include "fotonLiveBase.h"



class moriokaView: public fotonLiveBase{
public:
    moriokaView();
    ~moriokaView();
    void draw();
    
    // random walk particles
    vector<ofVec3f> rw_particles;
    
    // params
    int rw_particleLife = 10;
    int rw_recentParticleLife = 10;
    float rw_drawLineParam = 5.0;
    const static int rw_particleNum = 100;
    
    // method
    void rw_particleSetup();
    void rw_particleReset();
    void rw_particleDraw();
    
    
    // triangle spectrum
    float ts_offsetAngle = 0.0;
    int ts_highSpeedFrame = 0;
    // float ts_highSpeedKickTime = 0.0;
    
    // params
    float ts_offsetMoveSpeed = 1.0;
    float ts_offsetMoveHighSpeed = 10.0;
    int ts_highSpeedFrameKeep = 15;
    float ts_hightSpeedThreshold = 0.03;
    
    float ts_circleYPos = 300.0;
    float ts_circleRadius = 300.0;
    float ts_spectrumParam = 100.0;
    
    float ts_spectrumThreshold = 0.1;
    float ts_spectrumByParam = 50.0;
    
    // method
    void ts_setup();
    void ts_draw();
    
    
    // landscape
    float leftLandAngle = 0.0;
    float rightLandAngle = 0.0;
    float AngleSpeed = 0.1;
    ofMesh leftMesh;
    ofMesh rightMesh;
};

#endif /* moriokaView_h */
