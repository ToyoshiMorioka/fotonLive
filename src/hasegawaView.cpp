//
//  hasegawaView.cpp
//  fotonLive
//
//  Created by Shunki Hasegawa on 23/05/2017.
//
//

#include "hasegawaView.h"

hasegawaView::hasegawaView(){
    
    ofBackground(30);
    
//    x=0.0;
//    y=0.0;
//    z=0.0;
//    
    
    beatFlag = true;
    triggerFlag = true;
    isTechno = false;
    isBgl = false;
    isBgp = false;
    
    vol = 0;

    easyCam.enableOrtho();
    easyCam.setVFlip(false);
    easyCam.setPosition(0, 0, 700);
    easyCam.setTarget(ofVec3f(0, 0, 1.0));
    easyCam.setDistance(1000);
    
    
    ofEnableLighting();
    ofSetSmoothLighting(true);
    ofEnableDepthTest();
    
    light.setup();
    
    light.setPosition(-100, 100, 100);
    light.lookAt(ofVec3f(0.0, 0.0, 0.0));
    light.setDirectional();
    
    material.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
    material.setAmbientColor(ofFloatColor(1.0, 1.0, 1.0));
    material.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    material.setEmissiveColor(ofFloatColor(1.0, 1.0, 1.0));
    material.setShininess(128);
    
    
    
    
}

hasegawaView::~hasegawaView(){
    ofDisableLighting();

    
}

void hasegawaView::triggerControll(int trigger){
    float volume = input * 200;
    isTechno = false;
    isBgl = false;
    
    isBgp = false;
    r_count += 1.0;;
    
    switch(trigger){
        case 0:
            ofRotateX(rot);
            break;
        case 1:
            ofRotateY(rot);
            break;
        case 2:
            ofRotateY(rot);
            ofRotateZ(rot*2);
            break;
        case 3:
            ofRotateX(-rot);
            ofRotateY(-rot*2);
            break;
        case 4:
            ofRotateY(-rot);
            break;
        case 5:
            ofRotateZ(-rot);
            break;
        case 6:
            ofRotateX(-rot);
            ofRotateZ(rot);
            break;
        case 7:
            ofRotateY(r_count);
            break;
        case 8:
            ofRotateZ(r_count);
            break;
        case 9:
            ofRotateX(r_count);
            break;
        case 10:
            ofRotateX(r_count);
            ofRotateZ(rot*2);
            break;
        case 11:
            ofRotateX(rot);
            ofRotateY(rot);
            break;
        case 12:
            ofRotateX(-rot);
            ofRotateZ(rot);
            break;
        case 13:
            ofRotateY(r_count);
            ofRotateX(-rot);
            break;
        case 14:
            ofRotateX(-r_count);
            ofRotateZ(r_count);
            break;
        case 15:
            ofRotateX(rot*2);
            break;
        case 16:
            ofRotateX(-r_count);
            ofRotateZ(rot*2);
            break;
        case 17:
            ofRotateX(-rot*2);
            ofRotateY(-rot*2);
            break;
            
        case 18:
            isBgl = true;
            ofRotateX(-rot*2);
            ofRotateY(-rot*2);
            break;
        case 19:
            isBgp = true;
            ofRotateZ(-rot);
        case 20:
            isTechno = true;
            
            ofFill();
            //te
            ofDrawSphere(-750, 300, 0, 30+volume);
            ofDrawSphere(-600, 300, 0, 30+volume);
            ofDrawSphere(-450, 300, 0, 30+volume);
            ofDrawLine(-750, 300+volume, 0, -600, 300-volume, 0);
            ofDrawLine( -600, 300+volume, 0, -450, 300-volume, 0);
            
            
            ofDrawSphere(-825, 100, 0, 30+volume);
            ofDrawSphere(-675, 100, 0, 30+volume);
            ofDrawSphere(-525, 100, 0, 30+volume);
            ofDrawSphere(-375, 100, 0, 30+volume);
            
            ofDrawLine(-825, 100+volume, 0, -675, 100-volume, 0);
            ofDrawLine( -675, 100+volume, 0, -525, 100-volume, 0);
            ofDrawLine( -525, 100+volume, 0, -375, 100-volume, 0);
            
            
            ofDrawSphere(-575, -100, 0, 30+volume);
            ofDrawSphere(-625, -250, 0, 30+volume);
            ofDrawSphere(-750, -350, 0, 30+volume);
            
            ofDrawLine( -525, 100+volume, 0, -575, -100-volume, 0);
            ofDrawLine( -575, -100+volume, 0, -625, -250-volume, 0);
            ofDrawLine( -625, -250+volume, 0, -750, -350-volume, 0);
            
            
            //ch
            ofDrawSphere(-140, 300, 0, 30+volume);
            ofDrawSphere(-165, 180, 0, 30+volume);
            ofDrawSphere(-190, 60, 0, 30+volume);
            
            ofDrawLine( -190, 60+volume, 0, -165, 180-volume, 0);
            ofDrawLine( -165, 180+volume, 0, -140, 300-volume, 0);
            
            ofDrawSphere(30, 300, 0, 30+volume);
            
            ofDrawLine( -140, 300+volume, 0, 30, 300-volume, 0);
            
            ofDrawSphere(200, 300, 0, 30+volume);
            ofDrawSphere(200, 150, 0, 30+volume);
            ofDrawSphere(180, -20, 0, 30+volume);
            
            ofDrawLine( 30, 300+volume, 0, 200, 300-volume, 0);
            ofDrawLine( -165, 180+volume, 0, -140, 300-volume, 0);
            ofDrawLine( 200, 300+volume, 0, 200, 150-volume, 0);
            ofDrawLine( 200, 150+volume, 0, 180, -20-volume, 0);
            
            ofDrawSphere(130, -200, 0, 30+volume);
            ofDrawSphere(40, -350, 0, 30+volume);
            
            ofDrawLine( 180, -20+volume, 0, 130, -200-volume, 0);
            ofDrawLine( 130, -200+volume, 0, 40, -350-volume, 0);
            
            
            //no
            ofDrawSphere(750, 300, 0, 30+volume);
            ofDrawSphere(725, 75, 0, 30+volume);
            ofDrawSphere(700, -150, 0, 30+volume);
            
            ofDrawSphere(550, -250, 0, 30+volume);
            ofDrawSphere(400, -300, 0, 30+volume);
            
            ofDrawLine( 750, 300+volume, 0, 725, 75-volume, 0);
            ofDrawLine( 725, 75+volume, 0, 700, -150-volume, 0);
            ofDrawLine( 700, -150+volume, 0, 550, -250-volume, 0);
            ofDrawLine( 550, -250+volume, 0, 400, -300-volume, 0);
            break;
        default:
            ofRotateX(rot);
            break;
    }
}

