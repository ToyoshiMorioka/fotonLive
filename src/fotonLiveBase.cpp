//
//  fotonLiveBase.cpp
//  fotonLive
//
//  Created by MORIOKAToyoshi on 2017/05/15.
//
//

#include "fotonLiveBase.h"

fotonLiveBase::fotonLiveBase(){
    fbo.allocate(ofGetWidth(), ofGetHeight(), fotonLiveBase_FBOFinalColorMode);
}

fotonLiveBase::~fotonLiveBase(){
    
}

void fotonLiveBase::draw(){
    
}

void fotonLiveBase::setup(fotonLiveAudio *audio){
    myAudio = audio;
}

ofFbo fotonLiveBase::drawAndGetFbo(){
    fbo.begin();
    draw();
    fbo.end();
    return fbo;
}
