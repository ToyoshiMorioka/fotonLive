#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    audio.setup();
    view1.setup(&audio);
    higashi.setup(&audio);
    akgwView.setup(&audio);
    mrtView.setup(&audio);
    hsgwView.setup(&audio);
    tokuiView.setup(&audio);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    ofFbo temp, temp2;
    switch(developer){
        case Developer::Morioka:
            temp = view1.drawAndGetFbo();
            break;
        case Developer::Akagawa:
            temp2 = akgwView.drawAndGetFbo();
            temp2.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
            break;
        case Developer::Higashi:
            temp = higashi.drawAndGetFbo();
            temp.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
            break;
        case Developer::Murata:
            temp = mrtView.drawAndGetFbo();
            temp.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
            break;
        case Developer::Hasegawa:
            temp = hsgwView.drawAndGetFbo();
            temp.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
            break;
        case Developer::Tokui:
            temp = tokuiView.drawAndGetFbo();
            temp.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
            break;
        default:
            break;
    }
    
    
    if(showAudioData){
        audioDataDraw();
        ofSetColor(0);
        ofDrawBitmapString(ofGetFrameRate(), 30, 25);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case 'r':
            higashi.reloadShader();
            break;
        case 's':
            higashi.switchShader();
            break;
        case 'a':
            showAudioData = !showAudioData;
            break;
        case ' ':
            developer = static_cast<Developer>((static_cast<int>(developer) + 1) % DeveloperNumber);
            break;
        default:
            break;
    }
    //akagawa degug---------
    if(key >=48&&key <58){
        akgwView.setRotatePattern(key-48);
    }
    if(key == OF_KEY_RETURN){
        akgwView.changeTrigger();
    }
    if(key == 'g'){
        akgwView.toggleGui();

    }
    
    //tokui degug---------
    if(key == 'v'){
        tokuiView.toggleGui();
    }
    if(key == 'c'){
        tokuiView.changeCameraPosition();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::audioDataDraw(){
    // audio data
    float bpm = audio.bpm();
    float bpmconfidence = audio.confidence();
    bool hasBeat = audio.hasBeat();
    vector<float> leftInput = audio.getLeftInput();
    vector<float> rightInput = audio.getRightInput();
    vector<float> spectrum = audio.getRawSpectrum();
    vector<float> nSpectrum = audio.getNormalSpectrum();
    
    
    // draw BPM
    ofSetColor(ofColor::black);
    ofDrawBitmapString("Estimated BPM: " + ofToString(bpm, 1), 30, 50);
    ofDrawBitmapString("Confidence: " + ofToString(bpmconfidence, 2), 30, 75); // confidence is an experimental feature at this moment
    
    // draw Beat
    ofDrawBitmapString("BEAT", 85, 195);
    if (hasBeat) {    //FIXME: calling this method resets hasBeat flag in ofxBTrack object
        ofSetColor(ofColor::magenta);
    } else ofSetColor(ofColor::lightGray);
    ofDrawCircle(100, 150, 30);
    
    ofSetColor(ofColor::magenta);
    ofDrawCircle(200, 150, 30 * audio.scaledVol);
    
    // draw left right input
    
    float inputWidth = (float)ofGetWidth() / (float)leftInput.size();
    for (int i = 0; i < leftInput.size(); i++) {
        
        float posx = (float)i * inputWidth;
        float posy = ofGetHeight() - 300;
        float height = -leftInput[i] * 300.0;
        ofSetColor(ofColor::blue.r, ofColor::blue.g, ofColor::blue.b, 128);
        ofDrawRectangle(posx, posy, inputWidth, height);
        
        height = -rightInput[i] * 300.0;
        ofSetColor(ofColor::red.r, ofColor::red.g, ofColor::red.b, 128);
        ofDrawRectangle(posx, posy, inputWidth, height);
    }
    
    // draw raw spectrum
    ofSetColor(ofColor::blue.r, ofColor::blue.g, ofColor::blue.b, 128);
    
    float width = (float)ofGetWidth() / (float)spectrum.size()*2;
    for (int i = 0; i < spectrum.size() / 2; i++) {
        
        float posx = (float)i * width;
        float posy = ofGetHeight();
        float height = -spectrum[i] * 300.0;
        ofDrawRectangle(posx, posy, width, height);
    }
    
    // draw normal spectrum
    ofSetColor(ofColor::red.r, ofColor::red.g, ofColor::red.b, 128);
    float nWidth = (float)ofGetWidth() / (float)nSpectrum.size() * 2;
    for (int i = 0; i < nSpectrum.size() / 2; i++) {
        
        float posx = (float)i * width;
        float posy = ofGetHeight();
        float height = -nSpectrum[i] * 300.0;
        ofDrawRectangle(posx, posy, width, height);
    }
}
