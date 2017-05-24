//
//  higashiView.cpp
//  fotonLive
//
//  Created by user on 2017/05/22.
//
//

#include "higashiView.hpp"

higashiView::higashiView(){
    reloadShader();
    resolution = ofVec2f(fotonLiveBase_FBOFinalWidth, fotonLiveBase_FBOFinalHeight);
    baseFbo.allocate(resolution.x, resolution.y);
}

void higashiView::reloadShader(){
    shader.load("higashi");
    effector.load("higashi_effect");
    cout << "shader loaded." << endl;
    
}

void higashiView::draw(){
    baseFbo.begin();
    shader.begin();
    shader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
    shader.setUniform2f("iResolution", resolution.x, resolution.y);
    shader.setUniform1f("bpm", myAudio->bpm());
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    shader.end();
    baseFbo.end();
    
    effector.begin();
    shader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
    effector.setUniform2f("iResolution", resolution.x, resolution.y);
    shader.setUniform1f("bpm", myAudio->bpm());
    shader.setUniform1f("volume", myAudio->scaledVol);
    effector.setUniformTexture("result", baseFbo.getTextureReference(), 1);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    effector.end();
    
}
