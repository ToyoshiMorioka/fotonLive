#pragma once

#include "ofMain.h"

#include "fotonLiveAudio.h"
#include "moriokaView.h"
#include "higashiView.hpp"
#include "akagawaView.h"
#include "murataView.h"
#include "hasegawaView.h"
#include "tokuiView.hpp"

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
    higashiView higashi;
    moriokaView mrokView;
    murataView mrtView;
    hasegawaView hsgwView;
    tokuiView tokuiView;
    
    float viewChangeLimitmilliSec = 30.0;
    float timer;
    
    bool showAudioData{false};
    
    // displayed developer info
    static const int DeveloperNumber = 6;
    enum class Developer: int{
        Morioka, Akagawa, Higashi, Murata, Hasegawa, Tokui
    };
    ofFbo memberFbo[DeveloperNumber];
    Developer developer = Developer::Morioka;
};
