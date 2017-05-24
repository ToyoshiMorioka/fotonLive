//
//  moriokaView.cpp
//  fotonLive
//
//  Created by MORIOKAToyoshi on 2017/05/18.
//
//

#include "akagawaView.h"

akagawaView::akagawaView(){
    panel.setup("akagawa");
    panel.add(addVol.set("addVol", 1, 0, 10));
    
    panel.add(lineColor.set("lineColor", ofFloatColor(0.5,0.0,0.5), ofFloatColor(0.0,0.0,0.0), ofFloatColor(1.0,1.0,1.0)));
    panel.add(bgColor.set("bgColor", ofColor(200,200,200), ofFloatColor(0,0,0), ofFloatColor(255,255,200)));
    panel.add(drawPattern.set("drawPattern", 0,0,3));

    
    panel.add(drawBox.set("drawBox", true));
    panel.add(drawCircle.set("drawCircle", false));
    panel.add(targetGlobalRotate.set("targetGlobalRotate", ofVec3f(35,45,0.0), ofVec3f(-360,-360,-360), ofVec3f(360,360,360)));
    panel.add(autoRotate.set("autoRotate", false));

    panel.add(loopNum.set("loopNum", 4, 1, 8));
    panel.add(translate.set("translate", ofVec3f(0.0,0.0,0.0), ofVec3f(-1000.0,-1000.0,-1000.0), ofVec3f(1000.0,1000.0,1000.0)));
    panel.add(targetLoopRotate.set("targetLoopRotate", ofVec3f(0.0,0.0,0.0), ofVec3f(-360,-360,-360), ofVec3f(360,360,360)));
    panel.add(scale.set("scale", 1, 0, 10));
    panel.add(circleMaxLife.set("circleMaxLife", 5, 0, 10));
    panel.add(circleDefaultRad.set("circleDefaultRad", 100, 0, 1000));
    panel.add(circleFadeScale.set("circleFadeScale", 1, 0, 2));
    
    
    panel.add(circleAddPos.set("circleAddPos", ofVec3f(0.0,0.1,0.0), ofVec3f(-2,-2,-2), ofVec3f(2,2,2)));
    panel.add(circleAddRot.set("circleAddRot", ofVec3f(0.0,0.1,0.01), ofVec3f(-1,-1,-1), ofVec3f(1,1,1)));
    
    cam.enableOrtho();
    cam.setNearClip(1);
    cam.setFarClip(10000);
    cam.setDistance(5000);
    
    light.setAmbientColor( ofColor(50.f, 50.f, 50.f));
    
    light.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
    light.setSpecularColor( ofColor(255.f, 25.f, 0.f));
    
    light.setDirectional();
    light.setOrientation( ofVec3f(0, 90, 0) );
    
    
    shader.load("","akagawa.frag");
    
    ofFbo::Settings fboSettings;
    fboSettings.width = fotonLiveBase_FBOFinalWidth;
    fboSettings.height = fotonLiveBase_FBOFinalHeight;
    fboSettings.internalformat = GL_RGBA;
    fboSettings.numSamples = 0;
    fboSettings.useDepth = true;
    fboSettings.useStencil = false;
    fboSettings.depthStencilAsTexture = false;
    fboSettings.textureTarget = GL_TEXTURE_RECTANGLE;
    fboSettings.minFilter =GL_LINEAR;
    fboSettings.maxFilter =GL_LINEAR;
    fboSettings.wrapModeHorizontal =GL_MIRRORED_REPEAT;
    fboSettings.wrapModeVertical =GL_MIRRORED_REPEAT;
    
    myFbo.allocate(fboSettings);
    count =0;
    totalCount =0;
    rotatePattern =0;
    for(int i = 0 ; i< 16; i++){
        v_audio.push_back(0.0);
    }
}

akagawaView::~akagawaView(){
    
}
void akagawaView::changeTrigger(){
    totalCount++;
    

    
    //change rotate pattern
    if(totalCount%8==0){
        int moveto = (ofRandom(1.0) < 0.5? -1 : 1) * (ofRandom(1.0) < 0.5? 1 : 2);
        rotatePattern +=moveto;
        if(rotatePattern< 0)rotatePattern+=6;
        rotatePattern%=7;
//        cout << "rotatePattern: " << rotatePattern << endl;
        setRotatePattern(rotatePattern);
        
        autoRotate = ofRandom(1.0) < 0.8? false : true;

    }
    
     //retorigger same rotate pattern
    if(ofRandom(1.0)<=0.5){
        setRotatePattern(rotatePattern);
    }
    
    //change drawPattern
    if(totalCount%(8*2)==0){
         float dp = ofRandom(1.0);
        
        if(dp<0.33){
            drawPattern = e_drawPattern::box;
        }else if(dp>=0.33&&dp<0.66){
            drawPattern = e_drawPattern::circle;
        }else{
            drawPattern = e_drawPattern::both;
        }
        cout << "drawPattern: " << drawPattern << endl;
        if(drawPattern==e_drawPattern::circle ||drawPattern==e_drawPattern::both){
            autoRotate = true;
        
        }else{
            autoRotate = ofRandom(1.0) < 0.8? false : true;
        }
    }

}

