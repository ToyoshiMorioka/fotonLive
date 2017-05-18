//
//  moriokaView.cpp
//  fotonLive
//
//  Created by MORIOKAToyoshi on 2017/05/18.
//
//

#include "moriokaView.h"

moriokaView::moriokaView(){
    
}

moriokaView::~moriokaView(){
    
}

void moriokaView::draw(){
    ofSetColor(255);
    ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth()/2/2);
}
