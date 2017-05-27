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
    baseFbo.allocate(resolution.x/2, resolution.y/2);
    current = Fragment::Shader3;
    for(int i = 0; i < historySize; ++i){
        history.push_front(0.0);
    }
}

void higashiView::reloadShader(){
    shader1.load("", "higashi/shader1.frag");
    shader2.load("", "higashi/shader2.frag");
    shader3.load("", "higashi/shader3.frag");
    effector.load("", "higashi/effector.frag");
    cout << "shader loaded." << endl;
    
}

void higashiView::switchShader(){
    current = static_cast<Fragment>((static_cast<int>(current)+1)%3);
}

void higashiView::switchEffector(){
    currentEffect = 0;
    if(ofRandom(1.0)<0.3) currentEffect += 1;
    if(ofRandom(1.0)<0.15) currentEffect += 2;
    if(ofRandom(1.0)<0.9) currentEffect += 4;
}

void higashiView::draw(){
    auto temp = temp_history();
    baseFbo.begin();
    ofShader *shader;
    if(current == Fragment::Shader1){
        shader = &shader1;
    }else if(current == Fragment::Shader2){
        shader = &shader2;
    }else{
        shader = &shader3;
    }
    shader->begin();
    shader->setUniform1f("iGlobalTime", ofGetElapsedTimef());
    shader->setUniform2f("iResolution", resolution.x/2, resolution.y/2);
    shader->setUniform1f("bpm", myAudio->bpm());
    shader->setUniform1fv("history", temp.data(), historySize);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    shader->end();
    baseFbo.end();
    
    effector.begin();
    effector.setUniform1f("iGlobalTime", ofGetElapsedTimef());
    effector.setUniform2f("iResolution", resolution.x, resolution.y);
    effector.setUniform1f("bpm", myAudio->bpm());
    effector.setUniform1f("volume", myAudio->scaledVol);
    effector.setUniformTexture("result", baseFbo.getTextureReference(), 1);
    effector.setUniform2f("result_resolution", resolution/2.0);
    effector.setUniform1i("state", currentEffect);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    effector.end();
    
}

array<float, higashiView::historySize> higashiView::temp_history(){
    if(myAudio->hasBeat()){
        if(myAudio->scaledVol > 0.2) history.push_front(1.0);
        else history.push_front(myAudio->scaledVol);
        switchEffector();
        if(ofRandom(1.0) < 0.2 && myAudio->scaledVol > 0.5) switchShader();
    }else{
        if(0.50 < myAudio->scaledVol && myAudio->scaledVol < 0.55){
            history.push_front(myAudio->scaledVol);
        }else{
            history.push_front(0.85*history.front());
        }
        
    }
    
    if(history.size() > historySize) history.pop_back();
    array<float, higashiView::historySize> temp;
    int i=0;
    for(auto v: history){
        temp[i] = v;
        ++i;
    }
    return temp;
}
