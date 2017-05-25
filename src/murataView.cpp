//
//  moriokaView.cpp
//  fotonLive
//
//  Created by MORIOKAToyoshi on 2017/05/18.
//
//

#include "murataView.h"

murataView::murataView(){
    ofTrueTypeFont::setGlobalDpi(72);
    
    verdana14.load("verdana.ttf", 14, true, true);
    verdana14.setLineHeight(18.0f);
    verdana14.setLetterSpacing(1.037);
    
    verdana30.load("verdana.ttf", 30, true, true);
    verdana30.setLineHeight(34.0f);
    verdana30.setLetterSpacing(1.035);
    
    verdana14A.load("verdana.ttf", 14, false);
    verdana14A.setLineHeight(18.0f);
    verdana14A.setLetterSpacing(1.037);
    
    franklinBook14.load("frabk.ttf", 14);
    franklinBook14.setLineHeight(18.0f);
    franklinBook14.setLetterSpacing(1.037);
    
    franklinBook14A.load("frabk.ttf", 14, false);
    franklinBook14A.setLineHeight(18.0f);
    franklinBook14A.setLetterSpacing(1.037);
    
    // Setup post-processing chain
    post.init(ofGetWidth(), ofGetHeight());
//    post.createPass<BloomPass>();
//    post.createPass<FxaaPass>();
//    post.createPass<NoiseWarpPass>();
//    post.createPass<KaleidoscopePass>();
    
//    post.createPass<FxaaPass>()->setEnabled(false);
//    ->setEnabled(false);
//    post.createPass<DofPass>()->setEnabled(false);
//    post.createPass<KaleidoscopePass>()->setEnabled(false);
//    post.createPass<NoiseWarpPass>()->setEnabled(false);
//    post.createPass<PixelatePass>()->setEnabled(false);
//    post.createPass<EdgePass>()->setEnabled(false);
//    post.createPass<VerticalTiltShifPass>()->setEnabled(false);
//    post.createPass<GodRaysPass>()->setEnabled(false);
    
    count = 0;
    scene = 0;
    
    rotate.set(0.0,0.0,0.0); // now v1.x is 10, v1.y is 50, v1.z is 80
    isRandom = false;
    rotateZ = 0;
    
    colorSet[0] = ofColor(219,127,241);
    colorSet[1] = ofColor(217,247,150);
    colorSet[2] = ofColor(28,21,24);
    colorSet[3] = ofColor(185,254,237);
    colorSet[5] = ofColor(231,14,78);
    colorSet[4] = ofColor(45,61,150);
    
    mix = 0;
    positiveMix = true;
    moveTrigger = false;
    enableFill = false;
//    ofColor(219,127,241)
}

murataView::~murataView(){
    
}

void murataView::draw(){
    
    //アルファチャンネル(透明度)を使用可能に
    ofEnableAlphaBlending();
    //混色を、加算混色に
    glBlendFunc(GL_ONE, GL_ONE);
    
    int colorIndex = scene % 3;
    
    ofBackground(colorSet[colorIndex*2]);

    
    bool hasBeat = myAudio->hasBeat();
    
    vector<float> leftInput = myAudio->getLeftInput();
    rotateZ = rotateZ - 0.1;
    
    if(positiveMix &&  moveTrigger){
        mix = mix + 0.1;
    }else if(!positiveMix && moveTrigger){
        mix = mix - 0.1;
    }
    if(mix > 1 && moveTrigger){
        mix = 1;
        positiveMix = false;
        moveTrigger = false;
    }
    
    if(mix < 0 && moveTrigger){
        mix = 0;
        positiveMix = true;
        moveTrigger = false;
    }
    
    
//    easyCam.begin();
    post.begin(easyCam);
    
    ofLog(OF_LOG_NOTICE, "posX is %f", easyCam.getGlobalPosition().x);
    ofLog(OF_LOG_NOTICE, "posY is %f", easyCam.getGlobalPosition().y);
    ofLog(OF_LOG_NOTICE, "posZ is %f", easyCam.getGlobalPosition().z);
    ofLog(OF_LOG_NOTICE, "oriX is %f", easyCam.getGlobalOrientation().x());
    ofLog(OF_LOG_NOTICE, "oriy is %f", easyCam.getGlobalOrientation().y());
    ofLog(OF_LOG_NOTICE, "oriz is %f", easyCam.getGlobalOrientation().z());
    ofLog(OF_LOG_NOTICE, "oriw is %f", easyCam.getGlobalOrientation().w());

    if(hasBeat){
        count++;
    }
    
    if(count % 4 == 0 && count != tempCount  ){
        rotate.set(ofRandom(-20,20),ofRandom(-90,90),ofRandom(-90,90)); // now v1.x is 10, v1.y is 50, v1.z is 80
        count = tempCount;
        scene++;
        enableFill = !enableFill;
//        mix = ofRandom(0,1);
    }
    if(scene % 4 == 0 &&  scene != tempScene  ){
        tempScene = scene;
        if(!moveTrigger){
            moveTrigger =  true;
        }
    }

    ofRotateX(rotate.x);
    ofRotateY(rotate.y);
    ofRotateZ(rotateZ);
    
    ofPushMatrix();

//    line.moveTo(0, 0 , 0);

    inputHistory.insert(inputHistory.begin(),leftInput);
    mixHistory.insert(mixHistory.begin(),mix);

    if(hasBeat){
        hasBeatHistory.insert(hasBeatHistory.begin(),true);
    }else{
        hasBeatHistory.insert(hasBeatHistory.begin(),false);
    }
    
    if(inputHistory.size() > 200.0){
        inputHistory.pop_back();
        hasBeatHistory.pop_back();
    }
    
//    cout<<myAudio->scaledVol<<endl;
    
    for (int i=0; i< inputHistory.size();i++){
        
        ofPath line = ofPath();
        if(hasBeatHistory[i] || i == 0){
            line.setStrokeColor(colorSet[colorIndex*2+1]);
            line.setFillColor(colorSet[colorIndex*2+1]);
            line.setFilled(enableFill);
            line.setStrokeWidth(10);
        }else{
            line.setStrokeColor(colorSet[colorIndex*2+1]);
            line.setFillColor(colorSet[colorIndex*2+1]);
            line.setFilled(enableFill);
            line.setStrokeWidth(4);
        }
        
        float tempMax = 0;
        float tempi = 0;
        float radius = 200;

//        float mix = 0.5;
        
        for (int j = 0; j < inputHistory[i].size(); j++) {
            int index = (int)inputHistory[i].size() - j - 1;
            float lineX = -1 *((float)j - ( inputHistory[i].size() / 2) )* 10;
            float lineY = -inputHistory[i][j] * 300;
            float lineZ = i*-100 + 500;
            
            float theta = 2*3.14 / inputHistory[i].size() * j;
            
            float circleX = sin(theta) * radius + (inputHistory[i][j]*200) ;
            float circleY = cos(theta) * radius + (inputHistory[i][j]*200);
            float circleZ = i*-100 + 500 + ((1/inputHistory[i].size() * j) * -100);
            
            float posX = lineX * mixHistory[i] + circleX *  (1-mixHistory[i]);
            float posY = lineY* mixHistory[i] + circleY *  (1-mixHistory[i]);
            float posZ = lineZ* mixHistory[i] + circleZ *  (1-mixHistory[i]);
            
            line.lineTo( posX,posY,lineZ );
            
//            if(tempMax < inputHistory[i][j]){
//                tempMax = height;
//                tempi = posx;
//            }
            
            
            
        }


        for (int j = 0; j < inputHistory[i].size(); j++) {

        }
        
        line.draw();
        line.close();
    }
    ofPopMatrix();
    
    post.end();

//    audioDataDraw();
}

