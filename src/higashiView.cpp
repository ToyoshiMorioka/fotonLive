//
//  higashiView.cpp
//  fotonLive
//
//  Created by user on 2017/05/22.
//
//

#include "higashiView.hpp"

higashiView::higashiView(){
    shader.load("higashi");
}

void higashiView::draw(){
    ofClear(255);
    ofSetColor(255);
    shader.begin();
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    shader.end();
}