void hasegawaView::mainDraw(float l, float r){
    ofFill();
    float lMove;
    float rMove;
    
    float baseX = 320;
    float baseY = 120;
    float baseZ = 150;
    
    float x1;
    float y1[2];
    float z1[2];
    
    float x2;
    float y2[4];
    float z2;
    
    float x3;
    float y3[3];
    float z3[3];
    
    float x4;
    float y4[2];
    float z4;
    
    float x5;
    float y5;
    float z5;
    

    for(int i=0; i<2; i++){
        x1 = -baseX*2;
        y1[i]= 2*(-baseY+(i*2)*baseY);
        z1[i] = 0;
        lMove = ofRandom(-l, l);
        rMove = ofRandom(-r, r);
        ofDrawSphere(x1+lMove, y1[i]+rMove, z1[i]+lMove-rMove, 40+l);
    }
    for(int i=0; i<4; i++){
        
        x2 = -baseX;
        if(i<2) y2[i] = -2*i*baseY-baseY;
        else y2[i] = 2*(i-2)*baseY+baseY;
        z2 = baseZ * 2;
        lMove = ofRandom(-l, l)/2;
        rMove = ofRandom(-r, r)/2;
        ofDrawSphere(x2+lMove, y2[i]+rMove, z2+lMove-rMove, 37+l);
        lMove = ofRandom(-l, l)/2;
        rMove = ofRandom(-r, r)/2;
        ofDrawSphere(x2+lMove, y2[i]+rMove, -z2+lMove-rMove, 37+l);
        
        ofSetColor(255, 0, 0);
        ofDrawLine(x1, y1[0], z1[0], x2, y2[i], z2);
        ofDrawLine(x1, y1[0], z1[0], x2, y2[i], -z2);
        ofDrawLine(x1, y1[1], z1[1], x2, y2[i], z2);
        ofDrawLine(x1, y1[1], z1[1], x2, y2[i], -z2);
    }
    for(int i=0; i<3; i++){
        x3 = 0;
        y3[i] = 2*(i-1)*baseY;
        for(int j =0; j<3; j++){
            z3[j]= (j * baseZ-baseZ) * 4;
            lMove = ofRandom(-l, l)/2;
            rMove = ofRandom(-r, r)/2;
            if(j == 2) ofDrawSphere(x3+lMove, y3[i]+rMove, z3[j]+lMove-rMove, 35+(l*2));
            else ofDrawSphere(x3+lMove, y3[i]+rMove, z3[j]+lMove-rMove, 35+l);
            
            ofDrawLine(x2, y2[0], z2, x3, y3[i], z3[j]);
            ofDrawLine(x2, y2[1], z2, x3, y3[i], z3[j]);
            ofDrawLine(x2, y2[2], z2, x3, y3[i], z3[j]);
            ofDrawLine(x2, y2[3], z2, x3, y3[i], z3[j]);
            ofDrawLine(x2, y2[0], -z2, x3, y3[i], z3[j]);
            ofDrawLine(x2, y2[1], -z2, x3, y3[i], z3[j]);
            ofDrawLine(x2, y2[2], -z2, x3, y3[i], z3[j]);
            ofDrawLine(x2, y2[3], -z2, x3, y3[i], z3[j]);
        }
        
        ofDrawLine(x1, y1[0], z1[0], x2, y2[i], z2);
        ofDrawLine(x1, y1[0], z1[0], x2, y2[i], -z2);
        ofDrawLine(x1, y1[1], z1[1], x2, y2[i], z2);
        ofDrawLine(x1, y1[1], z1[1], x2, y2[i], -z2);
    }
    
    for(int i=0; i<2; i++){
        x4 = baseX;
        y4[i] = 2*(-baseY+(i*2)*baseY);
        z4 = 2*baseZ;
        lMove = ofRandom(-l, l)/8;
        rMove = ofRandom(-r, r)/8;
        ofDrawSphere(x4+lMove, y4[i]+rMove, z4+lMove-rMove, 42+l);
        lMove = ofRandom(-l, l)/8;
        rMove = ofRandom(-r, r)/8;
        ofDrawSphere(x4+lMove, y4[i]+rMove, -z4+lMove-rMove, 42+l);
        for(int j=0; j<3; j++){
            for(int k=0; k<3; k++){
                ofDrawLine(x3, y3[j], z3[k], x4, y4[i], z4);
                ofDrawLine(x3, y3[j], z3[k], x4, y4[i], -z4);
            }
        }
    }
    
    x5 = baseX * 2;
    y5 = 0;
    z5 = 0;
    lMove = ofRandom(-l, l)/16;
    rMove = ofRandom(-r, r)/16;
    ofDrawSphere(x5, y5, z5, 55+l+r);
    for(int i=0; i<2; i++){
        
        ofDrawLine(x4, y4[i], z4, x5, y5, z5);
        ofDrawLine(x4, y4[i], -z4, x5, y5, z5);
    }

}



