//
//  tokuiView.cpp
//  fotonLive
//
//  Created by Taro Tokui on 2017/05/26.
//
//

#include "tokuiView.hpp"

tokuiView::tokuiView(){
    
//    ofBackground(0, 0, 0);
    
    cam.setupPerspective();
    
    //パーティクルの数を設定
    particleNum = 500000;
    texRes = ceil(sqrt(particleNum));
    
    // レンダリング用シェーダーの読み込み
    render.load("tokui/render");
    
    // アップデート用シェーダーの読み込み
    updatePos.load("","tokui/update.frag");
    
    // パーティクルの初期設定
    particles.setMode(OF_PRIMITIVE_POINTS);
    for(int i=0;i<texRes;i++){
        for(int j=0;j<texRes;j++){
            int index = i * texRes + j;
            if(index < particleNum){
                particles.addVertex(ofVec3f(0,0,0));
                particles.addTexCoord(ofVec2f(i, j)); // Fboのテクスチャー内で、データを読み出す位置を設定
                particles.addColor(ofFloatColor(1.0,1.0,1.0,0.5));
            }
        }
    }
    
    
    // パーティクルの座標・加速度の保存用Fbo
    // RGBA32Fの形式で2つのColorbufferを用意
    pingPong.allocate(texRes, texRes, GL_RGBA32F, 2);
    
    // パーティクルの位置と経過時間の初期設定
    float * posAndAge = new float[texRes * texRes * 4];
    for (int x = 0; x < texRes; x++){
        for (int y = 0; y < texRes; y++){
            int i = texRes * y + x;
            posAndAge[i*4 + 0] = ofRandom(-1.0,1.0);
            posAndAge[i*4 + 1] = ofRandom(-1.0,1.0);
            posAndAge[i*4 + 2] = ofRandom(-1.0,1.0);
            posAndAge[i*4 + 3] = 0;
        }
    }
    
    //pingPongBufferに初期値を書き込み
    pingPong.src->getTextureReference(0).loadData(posAndAge, texRes, texRes, GL_RGBA);
    delete [] posAndAge;
    
    // パーティクルの速度と生存期間の初期設定
    float * velAndMaxAge = new float[texRes * texRes * 4];
    for (int x = 0; x < texRes; x++){
        for (int y = 0; y < texRes; y++){
            int i = texRes * y + x;
            velAndMaxAge[i*4 + 0] = 0.0;
            velAndMaxAge[i*4 + 1] = 0.0;
            velAndMaxAge[i*4 + 2] = 0.0;
            velAndMaxAge[i*4 + 3] = ofRandom(1,150);
        }
    }
    
    //pingPongBufferに初期値を書き込み
    pingPong.src->getTextureReference(1).loadData(velAndMaxAge, texRes, texRes, GL_RGBA);
    delete [] velAndMaxAge;
    
    showTex = false;
    
    // init gui
    params.setName("settings");
    params.add(u_timestep.set("u_timestep",0.0,0.0,0.01));
    params.add(u_scale.set("u_scale",0.00,0.0,0.001));
    params.add(u_emitterHight.set("u_emitterHight",250,0,500));
    params.add(u_emitterRadius.set("u_emitterRadius",2000,0,5000));
    params.add(u_gravity.set("u_gravity",ofVec3f(0,-0.25,0),ofVec3f(-1,-1,-1),ofVec3f(1,1,1)));
    
    gui.setup(params, "tokui_settings.xml");
//    gui.loadFromFile("tokui_settings.xml");
    showGui = false;
    kick_counter = 0;
    
    initCameraPositions();
}


void tokuiView::draw(){
    
    ofDisableAlphaBlending();
    
    float time = ofGetElapsedTimef();
    float kick = myAudio->hasBeat();
    
    // パーティクルの発生位置を更新
    prevEmitterPos = emitterPos;
    emitterPos = 300 * ofVec3f(ofSignedNoise(time, 0, 0),ofSignedNoise(0, time, 0),ofSignedNoise(0, 0, time));
    //        emitterPos = ofVec3f(mouseX,mouseY,0);
    emitterPos = 300 * ofVec3f(sin(time*10),0,cos(time*10));
    
    // パーティクルの位置を計算
    pingPong.dst->begin();
    
    // 複数バッファの書き出しを有効化
    pingPong.dst->activateAllDrawBuffers();
    ofClear(0);
    updatePos.begin();
    
    // パーティクルの位置と経過時間
    updatePos.setUniformTexture("u_posAndAgeTex", pingPong.src->getTextureReference(0), 0);
    
    // パーティクルの速度と生存期間
    updatePos.setUniformTexture("u_velAndMaxAgeTex", pingPong.src->getTextureReference(1), 1);
    
    updatePos.setUniform1f("u_time", time);
    updatePos.setUniform1f("u_timestep", u_timestep);
    updatePos.setUniform1f("u_scale", u_scale);
    updatePos.setUniform3f("u_emitterPos", 0, u_emitterHight, 0);
    updatePos.setUniform3f("u_prevEmitterPos", prevEmitterPos.x, prevEmitterPos.y, prevEmitterPos.z);
    updatePos.setUniform1f("u_emitterRadius", u_emitterRadius * myAudio->scaledVol);
//    updatePos.setUniform1f("u_emitterRadius", 5000 * myAudio->scaledVol);
    updatePos.setUniform1f("u_kick", kick);
    updatePos.setUniform3f("u_gravity", u_gravity.get().x, u_gravity.get().y, u_gravity.get().z);
    
    pingPong.src->draw(0, 0);
    updatePos.end();
    
    pingPong.dst->end();
    pingPong.swap();
    
    
    ofBackground(0);
    
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnablePointSprites();
    
    if(kick)
    {
        kick_counter++;
        if (kick_counter>=4) {
            kick_counter = 0;
            changeCameraPosition();
        }
    }
//    cout << myAudio->bpm() << endl;
    updateCameraPosition();
    cam.lookAt(currentCameraTarget, currentCameraUp);
    cam.setPosition(currentCameraPosition);
    
//    cout << "------------" << endl;
//    cout << cam.getPosition() << endl;
//    cout << cam.getLookAtDir() << endl;
//    cout << cam.getUpDir() << endl;
    
    cam.begin();
    render.begin();
    
    // パーティクルの位置と経過時間
    render.setUniformTexture("u_posAndAgeTex", pingPong.src->getTextureReference(0), 0);
    particles.draw();
    render.end();
    cam.end();
    ofDisablePointSprites();
    ofPopStyle();
    
    ofSetColor(255);
    if(showGui) gui.draw();
    
    ofEnableAlphaBlending();
}


