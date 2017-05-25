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
#include "fotonConst.h"

class higashiView: public fotonLiveBase{
public:
    higashiView();
    ~higashiView(){}
    void reloadShader();
    void switchShader();
    void switchEffector();
    void draw();
    
private:
    static constexpr int historySize{64};
    array<float, historySize> temp_history();
    ofShader shader1;
    ofShader shader2;
    ofShader shader3;
    ofShader effector;
    ofVec2f resolution;
    ofFbo baseFbo;
    enum class Fragment: int{Shader1, Shader2, Shader3};
    Fragment current;
    list<float> history;
    int currentEffect{0x01};
    
};


#endif /* higashiView_hpp */
