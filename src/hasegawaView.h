//
//  hasegawaView.hpp
//  fotonLive
//
//  Created by Shunki Hasegawa on 23/05/2017.
//
//

#ifndef hasegawaView_h
#define hasegawaView_h

#include "fotonLiveBase.h"
#include "fotonConst.h"

class hasegawaView: public fotonLiveBase{
    
public:
    hasegawaView();
    ~hasegawaView();
    void draw();
    
    
private:
    void triggerControll(int trigger);
    void mainDraw(float l, float r);
    
    Boolean beatFlag;
    ofVec2f resolution;
    ofFbo baseFbo;
    float lastBeatTime;
    Boolean triggerFlag;
    float triggerTime;
    
    float vol;
    
    ofLight light;
    ofMaterial material;
    ofEasyCam easyCam;
    
    ofSpherePrimitive sphere;
    
    float input;
    
    int trigger_int;
    float r_count;
    

    float rot;
    
    Boolean isTechno;
    Boolean isBgl;
    Boolean isBgp;
    
    //ofShader shader;
    
};

#endif /* hasegawaView_hpp */
