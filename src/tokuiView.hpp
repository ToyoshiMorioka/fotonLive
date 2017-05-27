//
//  tokuiView.hpp
//  fotonLive
//
//  Created by Taro Tokui on 2017/05/26.
//
//

#ifndef tokuiView_hpp
#define tokuiView_hpp

#include "fotonLiveBase.h"
#include "fotonConst.h"

#include "ofxGui.h"

static const int CAMERA_POSITION_COUNT = 10;

struct pingPongBuffer {
public:
    void allocate( int _width, int _height, int _internalformat = GL_RGBA, int _numColorBuffers = 1){
        // Allocate
        ofFbo::Settings fboSettings;
        fboSettings.width  = _width;
        fboSettings.height = _height;
        fboSettings.numColorbuffers = _numColorBuffers;
        fboSettings.useDepth = false;
        fboSettings.internalformat = _internalformat;	// Gotta store the data as floats, they won't be clamped to 0..1
        fboSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
        fboSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
        fboSettings.minFilter = GL_NEAREST; // No interpolation, that would mess up data reads later!
        fboSettings.maxFilter = GL_NEAREST;
        for(int i = 0; i < 2; i++){
            FBOs[i].allocate(fboSettings );
        }
        
        // Clean
        clear();
        
        // Set everything to 0
        flag = 0;
        swap();
        flag = 0;
    }
    
    void swap(){
        src = &(FBOs[(flag)%2]);
        dst = &(FBOs[++(flag)%2]);
    }
    
    void clear(){
        for(int i = 0; i < 2; i++){
            FBOs[i].begin();
            ofClear(0,255);
            FBOs[i].end();
        }
    }
    
    ofFbo& operator[]( int n ){ return FBOs[n];}
    
    ofFbo   *src;       // Source       ->  Ping
    ofFbo   *dst;       // Destination  ->  Pong
private:
    ofFbo   FBOs[2];    // Real addresses of ping/pong FBO´s
    int     flag;       // Integer for making a quick swap
};

class tokuiView: public fotonLiveBase{
public:
    tokuiView();
    ~tokuiView(){}
    void reloadShader();
    void switchShader();
    void switchEffector();
    void draw();
    void changeCameraPosition();
    void updateCameraPosition();
    
    void toggleGui(){showGui =!showGui;}
private:
    
    ofVboMesh particles;
    ofShader render, updatePos;
    ofEasyCam cam;
    pingPongBuffer pingPong;
    ofVec3f emitterPos, prevEmitterPos;
    int particleNum, texRes;
    bool showTex;
    
    bool showGui;
    ofxPanel gui;
    ofParameterGroup params;
    ofParameter<float> u_timestep;
    ofParameter<float> u_scale;
    ofParameter<float> u_emitterHight;
    ofParameter<float> u_emitterRadius;
    ofParameter<ofVec3f> u_gravity;

    ofVec3f cameraPositions[CAMERA_POSITION_COUNT];
    ofVec3f cameraTargets[CAMERA_POSITION_COUNT];
    ofVec3f cameraUps[CAMERA_POSITION_COUNT];
    ofVec3f currentCameraPosition;
    ofVec3f currentCameraTarget;
    ofVec3f currentCameraUp;
    int id;
    int kick_counter;
    
    void initCameraPositions();
};

#endif /* tokuiView_hpp */
