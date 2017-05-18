//
//  fotonLiveBase.h
//  fotonLive
//
//  Created by MORIOKAToyoshi on 2017/05/15.
//
//

#ifndef fotonLiveBase_h
#define fotonLiveBase_h

#include "ofMain.h"
#include "fotonLiveAudio.h"
#include "fotonConst.h"

class fotonLiveBase {
public:
    fotonLiveBase();
    virtual ~fotonLiveBase();
    virtual void draw();
    
    void setup(fotonLiveAudio* audio);
    ofFbo drawAndGetFbo();
    
private:
    bool setupFlag;
    fotonLiveAudio* myAudio;
    ofFbo fbo;
    // ofSoundStream sound;
};

#endif /* fotonLiveBase_h */