//--------------------------------------------------------------
void murataView::audioDataDraw(){
    // audio data
    
    float bpm = myAudio->bpm();
    float bpmconfidence = myAudio->confidence();
    bool hasBeat = myAudio->hasBeat();
    vector<float> leftInput = myAudio->getLeftInput();
    vector<float> rightInput = myAudio->getRightInput();
    vector<float> spectrum = myAudio->getRawSpectrum();
    vector<float> nSpectrum = myAudio->getNormalSpectrum();

    // draw BPM
//    ofSetColor(ofColor::black);
//    ofDrawBitmapString("Estimated BPM: " + ofToString(bpm, 1), 30, 50);
//    ofDrawBitmapString("Confidence: " + ofToString(bpmconfidence, 2), 30, 75); // confidence is an experimental feature at this moment
//    
    // draw Beat
    ofDrawBitmapString("BEAT", 85, 195);
    if (hasBeat) {    //FIXME: calling this method resets hasBeat flag in ofxBTrack object
        ofSetColor(ofColor::magenta);
    } else ofSetColor(ofColor::lightGray);
    ofDrawCircle(100, 150, 30);
    
    ofSetColor(ofColor::magenta);
    ofDrawCircle(200, 150, 30 * myAudio->scaledVol);
    
    // draw left right input
    
//    float inputWidth = (float)ofGetWidth() / (float)leftInput.size();
//    for (int i = 0; i < leftInput.size(); i++) {
//        
//        float posx = (float)i * inputWidth;
//        float posy = ofGetHeight() - 300;
//        float height = -leftInput[i] * 300.0;
//        ofSetColor(ofColor::blue.r, ofColor::blue.g, ofColor::blue.b, 128);
//        ofDrawRectangle(posx, posy, inputWidth, height);
    
//        height = -rightInput[i] * 300.0;
//        ofSetColor(ofColor::red.r, ofColor::red.g, ofColor::red.b, 128);
//        ofDrawRectangle(posx, posy, inputWidth, height);
//    }
    
    // draw raw spectrum
//    ofSetColor(ofColor::blue.r, ofColor::blue.g, ofColor::blue.b, 128);
//    
//    float width = (float)ofGetWidth() / (float)spectrum.size()*2;
//    for (int i = 0; i < spectrum.size()/2; i++) {
//        float posx = (float)i * width;
//        float posy = ofGetHeight();
//        float height = -spectrum[i] * 300.0;
//        ofDrawRectangle(posx, posy, width, height);
//    }
    
//    // draw normal spectrum
//    ofSetColor(ofColor::red.r, ofColor::red.g, ofColor::red.b, 128);
//    float nWidth = (float)ofGetWidth() / (float)nSpectrum.size()*2;
//    for (int i = 0; i < nSpectrum.size()/2; i++) {
//        
//        float posx = (float)i * width;
//        float posy = ofGetHeight();
//        float height = -nSpectrum[i] * 300.0;
//        ofDrawRectangle(posx, posy, width, height);
//    }
    
}