void tokuiView::changeCameraPosition()
{
    id++;
    if(id >= CAMERA_POSITION_COUNT)
    {
        id = 0;
    }
//    index = ofRandom(0.99999)*CAMERA_POSITION_COUNT;
//    cout << index << endl;
//    currentCameraPosition = cameraPositions[index];
//    currentCameraTarget = cameraTargets[index];
//    currentCameraUp = cameraUps[index];
    
    
}

void tokuiView::updateCameraPosition()
{
    ofVec3f diff_position = cameraPositions[id] - currentCameraPosition;
    ofVec3f diff_target = cameraTargets[id] - currentCameraTarget;
    ofVec3f diff_up = cameraUps[id] - currentCameraUp;
    
    currentCameraPosition += diff_position / 10;
    currentCameraTarget += diff_target / 10;
    currentCameraUp += diff_up / 10;
}

void tokuiView::initCameraPositions()
{
    
    cameraPositions[0] = ofVec3f(485.817, -158.004, -119.938);
    cameraTargets[0] = ofVec3f(-0.964549, 0.195472, 0.177301);
    cameraUps[0] = ofVec3f(-0.11149, 0.307121, -0.945117);
    
    cameraPositions[1] = ofVec3f(156.542, -572.78, -722.646);
    cameraTargets[1] = ofVec3f(-0.16737, 0.612397, 0.77263);
    cameraUps[1] = ofVec3f(-0.0942158, -0.790033, 0.605782);
    
    cameraPositions[2] = ofVec3f(46.093, 298.01, 112.261);
    cameraTargets[2] = ofVec3f(-0.14325, -0.926155, -0.348879);
    cameraUps[2] = ofVec3f(0.844376, 0.0695078, -0.531222);
    
    cameraPositions[3] = ofVec3f(99.5797, -364.358, -459.691);
    cameraTargets[3] = ofVec3f(-0.16737, 0.612397, 0.77263);
    cameraUps[3] = ofVec3f(-0.0942158, -0.790033, 0.605782);
    
    cameraPositions[4] = ofVec3f(247.149, -904.307, -1140.92);
    cameraTargets[4] = ofVec3f(-0.16737, 0.612397, 0.77263);
    cameraUps[4] = ofVec3f(-0.0942158, -0.790033, 0.605782);
    
    cameraPositions[5] = ofVec3f(-283.759, -891.261, -1142.66);
    cameraTargets[5] = ofVec3f(0.192162, 0.603563, 0.773813);
    cameraUps[5] = ofVec3f(-0.0892162, -0.774499, 0.626253);
    
    cameraPositions[6] = ofVec3f(-148.161, -465.359, -596.625);
    cameraTargets[6] = ofVec3f(0.192162, 0.603563, 0.773813);
    cameraUps[6] = ofVec3f(-0.0892162, -0.774499, 0.626253);
    
    cameraPositions[7] = ofVec3f(302.049, 525.861, -107.658);
    cameraTargets[7] = ofVec3f(-0.490409, -0.853784, 0.174794);
    cameraUps[7] = ofVec3f(0.870842, -0.487832, 0.0604443);
    
    cameraPositions[8] = ofVec3f(-24.2834, -320.129, -21.5616);
    cameraTargets[8] = ofVec3f(0.0754627, 0.994895, 0.0670091);
    cameraUps[8] = ofVec3f(0.931831, -0.0464385, -0.359908);
    
    cameraPositions[9] = ofVec3f(2.1512, 537.348, 0.280567);
    cameraTargets[9] = ofVec3f(-0.00400484, -0.999992, -0.000519938);
    cameraUps[9] = ofVec3f(0.846884, -0.00311518, -0.531769);
    
    
    currentCameraPosition = cameraPositions[0];
    currentCameraTarget = cameraTargets[0];
    currentCameraUp = cameraUps[0];

    id = 0;
}
