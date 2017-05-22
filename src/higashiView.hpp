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
    void draw();
    
private:
    ofShader shader;
};


#endif /* higashiView_hpp */
