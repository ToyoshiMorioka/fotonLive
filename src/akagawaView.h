//
//  moriokaView.h
//  fotonLive
//
//  Created by MORIOKAToyoshi on 2017/05/18.
//
//

#ifndef akagawaView_h
#define akagawaView_h

#include "fotonLiveBase.h"
#include "ofxGui.h"

class akagawaView: public fotonLiveBase{
    
    typedef struct{
        float radius;
        float scale;
        float life;
        ofVec3f position;
        ofVec3f rotate;
    }circleSet;
    
    enum e_drawPattern{box =0, circle, both};
public:
    akagawaView();
    ~akagawaView();
    void draw();
    
    void setRotatePattern(int i);
    
    void changeTrigger();
    void toggleGui();
private:
    ofEasyCam cam;
    
    bool showGui;
    ofxPanel panel;
    ofParameter<ofFloatColor> lineColor;
    ofParameter<ofColor> bgColor;
    
    ofParameter<int>loopNum;
    ofParameter<ofVec3f>translate;
    ofParameter<ofVec3f>targetLoopRotate;
    ofParameter<ofVec3f>targetGlobalRotate;

    ofParameter<float> scale;
    ofParameter<float> addVol;
    ofParameter<float> circleMaxLife;
    ofParameter<ofVec3f> circleAddPos;
    ofParameter<ofVec3f> circleAddRot;
    ofParameter<float> circleDefaultRad;
    ofParameter<float> circleFadeScale;

    ofParameter<bool> drawBox;
    ofParameter<bool> drawCircle;
    ofParameter<int>drawPattern;

    ofParameter<bool> autoRotate;

//    e_drawPattern drawPattern;
    ofVec3f boxTranslate;
    ofVec3f loopRotate;
    ofVec3f globalRotate;

    ofVec3f autoRotateForce;

    ofLight light;
    ofVec3f addTransform;
    
    ofShader shader;
    ofFbo myFbo;
    
    float lastBeatTime;
    float lastbpm;
    int count;
    
    vector<float> v_audio;
    
    vector<circleSet> circles;
    
    //trigger
    int totalCount;
    
    int rotatePattern;
    
};

#endif /* akagawaView_h */