void akagawaView::setRotatePattern(int i){
    //global
    
    scale = ofRandom(1.0,5.0);
    
    switch (i) {
        case 0:
            targetLoopRotate = ofVec3f(0,0,0);
            targetGlobalRotate = ofVec3f(35,45,0.0);
            break;
        case 1:
            targetLoopRotate = ofVec3f(90,0,0);
            targetGlobalRotate = ofVec3f(45,35,0.0);

            break;
        case 2:
            targetLoopRotate = ofVec3f(0,90,0);
            targetGlobalRotate = ofVec3f(35,45,0.0);

            break;
        case 3:
            targetLoopRotate = ofVec3f(0,0,90);
            targetGlobalRotate = ofVec3f(35,45,0.0);

            break;
        case 4:
            targetLoopRotate = ofVec3f(45,45,0);
            targetGlobalRotate = ofVec3f(0.0,0.0,0.0);

            break;
        case 5:
            targetLoopRotate = ofVec3f(0,45,45);
            targetGlobalRotate = ofVec3f(0.0,0.0,0.0);

            break;
        case 6:
            targetLoopRotate = ofVec3f(45,0,45);
            targetGlobalRotate = ofVec3f(0.0,0.0,0.0);

            break;
        default:
            break;
    }
    //box
    boxTranslate = ofVec3f(500,0,0);
    addTransform =ofVec3f(ofRandom(-1.0,1.0),ofRandom(-1.0,1.0),ofRandom(-1.0,1.0));
    
    //circle
    circleAddPos= ofVec3f(ofRandom(-2.0,2.0),ofRandom(-2.0, 2.0),ofRandom(-2.0, 2.0));
    circleAddRot= ofVec3f(ofRandom(-1.0, 1.0),ofRandom(-1.0, 1.0),ofRandom(-1.0, 1.0));
    circleFadeScale =ofRandom(circleFadeScale.getMin(), circleFadeScale.getMax());
    
    //global rotate
    
    //set autorotate
    float ar =ofRandom(1.0);
    if(ar<0.33){
    autoRotateForce = ofVec3f(1,0,0);
    }else if(ar>=0.33&&ar<0.66){
        autoRotateForce = ofVec3f(0,1,0);
    }else{
        autoRotateForce = ofVec3f(0,0,1);
    }
}