void hasegawaView::draw(){
    
    /*
    update
    */
    if(isBgl)ofBackground(150);
    else if(isBgp)ofBackground(239,117,152);
    else ofBackground(10);
    ofSetColor(255);
    bool hasBeat = myAudio->hasBeat();
    float bpm = myAudio->bpm();
    vol = myAudio->scaledVol;
    vector<float> leftInput = myAudio->getLeftInput();
    vector<float> rightInput = myAudio->getRightInput();
    vector<float> spectrum = myAudio->getRawSpectrum();
    vector<float> nSpectrum = myAudio->getNormalSpectrum();
    
    if(hasBeat){
        lastBeatTime = 0;
        triggerTime = 0;
        r_count = 0;
        
        trigger_int = ofRandom(0, 21);
        cout<<trigger_int<<endl;
        
    }
    
    float lastFrameTime = ofGetLastFrameTime();
    lastBeatTime += lastFrameTime;
    triggerTime +=lastFrameTime;
    float fourClickTime = 60.0/bpm;
    float sixteen = fourClickTime/4;
    float sixtyfour = fourClickTime/16;
    
    if(fourClickTime<lastBeatTime){
        beatFlag = false;
    }else{
        beatFlag = true;
    }
    
    
    if(sixtyfour<triggerTime){
        triggerFlag = true;
    }
    
    
    if(beatFlag){
        
    }
    

    rot = 0.5 * ofGetFrameNum();
    
    
    easyCam.begin();
    material.begin();
    
    light.enable();
    //light.setSpotlight();
    light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    light.setDiffuseColor(ofFloatColor(0.5, 0.5, 0.5));
    light.setAmbientColor(ofFloatColor(0.7, 0.7, 0.7));
    
    
    /*
     draw
     */
    
    
    if(leftInput.size() != 0){
        
        input = leftInput[0];
        
        //ofDrawBitmapString("Estimated BPM: " + ofToString(bpm, 1), 30, 50);
        triggerControll(trigger_int);
        if(!isTechno){
            //音の入力
            int v = 120;
            mainDraw(leftInput[0]*v, rightInput[0]*v);
        }
    }
    material.end();
    
    //light.disable();
    
    
    easyCam.end();
    
    ofPushMatrix();
    ofTranslate(200, 200, 0);
    ofPopMatrix();
    

}
