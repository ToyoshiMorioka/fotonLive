#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    audio.setup();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    // BPM
    ofSetColor(ofColor::black);
    float bpm = audio.bpm();
    ofDrawBitmapString("Estimated BPM: " + ofToString(bpm, 1), 30, 50);
    ofDrawBitmapString("Confidence: " + ofToString(audio.confidence(), 2), 30, 75); // confidence is an experimental feature at this moment
    
    // Beat
    ofDrawBitmapString("BEAT", 85, 195);
    if (audio.hasBeat()) {    //FIXME: calling this method resets hasBeat flag in ofxBTrack object
        ofSetColor(ofColor::magenta);
    } else ofSetColor(ofColor::lightGray);
    ofDrawCircle(100, 150, 30);
    
    ofSetColor(ofColor::magenta);
    ofDrawCircle(200, 150, 30 * audio.scaledVol);
    
    // left right input
    vector<float> left = audio.getLeftInput();
    vector<float> right = audio.getRightInput();
    float inputWidth = (float)ofGetWidth() / (float)left.size();
    for (int i = 0; i < left.size(); i++) {
        
        float posx = (float)i * inputWidth;
        float posy = ofGetHeight() - 300;
        float height = -left[i] * 300.0;
        ofSetColor(ofColor::blue.r, ofColor::blue.g, ofColor::blue.b, 128);
        ofDrawRectangle(posx, posy, inputWidth, height);
        
        height = -right[i] * 300.0;
        ofSetColor(ofColor::red.r, ofColor::red.g, ofColor::red.b, 128);
        ofDrawRectangle(posx, posy, inputWidth, height);
    }
    

    // raw spectrum
    ofSetColor(ofColor::blue.r, ofColor::blue.g, ofColor::blue.b, 128);
    vector<float> spectrum = audio.getRawSpectrum();
    float width = (float)ofGetWidth() / (float)spectrum.size();
    for (int i = 0; i < spectrum.size(); i++) {
        
        float posx = (float)i * width;
        float posy = ofGetHeight();
        float height = -spectrum[i] * 300.0;
        ofDrawRectangle(posx, posy, width, height);
    }
    
    // normal spectrum
    ofSetColor(ofColor::red.r, ofColor::red.g, ofColor::red.b, 128);
    vector<float> nSpectrum = audio.getNormalSpectrum();
    float nWidth = (float)ofGetWidth() / (float)nSpectrum.size();
    for (int i = 0; i < nSpectrum.size(); i++) {
        
        float posx = (float)i * width;
        float posy = ofGetHeight();
        float height = -nSpectrum[i] * 300.0;
        ofDrawRectangle(posx, posy, width, height);
    }

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