void akagawaView::draw(){
    
    //update----------------
    
    ofSetCylinderResolution(40,2,2);
    
    float vol = myAudio->scaledVol*myAudio->scaledVol*addVol;

    bool hasBeat =myAudio->hasBeat();
    
    if(hasBeat){
        lastBeatTime = 0;
        lastbpm =myAudio->bpm();
        changeTrigger();
    }
    
    
    float lastFrameTime =ofGetLastFrameTime();
    lastBeatTime+= lastFrameTime;
    
    float fourClickTime = (60.0/lastbpm);
    
    bool fourCount =false;
    if((lastBeatTime/fourClickTime<2.0 && fmod(lastBeatTime, fourClickTime) < fmod(lastBeatTime-lastFrameTime, fourClickTime))||hasBeat){
        fourCount = true;
    }
    
    float eightClickTime = (60.0/lastbpm)/2.0;
    bool eightCount =false;
    if((lastBeatTime/eightClickTime<4.0 && fmod(lastBeatTime, eightClickTime) < fmod(lastBeatTime-lastFrameTime, eightClickTime))||hasBeat){
        eightCount = true;
        count++;
    }
    
    float sixteenClickTime = (60.0/lastbpm)/4.0;
    bool sixteenCount =false;
    if((lastBeatTime/eightClickTime<8.0 && fmod(lastBeatTime, sixteenClickTime) < fmod(lastBeatTime-lastFrameTime, sixteenClickTime))||hasBeat){
        sixteenCount = true;
    }
    
    
    
    if(sixteenCount){
        
        if(drawPattern == e_drawPattern::circle||drawPattern == e_drawPattern::both){
            circleSet tempCircle;
            tempCircle.radius = vol*circleDefaultRad +2;
            tempCircle.life = circleMaxLife;
            tempCircle.position = ofVec3f(0.0,0.0,0.0);
            tempCircle.rotate = ofVec3f(0.0,0.0,0.0);
            tempCircle.scale = 1.0;
            
            circles.insert(circles.begin(), tempCircle);
        }
    }
    
    std::vector<circleSet>::iterator it = circles.begin();
    while (it != circles.end()) {
        it->life-=lastFrameTime;
        if(it->life<=0.0){
            it = circles.erase(it);
        }else{
            it->scale-=lastFrameTime/circleMaxLife*circleFadeScale;
            it->position += circleAddPos;
            it->rotate += circleAddRot;
            ++it;
        }
        
    }
    light.setOrientation( ofVec3f( cos(ofGetElapsedTimef()) * RAD_TO_DEG,0, 0) );
    
    boxTranslate +=  ofVec3f(-2,0.,0) + addTransform;
    
    scale -= 0.0003;
    
    loopRotate+=(ofVec3f(targetLoopRotate)-loopRotate)*0.05;
    
    
    
    
    if(autoRotate){
        float autorotateSpeed = drawPattern == e_drawPattern::both? 50.0 : 20;
        targetGlobalRotate =ofVec3f(targetGlobalRotate) + autoRotateForce * lastFrameTime * autorotateSpeed;
    }
    
    globalRotate +=(ofVec3f(targetGlobalRotate)-globalRotate)*0.05;

    
    
    //loop num
    if(drawPattern == e_drawPattern::circle){
        loopNum=8;
    }else{
        loopNum=4;
    
    }
    //draw-----------------------
    myFbo.begin();
    ofClear(0);
    ofSetColor(255,255,255);
    ofPushMatrix();
    ofEnableLighting();
    ofEnableDepthTest();
    
    light.enable();
    cam.begin();
    
    ofRotateX(globalRotate.x);
    ofRotateY(globalRotate.y);
    ofRotateZ(globalRotate.z);

    ofScale(scale, scale,scale);
    
    for(int i = 0 ; i<loopNum ; i++){
        
        ofPushMatrix();
        ofRotateX(loopRotate.x*(i+1));
        ofRotateY(loopRotate.y*(i+1));
        ofRotateZ(loopRotate.z*(i+1));
        
        ofTranslate(translate);
        
        int boxNum = 8;
        float width = 100;
        float boxDist = width*(boxNum-1);
        float height = 100+ 100* vol;
        
        if(drawPattern == e_drawPattern::box||drawPattern == e_drawPattern::both){
            for(int j =0 ;j<boxNum;j++){
                
                ofPushMatrix();
                
                
                //            cout << j << " " << j*width*2.0-width*(boxNum-1) << endl;
                ofTranslate(ofVec3f(j*width*2.0-width*(boxNum-1), 0.0,0.0)+boxTranslate);
                
                //draw box-------------
                
                //side
                ofPushMatrix();
                ofFill();
                ofSetColor(j, j, j);
                ofDrawRectangle(- width/2, -height/2, -width/2, width, height);
                ofRotateY(90);
                ofDrawRectangle(- width/2, -height/2, -width/2, width, height);
                ofRotateY(90);
                ofDrawRectangle(- width/2, -height/2, -width/2, width, height);
                ofRotateY(90);
                ofDrawRectangle(- width/2, -height/2, -width/2, width, height);
                ofPopMatrix();
                
                //top
                ofPushMatrix();
                ofRotateX(90);
                if(((i+j)%8)==(count%8)){
                    ofSetColor(255, 0, 255);
                }else{
                    ofSetColor(255, 255, 0);
                }
                ofDrawRectangle(-width/2,- width/2, -height/2, width, width);
                
                ofPopMatrix();
                //bottom
                ofPushMatrix();
                ofRotateX(-90);
                
                ofSetColor(j, j, j);
                ofDrawRectangle(-width/2,- width/2, -height/2, width, width);
                
                ofPopMatrix();
                
                //draw sphere-------------
                
                //            if(((i+j)%8)==(count%8)){
                //            ofSetColor(200, 250, 100);
                //            ofDrawSphere(0, 0, 50+500 * vol);
                //            }
                
                ofPopMatrix();
            }
        }
        ofTranslate(50, 0,100);
        if(drawPattern == e_drawPattern::circle||drawPattern == e_drawPattern::both){
        ofSetColor(255, 0, 255);
        ofDrawCylinder(ofVec3f(0,0,0), vol*circleDefaultRad +2 ,0.1);
        }
        for(int k =0; k< circles.size();k++){
            
            ofPushMatrix();
            ofRotateX(circles[k].rotate.x);
            ofRotateY(circles[k].rotate.y);
            ofRotateZ(circles[k].rotate.z);

            if(k==circles.size()-1){
                ofSetColor(255, 255, 0);

            }else{
                ofSetColor(k*2+20*i);

            }
            ofFill();
            ofDrawCylinder(circles[k].position, circles[k].radius*circles[k].scale,0.1);
            ofPopMatrix();
        }
        
        ofPopMatrix();
    }
    
    //draw circle---
    
    
    
    cam.end();
    ofPopMatrix();
    
    light.disable();
    ofDisableDepthTest();
    ofDisableLighting();
    
    ofSetColor(255);
    
    myFbo.end();
    
    ofBackgroundGradient(ofColor(bgColor)+ofColor(60,60,60), bgColor);
    ofSetColor(255);
    
    //pass uniform to shader
    
    
    shader.begin();
    shader.setUniform4f("lineColor", ofFloatColor(lineColor));
    
    myFbo.draw(0,0);
    shader.end();
    
//    if(hasBeat){
//        ofSetColor(255, 0, 0);
//        ofDrawCircle(10, 10, 10);
//    }
//    
//    if(fourCount){
//        ofSetColor(255, 0, 0);
//        ofDrawCircle(30, 10, 10);
//    }
//    
//    if(eightCount){
//        ofSetColor(255, 0, 0);
//        ofDrawCircle(50, 10, 10);
//    }
    
//    panel.draw();
    
}
