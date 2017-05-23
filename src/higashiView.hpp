//
//  higashiView.hpp
//  fotonLive
//
//  Created by user on 2017/05/22.
//
//

#ifndef higashiView_hpp
#define higashiView_hpp

#include "fotonLiveBase.h"

class higashiView: public fotonLiveBase{
public:
    higashiView();
    ~higashiView(){}
    void reloadShader();
    void draw();
    
private:
    ofShader shader;
    ofShader effector;
    ofVec2f resolution;
    ofFbo baseFbo;
};


#endif /* higashiView_hpp */
