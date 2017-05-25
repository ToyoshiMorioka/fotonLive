//
//  murataView.h
//  fotonLive
//
//  Created by HIROYOSHI MURATA on 2017/05/25.
//
//

#ifndef murataView_h
#define murataView_h

#include "fotonLiveBase.h"
#include "ofxGui.h"
#include "ofxPostProcessing.h"

class murataView: public fotonLiveBase{
    
public:
    murataView();
    ~murataView();
    void draw();
    
    void audioDataDraw();
private:
    ofEasyCam easyCam;
    vector<vector<float>> inputHistory;
    vector<float> mixHistory;
    vector<bool> hasBeatHistory;
    
    ofTrueTypeFont  franklinBook14;
    ofTrueTypeFont	verdana14;
    ofTrueTypeFont	verdana30;
    
    ofTrueTypeFont  franklinBook14A;
    ofTrueTypeFont	verdana14A;
    
    int count;
    int tempCount;
    int scene;
    int tempScene;
    ofVec3f rotate;
    bool isRandom;
    float rotateZ;
    
    ofxPostProcessing post;
    ofColor colorSet[6];
    
    float mix;
    bool positiveMix;
    bool moveTrigger;
    bool enableFill;
};



#endif /* murataView_h */
