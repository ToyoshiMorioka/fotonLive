#pragma once

#include "ofMain.h"

#include "fotonLiveAudio.h"
#include "moriokaView.h"
#include "higashiView.hpp"
#include "akagawaView.h"

class ofApp : public ofBaseApp{    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void audioDataDraw();
    
    fotonLiveAudio audio;
    akagawaView akgwView;
    // akagawaVis akagawa;
    higashiView higashi;
    moriokaView view1;
    
    bool showAudioData{false};
    
    // displayed developer info
    static const int DeveloperNumber = 3;
    enum class Developer: int{
        Morioka, Akagawa, Higashi
    };
    Developer developer = Developer::Higashi;
};
