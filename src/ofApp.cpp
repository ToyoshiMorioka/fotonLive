#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    audio.setup();
    view1.setup(&audio);
    higashi.setup(&audio);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    audioDataDraw();
    ofSetColor(255);
    ofFbo temp = higashi.drawAndGetFbo();
    temp.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
    for (int i = 0; i < spectrum.size()/2; i++) {
        
        float posx = (float)i * width;
        float posy = ofGetHeight();
        float height = -spectrum[i] * 300.0;
        ofDrawRectangle(posx, posy, width, height);
    }
    
    // draw normal spectrum
    ofSetColor(ofColor::red.r, ofColor::red.g, ofColor::red.b, 128);
    float nWidth = (float)ofGetWidth() / (float)nSpectrum.size()*2;
    for (int i = 0; i < nSpectrum.size()/2; i++) {
        
        float posx = (float)i * width;
        float posy = ofGetHeight();
        float height = -nSpectrum[i] * 300.0;
        ofDrawRectangle(posx, posy, width, height);
    }
}
